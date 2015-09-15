#!/bin/sh -xe 

TOP=$(cd $(dirname $0); pwd -P) 

CCFLAG="-I${TOP} -I${TOP}/api -g -DHAVE_DEBUG -fpermissive -Wall -c -o"

BUILD_FLAG="-fPIC -lm -lpthread -o"

CC="g++ ${CCFLAG}"

BUILD="g++ ${BUILD_FLAG}"

ARC="ar cru"

LIB="ranlib"

cd $TOP

$CC ZQTD.o ZQTD.cpp
$CC ZQMD.o ZQMD.cpp
$CC ctpstock.o CTPSTOCK.cpp
$BUILD ctpstock ctpstock.o ZQTD.o ZQMD.o  $TOP/api/thosttraderapiSSE.so $TOP/api/thostmduserapiSSE.so $TOP/libmysql/lib/libmysqlclient.a -levent -lpthread
