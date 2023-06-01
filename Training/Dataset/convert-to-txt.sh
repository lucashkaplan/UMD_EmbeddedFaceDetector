#!/bin/bash
FILES="png/good/*.png"
for f in $FILES
do
	if [ -f "$f" ]
	then
		num=`echo $f | cut -d "." -f 1 | cut -d "/" -f 3`
		if [ -f "txt/good/$num.txt" ]
		then
			rm "txt/good/$num.txt"
		fi
		enee408mim2txt $f
		mv "png/good/$num.txt" "txt/good/$num.txt"
	fi
done

FILES="png/bad/*.png"
for f in $FILES
do
    if [ -f "$f" ]
    then
        num=`echo $f | cut -d "." -f 1 | cut -d "/" -f 3`
		if [ -f "txt/bad/$num.txt" ]
        then
            rm "txt/bad/$num.txt"
        fi
        enee408mim2txt $f
        mv "png/bad/$num.txt" "txt/bad/$num.txt"
    fi
done

