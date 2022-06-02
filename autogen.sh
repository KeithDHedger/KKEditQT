#!/bin/bash

./remakesourcelist
aclocal
autoheader
touch NEWS AUTHORS
automake --add-missing --copy
autoconf
./configure $@

