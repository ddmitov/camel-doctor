#!/bin/sh

makedmg_version='v0.3'
macdeployqt_bin=$(which macdeployqt)
cameldoc_bundle=$1
cameldoc_pro=$(dirname $0)"/src/cameldoc.pro"

require_bundle_config() {
	if [ -z "$(egrep -v '^\s*\t*#' $cameldoc_pro | grep 'BUNDLE=1')" ]
		then
		echo "Failed to create dmg!"
		echo "Please, set BUNDLE=1 in $cameldoc_pro file and rebuild project."
		echo
		echo "Here is one liner to update your configuration: "
		echo "   perl -pi -e s,BUNDLE=0,BUNDLE=1,g $cameldoc_pro"
		echo
		exit 1
	fi
}

create_dmg() {
  echo "Starting package creation..."
  echo "$macdeployqt_bin $cameldoc_bundle -dmg"
  $macdeployqt_bin $cameldoc_bundle -dmg
  outdir=$(dirname $cameldoc_bundle)
  ls "$outdir/cameldoc.dmg"
  echo "Finished."
}

echo "Camel Doctor DMG Packer, $makedmg_version"
echo "Usage: makedmg.sh cameldoc.app"
echo 

require_bundle_config

if [ -z "$macdeployqt_bin" ]
	then
	echo "No macdeployqt found in PATH"
	if [ -z "$QTDIR" ]
		then
	    echo "Need to set QTDIR environment variable."
	    echo "Enter full path to the main Qt SDK directory:"
	    read qtdirname
	    set QTDIR = $qtdirname
	    if [ -z "$QTDIR" ]
	    	then
	    	echo "No QTDIR was set. I have to quit now."
	    	exit
	    fi
	    echo ""
	fi
	macdeployqt_bin="$QTDIR/bin/macdeployqt"
fi

if [ -z "$1" ]
	then
    echo "No command line argument supplied."
    echo "Trying to guess where cameldoc.app is."
    peb_bundle=$(ls -d */cameldoc.app)
    if [ -z "$cameldoc_bundle" ]
        then
        printf "Enter full or relative path to cameldoc.app: "
        read peb_bundle
    else
        echo "Found [$cameldoc_bundle]"
    fi
fi

create_dmg
exit 0
