#!/bin/bash

cp -r $CLIENT_HOME/Tools/effSpineADR .

for subfile in `ls effSpineADR|grep .png`
do
	etcpack "effSpineADR/"$subfile effSpineADR -c etc1 -f RGB -as -ext PNG
	rm -rf "effSpineADR/"$subfile
done

#rm -rf $ADR_HOME/assets/ipad/spine
cp -r effSpineADR/* $ADR_HOME/assets/ipad/effSpine/
rm -rf effSpineADR



		
