To use FIFO-Scheduling-Analysis, you must first compile the all the files in this folder. This is done using

>> make

Since FIFO-Scheduling-Analysis is written in C++, you need to have a C++ compiler installed on
your computer.

To run the scripts one by one , use the following commands one by one :

>>./runBaseConfig.sh
>>./runEvalEventOrder.sh
>>./runOOF.sh
>>./runOOM.sh
>>./runTasks.sh

If the number of tasksets are 1000 in script files, then execution of these script files may take longer time. If you want results immediatley, set number of tasksets as less value.

Running of these scripts will create the results folder.

To plot graphs use following commands one by one :

Check your system installed with gnu plot.

>>gnuplot plot_baseHarmonic.pt
>>gnuplot plot_baseLHarmonic.pt
>>gnuplot plot_baseRandom.pt
>>gnuplot plot_EventOrders.pt
>>gnuplot plot_nrOfTasks.pt
>>gnuplot plot_OOF.pt
>>gnuplot plot_OOM.pt
