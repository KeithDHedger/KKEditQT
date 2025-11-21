#!/bin/bash

if [ -e ./toolspath ];then
	. ./toolspath
else
	./setuptools
	. ./toolspath
fi

./remakesourcelist

aclocal
autoheader
touch NEWS README AUTHORS ChangeLog
automake --add-missing --copy
autoconf

./configure $@

