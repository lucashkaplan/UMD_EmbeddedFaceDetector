#!/bin/bash

face=0
nface=0
tface=0
fface=0
tnface=0
fnface=0

if [ -f tmp.txt]
then
	rm tmp.txt
fi

paste output.txt imgLabels.txt > tmp.txt

while read k f ka fa;
do
	echo "$fa: $f"

	if [ $fa = "F" ]
	then
		face=$(($face + 1))
		if [ $f = "F" ]
		then
			tface=$(($tface + 1))
		else
			fnface=$(($fnface + 1))
		fi
	else
		nface=$(($nface + 1))
		if [ $f = "F" ]
		then
			fface=$(($fface + 1))
		else
			tnface=$(($tnface + 1))
		fi
	fi

done < tmp.txt

rm tmp.txt

echo "Total With Face: $face"
echo "Total True Face: $tface"
echo "Total False Face: $fface"
echo "Total Without Face: $nface"
echo "Total True No face: $tnface"
echo "Total False No Face: $fnface"
tface=$(($tface*100))
fface=$(($fface*100))
fnface=$(($fnface*100))
echo "TP: $(($tface/$face))"
echo "FP: $(($fface/$face))"
echo "FN: $(($fnface/$nface))"
