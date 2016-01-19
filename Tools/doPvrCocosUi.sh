#!/bin/bash
cp -r cocosUi cocosUiPvr
cd cocosUiPvr

#for subfile in `ls`
#	do 
#		#filePath=$sourceDir"/"$file
#		if [ -d $subfile ]
#		then
#			TexturePacker --data $subfile.plist --format cocos2d --disable-rotation  --opt RGBA8888 --premultiply-alpha --texture-format pvr2ccz --sheet $subfile.pvr.ccz $subfile/*.png
#			rm -rf $subfile/*
#			mv  $subfile.pvr.ccz $subfile/
#			mv  $subfile.plist $subfile/
#		fi
#done

for subfile in `ls |grep .png`
do
	#去掉.png后缀
	fileName=${subfile%.*}

	TexturePacker --format cocos2d --disable-rotation  --opt RGBA8888 --premultiply-alpha --texture-format pvr2ccz --sheet $fileName.pvr.ccz $subfile
	rm -rf $subfile
done

cp -r * $CLIENT_HOME/GameSanGuo/GameSanGuo/Resources/ipad/cocosUi/
cd ..
rm -rf cocosUiPvr




		
