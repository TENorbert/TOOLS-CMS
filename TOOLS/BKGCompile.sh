#!/bin//bash
g++ BackgroundEstimation.cc  MathFunctions.cc -o BkGEstimate `root-config --cflags --libs` 
#g++ -c `root−config --cflags --glibs` FitHist.cc
#g++ -o HFit `root-config --glibs` FitHist.o
