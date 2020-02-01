#/bin/bash

GOLDEN='\033[0;33m'
NC='\033[0m'

export BRPATH=`pwd`/buildroot-2019.08.1

mkdir -p ${BRPATH}/output/target/demo

set -e
cd modules_3
./build

cp *.ko ${BRPATH}/output/target/demo
cd ..
cp i2c_test.py ${BRPATH}/output/target/demo

echo -e "${GOLDEN} kernel still requires to be built under $BRPATH ${NC}"
