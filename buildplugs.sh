#!/bin/bash

#©keithhedger Sat 12 Mar 19:03:19 GMT 2022 kdhedger68713@gmail.com

BUILDLANGPLUGS=${BUILDLANGPLUGS:-1}
BUILDPLUGS=${BUILDPLUGS:-1}
BUILDTOOLKITPLUGS=${BUILDTOOLKITPLUGS:-1}

INSTALLTO="$2"
export INSTALLTO

if [[ "$1" = "clean" ]];then
	find KKEditQT/languageplugins -type d -iname "build" -exec rm -rf '{}' \; &>/dev/null
	find KKEditQT/plugins -type d -iname "build" -exec rm -rf '{}' \; &>/dev/null
	find KKEditQT/toolkitplugins -type d -iname "build" -exec rm -rf '{}' \; &>/dev/null
	exit 0
fi

if [ $BUILDLANGPLUGS -eq 1 ];then
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
		fi
	popd
fi

if [ $BUILDPLUGS -eq 1 ];then
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
		fi
	popd
fi

if [ $BUILDTOOLKITPLUGS -eq 1 ];then
	pushd KKEditQT/toolkitplugins
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
		fi
	popd
fi


