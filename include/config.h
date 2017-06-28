#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <map>

#include "types.h"
#include "taskset.h"

class Config
{

  public:

	Config();

	int readConfig(std::string filename);
	
	int checkConfig();

	int parseCommandLine(int argc, char **argv);

	int parseConfig(int start, int nr, char **argv);

	~Config();

	int printHelp();

	void UUnifast(int n,float Ut, float *utilVec);
	void swap(int* value, int i1, int i2);
	void SortTasks(int* prior, longint_t* value, int len);
	
	int genTaskSet(Taskset &ts, std::string tsName, float util) ;
	int fillTaskSet(Taskset &ts) ;
	int loadTaskSet(Taskset &ts) ;
	
	bool simulation();
	bool example();
        bool singleTS();
	bool evalEventOrder();
	
	float getUtilStep();
	float getUtilStart();
	float getUtilEnd();
	std::string getNamePrefix();
	std::string getOutFile();

	std::string getOptType();
	std::string getOptValue();
	
	int getNrOfSteps();
	
	int getNrOfTaskSets();
	int getNrOfTasks();
	int getSeed();
	int getOOF();
	
	int getSimulationRuns();
	float getRTFactor();
	int getSimulLength();
        
        int getNonPeriodicTasks();
        int getNonHarmonicTasks();
	
  private:

	std::string _configName;
	std::string _fileName;
	std::string _tasksetFileName;
	std::string _namePrefix;
	std::string _optType;
	std::string  _optValue;
	std::string  _periodType;
	
	std::string _outFile;
        
        int _nonHarmonicTasks;
        int _nonPeriodicTasks;
	
	float _utilStart;
	float _utilEnd;
	float _utilSteps;
	int _nrOfTasks;
	int _nrOfTaskSets;
	int _optSteps;
	int _simulationRuns;
	
	int _verbose;
		
	bool _printHelp;
	bool _genTS;
	bool _singleTS;
	bool _weighted;
	bool _simulation;
	bool _example;
	bool _eventOrder;
	
	int _randSeed;
	
	float _deadlineFac;
	float _rtFactor;
	int _simulationLength;
	
	int _scale;
	int _oof;
	int _oom;
	
};

#endif