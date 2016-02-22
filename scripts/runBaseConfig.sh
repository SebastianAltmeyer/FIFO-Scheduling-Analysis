#!/bin/sh

DEFAULTTASKSETS="10000"

echo "Compute Results for base configuration"

for PERIODTYPE in "lHarmonic" "harmonic" "random" 
do

	../bin/rtAnalyser -def config -taskSets $DEFAULTTASKSETS -periodType $PERIODTYPE -opt 1000  -o ../results/results_$PERIODTYPE -v
done

echo "Done"
