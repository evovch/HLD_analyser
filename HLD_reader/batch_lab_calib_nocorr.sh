#!/bin/bash

DIR=/home/evovch/Documents/Analysis_Sep2016/threshold_scan_2/config/

for CFGFILE in ${DIR}*.cfg
do
    echo "laboratory_calib_nocorr: ${CFGFILE}"
    ../build-HLD_reader-Desktop_Qt_5_7_0_GCC_64bit-Debug/HLD_reader ${CFGFILE} laboratory_calib_nocorr >> ${DIR}/../out_lab_calib_nocorr_.txt 2>> ${DIR}/../err_lab_calib_nocorr_.txt
done
