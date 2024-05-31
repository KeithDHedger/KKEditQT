#!/bin/bash

#©keithhedger Sat 12 Mar 19:03:19 GMT 2022 kdhedger68713@gmail.com

BUILDLANGPLUGS=${BUILDLANGPLUGS:-1}
BUILDPLUGS=${BUILDPLUGS:-1}
BUILDTOOLKITPLUGS=${BUILDTOOLKITPLUGS:-1}

export LOCAL=${LOCAL:-0}

INSTALLTO="$2"
export INSTALLTO

if [[ "$1" = "clean" ]];then
	find KKEditQT/langplugins -type d -iname "build" -exec rm -rf '{}' \;
	find KKEditQT/plugins -type d -iname "build" -exec rm -rf '{}' \;
	find KKEditQT/toolkitplugins -type d -iname "build" -exec rm -rf '{}' \;
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

	case $WHAT in
		"clean")
			rm -rf build
			;;
		"build")
			mkdir -vp build
			cd build
			qmake ..
			make
			;;
		 "install")
			mkdir -vp build
			cd build
		 	make install
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

