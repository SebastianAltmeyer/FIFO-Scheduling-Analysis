#!/bin/sh

DEFAULTTASKSETS="100"

for PERIODTYPE in "random" "lHarmonic" "harmonic" 
do
	WENAME=../results/res_$PERIODTYPE"_gran_we"

	echo ""  > $WENAME 
	
	for GRAN in 1 10 50 100 200 300 400 500 600 700 800 900 1000
	do
		../bin/rtAnalyser -def config -taskSets $DEFAULTTASKSETS -periodType $PERIODTYPE -opt 1000 -gran $GRAN -o ../tmp/res_$PERIODTYPE"_GRAN"$GRAN 

		FILENAME=../tmp/res_$PERIODTYPE"_GRAN"$GRAN"_we"
		TEMPFile=../tmp/tmp
		
		echo $GRAN  > $TEMPFile 
		paste $TEMPFile $FILENAME >> $WENAME
		rm $TEMPFile
	done
done

echo "Done"
