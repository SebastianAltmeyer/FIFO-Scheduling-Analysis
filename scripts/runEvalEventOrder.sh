#!/bin/sh

DEFAULTTASKSETS="100"

echo "Compute Results for base configuration"

for PERIODTYPE in "lHarmonic" "harmonic"
do

	../bin/rtAnalyser -def config -taskSets $DEFAULTTASKSETS -periodType $PERIODTYPE -opt 1000  -o ../results/resultsEO_$PERIODTYPE -v -evalEventOrder 1000 0 0.5 -utilSteps 0.1
done

echo "Done"
# run -def config -evalEventOrder 10 10000 0.4
