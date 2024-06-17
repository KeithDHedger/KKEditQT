#!/bin/bash

if [ -e ./toolspath ];then
	. ./toolspath
else
	./setuptools
	. ./toolspath
fi

if [ $QTVERSION -ne 6 ];then
	ENABLEQT6="--disable-qt6"
else
	ENABLEQT6="--enable-qt6"
fi
./remakesourcelist
aclocal
autoheader
touch NEWS AUTHORS
automake --add-missing --copy
autoconf
./configure $@ $ENABLEQT6

