#!/bin/bash

#©keithhedger Sat 12 Mar 19:03:19 GMT 2022 kdhedger68713@gmail.com

if [[ "$1" = "clean" ]];then
	find KKEditQT/languageplugins -type d -iname "build" -exec rm -rf '{}' \; &>/dev/null
	find KKEditQT/plugins -type d -iname "build" -exec rm -rf '{}' \; &>/dev/null
	exit 0
fi

pushd KKEditQT/languageplugins
	if [[ "$1" = "build" ]];then
		while read
			do
				pushd $(dirname $REPLY)
					mkdir -p build && cd build
					qmake ..
					make -j3
				popd
			done < <(find -maxdepth 2 -iname "*.pro")
	else
		while read
			do
				pushd $(dirname $REPLY)
					cd build
					make -j3 install
				popd
			done < <(find -maxdepth 2 -iname "*.pro")
	mkdir "${2}/langplugins"
	mv -v /tmp/langplugins/* "${2}/langplugins"
	fi
popd

pushd KKEditQT/plugins
	if [[ "$1" = "build" ]];then
		while read
			do
				pushd $(dirname $REPLY)
					mkdir -p build && cd build
					qmake ..
					make -j3
				popd
			done < <(find -maxdepth 2 -iname "*.pro")
	else
		while read
			do
				pushd $(dirname $REPLY)
					cd build
					make -j3 install
				popd
			done < <(find -maxdepth 2 -iname "*.pro")
	mkdir "${2}/plugins"
	mv -v /tmp/plugins/* "${2}/plugins"
	fi
popd
