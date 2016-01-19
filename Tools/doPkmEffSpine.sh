#!/bin/bash
if [ "$MALI_HOME" == "" ];then 
	echo "define MALI_HOME first, example:/Users/cgp/soft/Mali_Texture_Compression_Tool_v4.1.0.257_MacOSX_x64/bin"
	exit 1
fi

cd $MALI_HOME
./localPkmEffSpine.sh

