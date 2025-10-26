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

case $USEQTVERSION in
	"5")
		autoconf configure.qt5.ac > ./configure
		chmod +x ./configure
		;;
	*)
		autoconf configure.ac > ./configure
		;;
esac

autoconf
./configure $@

