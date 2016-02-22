#!/bin/sh

DEFAULTTASKSETS="1000"

# for PERIODTYPE in "random" "lHarmonic" "harmonic" 
# do
# 	WENAME=../results/res_$PERIODTYPE"_tasks_we"
# 
# 	echo ""  > $WENAME 
# 	
# 	for TASKS in 2 4 6 8 10 12 16 20
# 	do
# 		../bin/rtAnalyser -def config -taskSets $DEFAULTTASKSETS -nrOfTasks $TASKS -periodType $PERIODTYPE -opt 1000  -o ../tmp/res_$PERIODTYPE"_"$TASKS 
# 
# 		FILENAME=../tmp/res_$PERIODTYPE"_"$TASKS"_we"
# 		TEMPFile=../tmp/tmp
# 		
# 		echo $TASKS  > $TEMPFile 
# 		paste $TEMPFile $FILENAME >> $WENAME
# 		rm $TEMPFile
# 	done
# done

for PERIODTYPE in "harmonic" 
do
	WENAME=../results/res_$PERIODTYPE"_tasks_we"

	echo ""  > $WENAME 
	
	for TASKS in 2 4 6 8 10 12 16 20
	do
		../bin/rtAnalyser -def config -taskSets $DEFAULTTASKSETS -nrOfTasks $TASKS -periodType $PERIODTYPE -opt 1000  -o ../tmp/res_$PERIODTYPE"_"$TASKS -oom 6

		FILENAME=../tmp/res_$PERIODTYPE"_"$TASKS"_we"
		TEMPFile=../tmp/tmp
		
		echo $TASKS  > $TEMPFile 
		paste $TEMPFile $FILENAME >> $WENAME
		rm $TEMPFile
	done
done

echo "Done"
