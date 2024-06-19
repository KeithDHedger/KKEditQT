#!/bin/bash

#©keithhedger Sat 12 Mar 19:03:19 GMT 2022 kdhedger68713@gmail.com

NORMAL="\e[0m"
RED="\e[1;31m"
GREEN="\e[1;32m"

echo -e "${RED}Running buildplugs.sh ...${NORMAL}"

if [ -e ./toolspath ];then
	. ./toolspath
else
	./setuptools
fi

BUILDLANGPLUGS=${BUILDLANGPLUGS:-1}
BUILDPLUGS=${BUILDPLUGS:-1}
BUILDTOOLKITPLUGS=${BUILDTOOLKITPLUGS:-1}

export LOCAL=${LOCAL:-0}

INSTALLTO="$2"
export INSTALLTO

if [[ "$1" = "clean" ]];then
	find KKEditQT/langplugins -type d -iname "build" -exec rm -rf '{}' \; 2>/dev/null
	find KKEditQT/plugins -type d -iname "build" -exec rm -rf '{}' \; 2>/dev/null
	find KKEditQT/toolkitplugins -type d -iname "build" -exec rm -rf '{}' \; 2>/dev/null
	exit 0
fi

if [ ${LOCAL:-0} -eq 1 ];then
	unset DESTDIR
fi

buildPlug ()
{
	WHAT="$1"
	WHERE="$2"
	THISDIR=$(basename $(pwd))
	PARENTDIR="$(basename "$(dirname $(pwd))")"

	if [ -e pass ];then
		return
	fi

	echo -e "${GREEN}In $PARENTDIR doing $WHAT on $THISDIR ...${NORMAL}"

	case $WHAT in
		"clean")
			rm -rf build
			;;
		"build")
			mkdir -vp build
			cd build
			$QMAKEPATH ..
			make||exit 100
			;;
		 "install")
			mkdir -vp build
			cd build
			$QMAKEPATH ..
		 	make install||exit 100
			if [ ${LOCAL:-0} -eq 1 ];then
				mkdir -vp  ~/.KKEditQT/$PARENTDIR/$THISDIR
				cp -r plugins/* ~/.KKEditQT/$PARENTDIR/$THISDIR
			else
				mkdir -vp "${WHERE}/$PARENTDIR/$THISDIR"
				cp -r plugins/* "${WHERE}/$PARENTDIR/$THISDIR"
			fi
		 	;;
	esac
}

if [ $BUILDLANGPLUGS -eq 1 ];then
	pushd KKEditQT/langplugins
		while read
			do
				pushd $(dirname $REPLY)
					buildPlug "$1" "$2"
				popd
			done < <(find -maxdepth 2 -iname "*.pro")
	popd
fi

if [ $BUILDPLUGS -eq 1 ];then
	pushd KKEditQT/plugins
			while read
				do
					pushd $(dirname $REPLY)
						buildPlug "$1" "$2"
					popd

				done < <(find -maxdepth 2 -iname "*.pro")
	popd
fi

if [ $BUILDTOOLKITPLUGS -eq 1 ];then
	pushd KKEditQT/toolkitplugins
		while read
			do
				pushd $(dirname $REPLY)
					buildPlug "$1" "$2"
				popd
			done < <(find -maxdepth 2 -iname "*.pro")
	popd
fi

