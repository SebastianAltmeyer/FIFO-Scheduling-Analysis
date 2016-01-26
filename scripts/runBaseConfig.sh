#!/bin/sh

DEFAULTRESULT="../output/resultBase"

DEFAULTNROFTASKS="5"
DEFAULTTASKSETS="100"

echo "Compute Results for base configuration"

for PERIODTYPE in "random" "looselyHarmoic" "harmonic" 
do

	../bin/rtAnalyser -def config -taskSets $DEFAULTTASKSETS -nrOfTasks $DEFAULTNROFTASKS -periodType $PERIODTYPE -opt 1000 -oom 2 -o ../results/resBase_$PERIODTYPE

done

echo "Done"
