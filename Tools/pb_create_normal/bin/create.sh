#!/bin/bash

cd ../input
files=`ls -la *.proto |awk '{print $NF}'`
for file in $files;do
sed -i '' '1 a\
option optimize_for = LITE_RUNTIME;' $file
done

rm -f ../output/*.*
protoc ../input/*.proto --proto_path=../input --cpp_out=../output