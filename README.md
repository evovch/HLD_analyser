# HLD_analyser

This standalone code implements the data analysis for the RICH detector of the CBM experiment.

It has been developed in order to avoid the usage of the massive CbmRoot framework (fairroot.gsi.de) to perform rather specific analysis of the relatively small set of data gathered during the common CBM beam tests at PS@CERN during November 2014.

It implements all the necessary general steps like raw data unpacking, fine time calibration, inter-channel delays correction, hit building (edge matching), event building, ring reconstruction and specific procedures for the time-related data analysis.

This C++ project is developed using the Qt environment and utilizes the Qt-Root interface developed in GSI for the Go4 project (go4.gsi.de). The ring finding and fitting is taken from the CmbRoot sources and adopted with the help of the author S. Lebedev. While working at the similar functionality inside CbmRoot it turned out that the edge matching procedure is the most time consuming. In this implementation the data processing pipeline has been designed from the start to enable the parallelization of the edge matching procedure using the OpenMP "parallel for".

The software takes an input HLD file of a certain format, containing raw data received from the detector, and produces a comprehensive set of histograms allowing to control the result of each stage.

In case of interest, please, contact evovch@gmail.com.
