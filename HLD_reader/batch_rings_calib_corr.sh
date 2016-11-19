#!/bin/bash

DIR=/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/config/

for CFGFILE in ${DIR}*
do
    echo "rings_calib_corr: ${CFGFILE}"
    ../build-HLD_reader-Desktop_Qt_5_7_0_GCC_64bit-Debug/HLD_reader ${CFGFILE} rings_calib_corr >> ${DIR}/../out_rings_calib_corr.txt 2>> ${DIR}/../err_rings_calib_corr.txt
done
