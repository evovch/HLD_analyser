#!/bin/bash

DIR=/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/config/

for CFGFILE in ${DIR}*
do
    echo "laser_nocalib_corr: ${CFGFILE}"
    ../build-HLD_reader-Desktop_Qt_5_7_0_GCC_64bit-Debug/HLD_reader ${CFGFILE} laser_nocalib_corr >> ${DIR}/../out_laser_nocalib_corr.txt 2>> ${DIR}/../err_laser_nocalib_corr.txt
done
