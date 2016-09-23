#!/bin/bash

DIR=/home/evovch/Documents/Analysis_Sep2016/WLS_off/config/

for CFGFILE in ${DIR}*
do
    echo "rings_calib_nocorr: ${CFGFILE}"
    ../build-HLD_reader-Desktop_Qt_5_7_0_GCC_64bit-Debug/HLD_reader ${CFGFILE} rings_calib_nocorr >> ${DIR}/../out_rings_calib_nocorr.txt 2>> ${DIR}/../err_rings_calib_nocorr.txt
done
