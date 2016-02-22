#!/bin/sh

DEFAULTTASKSETS="1000"

for PERIODTYPE in "random" "lHarmonic" "harmonic" 
do
	WENAME=../results/res_$PERIODTYPE"_oom_we"

	echo ""  > $WENAME 
	
	for OOM in 1 2 3 4 5
	do
		../bin/rtAnalyser -def config -taskSets $DEFAULTTASKSETS -periodType $PERIODTYPE -opt 1000 -oom $OOM -o ../tmp/res_$PERIODTYPE"_OOM"$OOM 

		FILENAME=../tmp/res_$PERIODTYPE"_OOM"$OOM"_we"
		TEMPFile=../tmp/tmp
		
		echo $OOM  > $TEMPFile 
		paste $TEMPFile $FILENAME >> $WENAME
		rm $TEMPFile
	done
done

echo "Done"
