#!/bin/bash
#usage: ./gen_imgs_txt.sh /Volumes/Transcend/KCFcpp/Dudek/img

cd $1
path=$1
dir=${path##*/}
echo $path
for i in `ls`
do
	ext=${i##*.}
	if [ $ext=="jpg" ];then
		echo $path/$i >>/Volumes/Transcend/KCFcpp/Dudek/images.txt
	fi
done
