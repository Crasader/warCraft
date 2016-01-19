#!/bin/bash
rm -rf Fragments
rm -rf pkmPackage
cp -r $CLIENT_HOME/Tools/Fragments .

sourceDir="Fragments"

# Get base dir name of sourceDir
sourceBaseDirName=`dirname $sourceDir`
sourceBaseName=`basename $sourceDir`


# Target dir name
targetRootDir=$sourceBaseDirName
targetBaseName="pkmPackage"


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
			TexturePacker --data $targetPathName.plist --format cocos2d --disable-rotation  --premultiply-alpha --opt RGBA8888  --size-constraints AnySize --force-squared --quiet --sheet $targetPathName.png $filePath/*.png
			etcpack $targetPathName.png $targetDir -c etc1 -f RGB -as -ext PNG
			rm -rf $targetPathName.png
			for subfile in `ls $filePath`

				do
					subfilePath=$sourceDir"/"$file"/"$subfile
					if [ -d $subfilePath ]
					then 
						#echo $subfile
						targetPathName=$targetDir"/"$subfile
						TexturePacker --data $targetPathName.plist --format cocos2d --disable-rotation  --premultiply-alpha --opt RGBA8888  --size-constraints AnySize --force-squared --quiet --sheet $targetPathName.png $subfilePath
						etcpack $targetPathName.png $targetDir -c etc1 -f RGB -as -ext PNG
						rm -rf $targetPathName.png
					fi
				done
		fi
done

#scmls
cp -r $CLIENT_HOME/Tools/scmls/sgbossbattlelayer/* pkmPackage/sgbossbattlelayer/
cp -r $CLIENT_HOME/Tools/scmls/battle/* pkmPackage/battle/
cp -r $CLIENT_HOME/Tools/scmls/animationFile/* pkmPackage/animationFile/

#cp -r $CLIENT_HOME/Tools/scmls/sanguo.zip pkmPackage/sanguobigpic/

cp -r $CLIENT_HOME/Tools/scmls/erwer.plist pkmPackage/particle
cp -r $CLIENT_HOME/Tools/scmls/yinghua.plist pkmPackage/particle

mv  pkmPackage/particle/hua.plist pkmPackage
mv  pkmPackage/particle/hua.pkm pkmPackage
mv  pkmPackage/particle/hua_alpha.pkm pkmPackage

cp  -r pkmPackage/*  $ADR_HOME/assets/ipad/


		
