#!/bin/bash

DIR=/home/evovch/Documents/Analysis_Sep2016/config/
SUMFILENAME=analysis_sum_laser_calib_nocorr.root

#cd ${DIR}../analysis_results_laser_calib_nocorr/
#hadd ${SUMFILENAME} analysis_info_*.root
#cd $OLDPWD

../build-CorrectionsBuilder-Desktop_Qt_5_6_0_GCC_64bit-Release/CorrectionsBuilder ${DIR}../analysis_results_laser_calib_nocorr/${SUMFILENAME} ${DIR}../corrections.txt
