#!/bin/bash

DIR=/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/config/

for CFGFILE in ${DIR}*
do
    echo "rings_nocalib_nocorr: ${CFGFILE}"
    ../build-HLD_reader-Desktop_Qt_5_7_0_GCC_64bit-Debug/HLD_reader ${CFGFILE} rings_nocalib_nocorr >> ${DIR}/../out_rings_nocalib_nocorr.txt 2>> ${DIR}/../err_rings_nocalib_nocorr.txt
done
