#!/bin/bash

#©keithhedger Sat 12 Mar 19:03:19 GMT 2022 kdhedger68713@gmail.com

#MAXJOBS=12 BUILDPLUGS=1 BUILDTOOLKITPLUGS=1 BUILDLANGPLUGS=1 LOCAL=1 ./buildplugs.sh install

NORMAL='\e[0m'
RED='\e[1;31m'
GREEN='\e[1;32m'
YELLOW='\e[1;33m'
HILITE='\e[1;37;41m'

echo -e "${RED}Running buildplugs.sh ...${NORMAL}"

if [ -e ./toolspath ];then
	. ./toolspath
else
	./setuptools
	. ./toolspath
fi

BUILDLANGPLUGS=${BUILDLANGPLUGS:-1}
BUILDPLUGS=${BUILDPLUGS:-1}
BUILDTOOLKITPLUGS=${BUILDTOOLKITPLUGS:-1}

export LOCAL=${LOCAL:-0}

INSTALLTO="$2"
#export MAXJOBS=$(( ( $(nproc) -3 ) /3 ))

MAXJOBS=${MAXJOBS:-1}
export MAXJOBS
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

waitforjobs()
{
    while test $(jobs -p | wc -w) -gt "$1"; do wait -n; done
}

buildPlug ()
{
	WHAT="$1"
	WHERE="$2"
	THISDIR=$(basename $(pwd))
	PARENTDIR="$(basename "$(dirname $(pwd))")"

	if [ -e pass ];then
		return
	fi

	echo -e "${NORMAL}${GREEN}In ${GREEN}$PARENTDIR doing $WHAT on $THISDIR ...${NORMAL}"
	case $WHAT in
		"clean"|"distclean")
			rm -rf build
			;;
		"build")
			mkdir -p build &>/dev/null
			cd build &>/dev/null
			$QMAKEPATH .. &>/dev/null
			make||exit 100
			;;
		 "install")
			mkdir -p build &>/dev/null
			cd build &>/dev/null
			$QMAKEPATH .. &>/dev/null
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
	pushd KKEditQT/langplugins &>/dev/null
		if [ "X$1" = "Xsingle" ];then
			pushd "$2" &>/dev/null
				LOCAL=1 buildPlug "install"
			popd &>/dev/null
		else
			while read
				do
					pushd $(dirname $REPLY) &>/dev/null
						waitforjobs $MAXJOBS
						( buildPlug "$1" "$2" ) &
					popd &>/dev/null
				done < <(find -maxdepth 2 -iname "*.pro")
		fi
	popd
fi

if [ $BUILDPLUGS -eq 1 ];then
	pushd KKEditQT/plugins &>/dev/null
		if [ "X$1" = "Xsingle" ];then
			pushd "$2" &>/dev/null
				LOCAL=1 buildPlug "install"
			popd &>/dev/null
		else
			while read
				do
					pushd $(dirname $REPLY) &>/dev/null
						waitforjobs $MAXJOBS
						( buildPlug "$1" "$2" ) &
					popd &>/dev/null

				done < <(find -maxdepth 2 -iname "*.pro")
		fi
	popd
fi

if [ $BUILDTOOLKITPLUGS -eq 1 ];then
	pushd KKEditQT/toolkitplugins &>/dev/null
		if [ "X$1" = "Xsingle" ];then
			pushd "$2" &>/dev/null
				LOCAL=1 buildPlug "install"
			popd &>/dev/null
		else
			while read
				do
					pushd $(dirname $REPLY) &>/dev/null
						waitforjobs $MAXJOBS
						( buildPlug "$1" "$2" ) &
					popd &>/dev/null
				done < <(find -maxdepth 2 -iname "*.pro")
		fi
	popd
fi

