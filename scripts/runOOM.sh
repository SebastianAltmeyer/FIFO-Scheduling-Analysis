#!/bin/sh

DEFAULTRESULT="../output/resultBase"

DEFAULTNROFTASKS="5"
DEFAULTTASKSETS="100"

echo "Compute Results for base configuration"

for PERIODTYPE in "random" "looselyHarmoic" "harmonic" 
do
	WENAME=../results/resBase_$PERIODTYPE"_oom_we"

	echo ""  > $WENAME 
	
	for OOM in 2 4 6 8 10 12 16 20
	do
		../bin/rtAnalyser -def config -taskSets $DEFAULTTASKSETS -periodType $PERIODTYPE -opt 1000 -oom $OOM -o ../tmp/resBase_$PERIODTYPE"_"$OOM 

		FILENAME=../tmp/resBase_$PERIODTYPE"_"$OOM"_we"
		TEMPFile=../tmp/tmp
		
		echo $OOM  > $TEMPFile 
		paste $TEMPFile $FILENAME >> $WENAME
		rm $TEMPFile
	done
done

echo "Done"
