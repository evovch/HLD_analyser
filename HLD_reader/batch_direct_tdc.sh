#!/bin/bash

DIR=/home/evovch/Documents/Analysis_Sep2016/directTDC/config/

MODE=direct_tdc_global_pseudocalib

for CFGFILE in ${DIR}*.cfg
do
    echo "${MODE}: ${CFGFILE}"
    ../build-HLD_reader-Desktop_Qt_5_7_0_GCC_64bit-Debug/HLD_reader ${CFGFILE} ${MODE} >> ${DIR}/../out_${MODE}.txt 2>> ${DIR}/../err_${MODE}.txt
done
