#/bin/bash

export BRPATH=`pwd`/buildroot-2019.08.1

mkdir -p ${BRPATH}/output/target/demo

set -e
cd modules
./build

if [ $? == 0 ]; then
 cp *.ko ${BRPATH}/output/target/demo
else
 echo Error occured 
 exit $?
fi

cd ..

cp test_adc.py ${BRPATH}/output/target/demo

set -e
cd ${BRPATH}
make

