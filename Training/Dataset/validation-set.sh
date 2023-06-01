#!/bin/bash
FILES="full/txt/*.txt"
declare -i count
count = 0
for f in $FILES
do	
	if (( $count % 49 == 0 )) && [ -f "$f" ]
	then
		num=`echo $f | cut -d "." -f 1 | cut -d "/" -f 3`
		mv "full/txt/$num.txt" "validate/txt/$num.txt"
        rm "full/txt/$num.txt"
	fi
	((count++))
done
