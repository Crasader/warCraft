#!/bin/bash

#TexturePacker --data $test.plist --format cocos2d --disable-rotation  --opt RGBA8888 --premultiply-alpha --texture-format pvr2ccz --sheet test.pvr.ccz test/*.png
sourceDir="Fragments"

# Get base dir name of sourceDir
sourceBaseDirName=`dirname $sourceDir`
sourceBaseName=`basename $sourceDir`


# Target dir name
targetRootDir=$sourceBaseDirName
targetBaseName="pvrcczPackage"


# mkdir target dir 
mkdir -v $targetRootDir"/"$targetBaseName

for file in `ls $sourceDir`
	do 
		filePath=$sourceDir"/"$file
		if [ -d $filePath ]
		then
			targetDir=$targetRootDir"/"$targetBaseName"/"$file
			mkdir -v $targetDir

			targetPathName=$targetDir/$file
			TexturePacker --data $targetPathName.plist --format cocos2d --disable-rotation  --opt RGBA8888 --premultiply-alpha --texture-format pvr2ccz --sheet $targetPathName.pvr.ccz $filePath/*.png

			for subfile in `ls $filePath`

				do
					subfilePath=$sourceDir"/"$file"/"$subfile
					if [ -d $subfilePath ]
					then 
						#echo $subfile
						targetPathName=$targetDir"/"$subfile
						TexturePacker --data $targetPathName.plist --format cocos2d --disable-rotation --opt RGBA8888 --premultiply-alpha --texture-format pvr2ccz --sheet $targetPathName.pvr.ccz $subfilePath
					fi
				done
		fi
done

cp -r scmls/sgbossbattlelayer/* pvrcczPackage/sgbossbattlelayer/
cp -r scmls/battle/* pvrcczPackage/battle/
cp -r scmls/animationFile/* pvrcczPackage/animationFile/
#cp -r scmls/sanguo.zip pvrcczPackage/sanguobigpic/
cp -r scmls/erwer.plist pvrcczPackage/particle
cp -r scmls/yinghua.plist pvrcczPackage/particle
mv  pvrcczPackage/particle/hua.plist pvrcczPackage
mv  pvrcczPackage/particle/hua.pvr.ccz pvrcczPackage
cp  -r pvrcczPackage/*  $IPAD_HOME
cp -r Fragments/effects/*.scml $IPAD_HOME/effects/
rm -rf pvrcczPackage




		
