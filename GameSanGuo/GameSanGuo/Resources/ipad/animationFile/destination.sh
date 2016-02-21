#!/bin/bash


for file in `ls`;
do
	for subfile in `ls $file`
		do
			subfilePath=$file"/"$subfile
			mv $subfilePath/*.png $file"/"
			rm -rf $subfilePath
		done 
		
done;
