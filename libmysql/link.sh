#!/bin/sh -xe

TOP=`cd $(dirname $0); pwd -P`

cd $TOP

rm -rf include
rm -rf lib

ARCH=`uname -m`

if [ ! -d $ARCH ] && [ ! -l $ARCH ]; then
    echo "Unknow architecture $ARCH";
    exit 1;
fi

ln -sf $ARCH/include include
ln -sf $ARCH/lib lib
