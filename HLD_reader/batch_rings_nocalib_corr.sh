#!/bin/bash

DIR=/home/evovch/Documents/Analysis_Sep2016/config/

for CFGFILE in ${DIR}*
do
    echo "rings_nocalib_corr: ${CFGFILE}"
    ../build-HLD_reader-Desktop_Qt_5_7_0_GCC_64bit-Debug/HLD_reader ${CFGFILE} rings_nocalib_corr >> ${DIR}/../out_rings_nocalib_corr.txt 2>> ${DIR}/../err_rings_nocalib_corr.txt
done
