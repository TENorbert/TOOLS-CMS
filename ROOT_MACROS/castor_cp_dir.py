#!/usr/bin/env python
#
# Example use at command prompt:
#   castor_cp_dir.py /castor/cern.ch/user/g/gaultney/SergueiTrees/Commissioning10_PromptReco_v8
#
# Creates a dir in the current directory, and fills it
# with copies of files from the given castor dir.
#
# Michael Anderson, with thanks to Will Maier
# Apr 6, 2010

import sys                    # For exiting program
if sys.version_info < (2, 6):
    print "Go to CMSSW dir and type 'cmsenv' first. (this loads a modern version of python)"
    sys.exit(0)

import commands               # Allows running of shell commands
import os                     # For making directories
from datetime import datetime # For timing how long this takes
import multiprocessing
from Queue import Empty

simultaneous_downloads = 20  # Optimal number of parallel downloads is unknown

# Copies file
def cpFile(queue, fromDir, toDir, globalVars):
    while True:
        try:
            fileName = queue.get(timeout=5)
        except Empty:
            break
        fromFile = '/'.join( (fromDir, fileName) )
        toFile   = '/'.join( (  toDir, fileName) )
        # Check if file already exists
        if (os.path.isfile(toFile)):
            print "%s exists, skipping..." % toFile
        else:
            # Copy from castor
            commands.getoutput("rfcp %s %s" % (fromFile, toFile))
            globalVars.filesDownloaded += 1
            print "  (%i/%i) %s  copied." % (globalVars.filesDownloaded, globalVars.numFilesToCopy, fileName)
        queue.task_done()

# Tells person how long this all took
def timeTakenString(start, end):
    timeTaken = end - start
    hours, remainder = divmod(timeTaken.seconds, 3600)
    minutes, seconds = divmod(remainder, 60)
    if hours>0:
        return "%i hours, %i minutes" % (hours, minutes)
    elif minutes>0:
        return "%i minutes" % minutes
    return "%i seconds" % seconds


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print "Usage:\n  %s /castor/cern.ch/user/..." % sys.argv[0]
        sys.exit(0)
    fromDir = sys.argv[1]

    # Get list of Files by converting getoutput string to a list with split()
    listOfAllFiles = commands.getoutput("nsls %s" % fromDir).split('\n')
    if "No such file" in listOfAllFiles[0]:
        print "Dir does not exist:\n  %s" %  fromDir
        sys.exit(0)

    # Grab the last dir name - basically change "dirA/subDirB/" to "subDirB"
    toDir = fromDir.split("/")[-1]
    if toDir == '': toDir = fromDir.split("/")[-2]
    toDir = os.getcwd() + "/" + toDir

    # Check if output dir exists
    if os.path.isdir(toDir):
        listOfAlreadyCopiedFiles = os.listdir(toDir)
        listOfFilesToCopy = list(set.difference(set(listOfAllFiles),set(listOfAlreadyCopiedFiles)))
        print "Dir exists:\n  %s\nwith %i files, will copy only new files.\n" % (toDir, len(listOfAlreadyCopiedFiles))
    else:
        os.mkdir(toDir)
        listOfFilesToCopy = listOfAllFiles

    numFilesToCopy = len(listOfFilesToCopy)
    if numFilesToCopy == 0:
        print "No new files to copy, quitting..."
        sys.exit(0)
    
    # Put every file name into the queue
    mainQueue = multiprocessing.JoinableQueue()
    for fileName in listOfFilesToCopy:
        mainQueue.put( fileName )


    # Create a counter (variable shared among downloaders)
    globalVars = multiprocessing.Manager().Namespace()
    globalVars.filesDownloaded = 0
    globalVars.numFilesToCopy  = numFilesToCopy

    # Start up the threads in parallel
    print "Copying %i files from\n  %s\nto\n  %s\n" % (numFilesToCopy, fromDir, toDir)
    if (simultaneous_downloads > numFilesToCopy): simultaneous_downloads = numFilesToCopy
    print "Starting %i parallel downloaders..." % (simultaneous_downloads)
    startTime = datetime.now()
    for i in range(simultaneous_downloads):
        p = multiprocessing.Process(target=cpFile, name="worker-%i"%i, args=(mainQueue, fromDir, toDir, globalVars))
        p.daemon = True
        p.start()
    
    # Wait for processes to finish
    mainQueue.join()

    # Finish up
    endTime = datetime.now()
    timeTaken = timeTakenString(startTime, endTime)
    numFiles  = len(os.listdir(toDir))  # Independent check of # of files
    print "%s files copied in %s.\nTotal of %i files stored in\n   %s/" % (numFilesToCopy, timeTaken, numFiles, toDir)
