#!/bin/sh

DIR=`dirname $0`

if [ ! -f "$DIR/../../../osd.c" ]; then
    echo
    echo
    echo "Warning while building ttxtsubs plugin:"
    echo $0: "Can not find VDR's osd.c so can not check for correct patch!"
    echo
    echo
    exit 0;
fi

grep vdrttxtsubshooks "$DIR/../../../osd.c" >> /dev/null;
if [ $? -ne 0 ]; then
    echo
    echo
    echo "Error while building ttxtsubs plugin:"
    echo "You have not patched your VDR - please read the README!"
    echo
    echo
    exit 1;
fi

#grep OSD_HOOK_2 "$DIR/../../../osd.c" >> /dev/null;
#if [ $? -ne 0 ]; then
#    echo
#    echo
#    echo "Error while building ttxtsubs plugin:"
#    echo "       You have an older version of the VDR patch installed!"
#    echo "       You need to upgrade it."
#    echo "       Please do:"
#    echo "        cd /path/to/vdr    [ replace with your path ]"
#    echo "        patch -b < PLUGINS/src/ttxtsubs/VDR.upgrade-patch"
#    echo "        make"
#    echo
#    exit 1;
#fi

