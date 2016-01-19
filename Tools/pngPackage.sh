#!/bin/bash



sourceDir=$1

if [ "$sourceDir" == "" ];then 
	echo "Usage: convert.sh sourceDir"
	echo -e "where sourceDir if directionary that you want convert. \
	\nThis tool will convert all png files in sub floder to one png sheet named by directionary name."
	exit 1
fi

# Get base dir name of sourceDir
sourceBaseDirName=`dirname $sourceDir`
sourceBaseName=`basename $sourceDir`


# Target dir name
targetRootDir=$sourceBaseDirName
targetBaseName=$sourceBaseName-new


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
			TexturePacker --data $targetPathName.plist --format cocos2d --disable-rotation --sheet $targetPathName.png $filePath/*.png

			for subfile in `ls $filePath`

				do
					subfilePath=$sourceDir"/"$file"/"$subfile
					if [ -d $subfilePath ]
					then 
						#echo $subfile
						targetPathName=$targetDir"/"$subfile
						TexturePacker --data $targetPathName.plist --format cocos2d --disable-rotation --sheet $targetPathName.png $subfilePath
					fi
				done
		fi
done
		
