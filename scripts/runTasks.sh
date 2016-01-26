#!/bin/sh

DEFAULTRESULT="../output/resultBase"

DEFAULTNROFTASKS="5"
DEFAULTTASKSETS="100"

echo "Compute Results for base configuration"

for PERIODTYPE in "random" "looselyHarmoic" "harmonic" 
do
	WENAME=../results/resBase_$PERIODTYPE"_tasks_we"

	echo ""  > $WENAME 
	
	for TASKS in 2 4 6 8 10 12 16 20
	do
		../bin/rtAnalyser -def config -taskSets $DEFAULTTASKSETS -nrOfTasks $TASKS -periodType $PERIODTYPE -opt 1000 -oom 2 -o ../tmp/resBase_$PERIODTYPE"_"$TASKS 

		FILENAME=../tmp/resBase_$PERIODTYPE"_"$TASKS"_we"
		TEMPFile=../tmp/tmp
		
		echo $TASKS  > $TEMPFile 
		paste $TEMPFile $FILENAME >> $WENAME
		rm $TEMPFile
	done
done

echo "Done"
