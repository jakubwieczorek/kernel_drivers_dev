#/bin/bash

set -e

#download Buildroot
wget https://buildroot.org/downloads/buildroot-2019.08.1.tar.bz2

#unpack it
tar -xjf buildroot-2019.08.1.tar.bz2

#set the path to the Buildroot environment
export BRPATH=`pwd`/buildroot-2019.08.1

#Copy configuration
cp .config ${BRPATH}

#Compile Buildroot
(
  set -e
  cd ${BRPATH}
  make
)
