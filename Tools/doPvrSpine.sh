#!/bin/bash

cp -r spineIOS spinePvr
cd spinePvr
for subfile in `ls |grep .png`
do
	#去掉.png后缀
	fileName=${subfile%.*}

	TexturePacker --format cocos2d --disable-rotation  --opt RGBA8888 --premultiply-alpha --texture-format pvr2ccz --sheet $fileName.pvr.ccz $subfile
	rm -rf $subfile
done

cp -r * $CLIENT_HOME/GameSanGuo/GameSanGuo/Resources/ipad/spine/
cd ..
rm -rf spinePvr




		
