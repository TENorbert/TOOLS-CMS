#!/bin/bash

# This Small tool almost gave me the hwole day to know how to use it.

# It actually Puts the input root files in the way accepted  by CMS Modules:

# Author TEN

 sed -e s/"\/hdfs"/"'file:\/hdfs"/  -e  s/"root"/"root',"/  < doubleEle.txt >& DoubleEleRun2012A.txt&
