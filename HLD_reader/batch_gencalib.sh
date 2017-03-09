#!/bin/bash

DIR=/home/evovch/Documents/Analysis_Sep2016/Time_precision_evolution_mar2017/config/

for CFGFILE in ${DIR}*.cfg
do
    echo "gencalib: ${CFGFILE}"
    ../build-HLD_reader-Desktop_Qt_5_7_0_GCC_64bit-Debug/HLD_reader ${CFGFILE} gencalib >> ${DIR}/../out_gencalib.txt 2>> ${DIR}/../err_gencalib.txt
done
