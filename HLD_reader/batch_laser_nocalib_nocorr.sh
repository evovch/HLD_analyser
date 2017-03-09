#!/bin/bash

DIR=/home/evovch/Documents/Analysis_Sep2016/Time_precision_evolution_mar2017/config/

for CFGFILE in ${DIR}*
do
    echo "laser_nocalib_nocorr: ${CFGFILE}"
    ../build-HLD_reader-Desktop_Qt_5_7_0_GCC_64bit-Debug/HLD_reader ${CFGFILE} laser_nocalib_nocorr >> ${DIR}/../out_laser_nocalib_nocorr.txt 2>> ${DIR}/../err_laser_nocalib_nocorr.txt
done
