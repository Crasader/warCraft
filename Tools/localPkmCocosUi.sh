#!/bin/bash
rm -rf cocosUi
cp -r $CLIENT_HOME/Tools/cocosUi .

for subfile in `ls cocosUi|grep .png`
do
	etcpack "cocosUi/"$subfile cocosUi -c etc1 -f RGB -as -ext PNG
	rm -rf "cocosUi/"$subfile
done

#rm -rf $ADR_HOME/assets/ipad/spine
cp -r cocosUi/* $ADR_HOME/assets/ipad/cocosUi/



		
