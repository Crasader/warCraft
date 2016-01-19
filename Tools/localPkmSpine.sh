#!/bin/bash

cp -r $CLIENT_HOME/Tools/spineADR .

for subfile in `ls spineADR|grep .png`
do
	etcpack "spineADR/"$subfile spineADR -c etc1 -f RGB -as -ext PNG
	rm -rf "spineADR/"$subfile
done

#rm -rf $ADR_HOME/assets/ipad/spine
cp -r spineADR/* $ADR_HOME/assets/ipad/spine/
rm -rf spineADR



		
