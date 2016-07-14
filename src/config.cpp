#include <config.h>

Config::Config() {
	_fileName = "";
	_configName = "empty";
	_optType = "rand";
	_optValue = "3";
	
	_verbose = 0;
	_printHelp = false;
	_weighted = false;
	_tasksetFileName = "";
	_periodType  = "";

	_utilSteps = 0.025;
	_utilStart = 0.05;
	_utilEnd = 1.0;

	_nrOfTasks = 0;
	_nrOfTaskSets = 0;
	_deadlineFac = 1.0;

	_randSeed = 42;
	_scale = 1000;
	_oof = 10;
	_oom = 2;
	_simulation = false;
	_example = false;
	_eventOrder = false;
	
	_optSteps = 0;
	_outFile = "result";
	_simulationRuns = 100;
	_simulationLength = -1;
	_rtFactor = 0.3;
        _nonHarmonicTasks = 0;
        _nonPeriodicTasks = 0;
}

int Config::readConfig(std::string filename) {
	
	std::ifstream fin;
	std::string input;
	fin.open(filename.c_str());
	
	if (!fin) {
		std::cerr << "Error: Could not open config file \'" <<  filename << "\'" << std::endl;
		return 1;
	}
	
	int counter = 0;
	char** charArr = new char*[100];
	char string[200];

	while (fin >> string && counter < 100) {
		charArr[counter] = new char[200];
		strncpy(charArr[counter], string, 200);
		counter++;
	}
	
	parseConfig(0,counter, charArr);

	for (int i = 0; i < counter; i++) {
		delete[] (charArr[i]);
	}
	
	delete[] charArr;
	
	fin.close();
	return 0;
}

int Config::parseCommandLine(int argc, char **argv) {
	if (argc < 2) {
		std::cerr << "Not enough command line provided." << std::endl;
		return 1;
	} else {
		return parseConfig(1,argc, argv);
	}
}

int Config::parseConfig(int start, int nr, char **argv) {
	
	for (int i = start; i < nr; i++) {
		if (!strcmp(argv[i], "-def")) {
			if (i + 1 < nr) {
				if (readConfig(argv[i+1]))
					return 1;
				i++;
			} else {
				std::cerr << "No config file given." << std::endl;
			}
		}	else if (!strcmp(argv[i], "-taskSets")) {
			if (i + 1 < nr) {
				_nrOfTaskSets = atoi(argv[i+1]);
				i++;
			} else {
				std::cerr << "No number of task sets provided." << std::endl;
			}
		} else if (!strcmp(argv[i], "-opt")) {
			if (i + 1 < nr) {
				_optSteps = atoi(argv[i+1]);
				i++;
			} else {
				std::cerr << "No number of optimization steps provided." << std::endl;
			}
		} else if (!strcmp(argv[i], "-nrOfTasks")) {
			if (i + 1 < nr) {
				_nrOfTasks = atoi(argv[i+1]);
				i++;
			} else {
				std::cerr << "No number of tasks provided." << std::endl;
			}
		} else if (!strcmp(argv[i], "-oom")) {
			if (i + 1 < nr) {
				_oom = atoi(argv[i+1]);
				i++;
			} else {
				std::cerr << "No period range provided." << std::endl;
			}
		} else if (!strcmp(argv[i], "-oof")) {
			if (i + 1 < nr) {
				_oof = atoi(argv[i+1]);
				i++;
			} else {
				std::cerr << "No period factor provided." << std::endl;
			}
		} else if (!strcmp(argv[i], "-gran")) {
			if (i + 1 < nr) {
				_oof = atoi(argv[i+1]);
				i++;
			} else {
				std::cerr << "No granularity provided." << std::endl;
			}
		}	else if (!strcmp(argv[i], "-example")) {
			_example = true;
		}	else if (!strcmp(argv[i], "-v")) {
			VERBOSE = 1;
		}	else if (!strcmp(argv[i], "-vv")) {
			VERBOSE = 2;
		}	else if (!strcmp(argv[i], "-vvv")) {
			VERBOSE = 3;
		}	else if (!strcmp(argv[i], "-vvvv")) {
			VERBOSE = 4;
		}	else if (!strcmp(argv[i], "-silent")) {
			VERBOSE = -1;
		}	else if (!strcmp(argv[i], "-simulation")) {
			_simulation = true;
		}	else if (!strcmp(argv[i], "-evalEventOrder")) {
			_eventOrder = true;
			if (i + 1 < nr) {
				_simulationRuns = atoi(argv[i+1]);
				i++;
			} else {
				std::cerr << "No number of simulation runs provided." << std::endl;
			}
			if (i + 1 < nr) {
				_simulationLength = atoi(argv[i+1]);
				i++;
			} else {
				std::cerr << "No simulation length provided." << std::endl;
			}
			if (i + 1 < nr) {
				_rtFactor = atof(argv[i+1]);
				i++;
			} else {
				std::cerr << "No runtime factor provided." << std::endl;
			}
		}	else if (!strcmp(argv[i], "-seed")) {
			if (i + 1 < nr) {
				_randSeed = atoi(argv[i+1]);
				i++;
			} else {
				std::cerr << "No number of random seed provided." << std::endl;
			}
		}	else if (!strcmp(argv[i], "-deadlineFactor")) {
			if (i + 1 < nr) {
				_deadlineFac = atof(argv[i+1]);
				i++;
			} else {
				std::cerr << "No end deadline factor provided." << std::endl;
			}
		}	else if (!strcmp(argv[i], "-util")) {
			if (i + 1 < nr) {
				_utilStart = atof(argv[i+1]);
				_utilEnd = atof(argv[i+1]);
				_genTS = true;
				i++;
			} else {
				std::cerr << "No utilization provided." << std::endl;
			}
		}	else if (!strcmp(argv[i], "-utilEnd")) {
			if (i + 1 < nr) {
				_utilEnd = atof(argv[i+1]);
				_genTS = true;
				i++;
			} else {
				std::cerr << "No end utilization provided." << std::endl;
			}
		}	else if (!strcmp(argv[i], "-utilStart")) {
			if (i + 1 < nr) {
				_utilStart = atof(argv[i+1]);
				_genTS = true;
				i++;
			} else {
				std::cerr << "No start utilization provided." << std::endl;
			}
		}	else if (!strcmp(argv[i], "-utilSteps")) {
			if (i + 1 < nr) {
				_utilSteps = atof(argv[i+1]);
				_genTS = true;
				i++;
			} else {
				std::cerr << "No utilization steps provided." << std::endl;
			}
		}	else if (!strcmp(argv[i], "-ts")) {
			if (i + 1 < nr) {
				_tasksetFileName = argv[i+1];
				_singleTS = true;
				i++;
			} else {
				std::cerr << "No taskset file given." << std::endl;
			}
		}	else if (!strcmp(argv[i], "-periodType")) {
			if (i + 1 < nr) {
				_periodType= argv[i+1];
				i++;
			} else {
				std::cerr << "No period definition type given." << std::endl;
			}
		}	else if (!strcmp(argv[i], "-optValue")) {
			if (i + 1 < nr) {
				_optValue = argv[i+1];
				i++;
			} else {
				std::cerr << "No optim.-value given." << std::endl;
			}
		}	else if (!strcmp(argv[i], "-optType")) {
			if (i + 1 < nr) {
				_optType = argv[i+1];
				i++;
			} else {
				std::cerr << "No optim.-Type given." << std::endl;
			}			
		} else if (!strcmp(argv[i], "-o")) {
			if (i + 1 < nr) {
				_outFile = argv[i+1];
				i++;
			} else {
				std::cerr << "No output file given." << std::endl;
			}
                } else if (!strcmp(argv[i], "-nH")) {
			if (i + 1 < nr) {
				_nonHarmonicTasks = atoi(argv[i+1]);;
				i++;
			} else {
				std::cerr << "No number of non-harmonic tasks file given." << std::endl;
			}
                } else if (!strcmp(argv[i], "-nP")) {
			if (i + 1 < nr) {
				_nonPeriodicTasks = atoi(argv[i+1]);;
				i++;
			} else {
				std::cerr << "No number of non-periodic tasks file given." << std::endl;
			}
		} else if (!strcmp(argv[i], "--help")) {
			_printHelp = true;
		} else {
			std::cerr << "Unknown command-line parameter \""<< argv[i] << "\"" << std::endl;
// 			return 1;
		}
	}

	srand(_randSeed);
	
	return 0;
}

int Config::checkConfig() {
	
	return 0;
}

Config::~Config() {
}

bool Config::simulation() {
	return _simulation;
}

bool Config::example() {
	return _example;
}

int Config::printHelp() {

	if (!_printHelp)
		return 0;
	
	
	std::string filename = "doc/help.txt";
	std::string input;
	
	std::ifstream fin;

	
	fin.open(filename.c_str());
	
	if (!fin) {
		std::cerr << "Error: Could not open help file \'" <<  filename << "\'" << std::endl;
		return 1;
	}

	while (std::getline(fin, input)) {
		std::cout << input << std::endl;
	}

	fin.close();

	return 1;
}

int Config::fillTaskSet(Taskset &ts) {

	int size = 3;
	std::string tsName = "Example";
	std::cout << " fillTaskSet example" << std::endl;

	int* priorityFinal = new int[size];
	longint_t* deadlineFinal = new longint_t[size];
	longint_t* periodFinal = new longint_t[size];
	longint_t* execTimesFinal = new longint_t[size];
	longint_t* offsetFinal = new longint_t[size];
        bool* sporadicFinal = new bool[size];

	priorityFinal[0] = 0;
	deadlineFinal[0] = 4;
	periodFinal[0] = 4;
	execTimesFinal[0] = 1;
	offsetFinal[0] = 0;
        sporadicFinal[0] = false;
	
	priorityFinal[1] = 1;
	deadlineFinal[1] = 4;
	periodFinal[1] = 4;
	execTimesFinal[1] = 2;
	offsetFinal[1] = 2;
        sporadicFinal[1] = false;
	
	priorityFinal[2] = 2;
	deadlineFinal[2] = 9;
	periodFinal[2] = 20;
	execTimesFinal[2] = 4;
	offsetFinal[2] = 2;
        sporadicFinal[2] = false;

	ts.init(tsName, size, priorityFinal, deadlineFinal, periodFinal, execTimesFinal, offsetFinal,sporadicFinal);

	return 0;
}

int Config::genTaskSet(Taskset &ts, std::string tsName, float util) {

	int size = _nrOfTasks;

	int priorities[size];
	longint_t execTimes[size];
	longint_t deadlines[size];
	longint_t periods[size];

// 	 std::cout << " genTaskSet: util " << util << std::endl;

	// compute Utilizations
	float utilVec[size];
	UUnifast(size, util, utilVec);

        std::sort(utilVec,utilVec+size);
        
        int iStart = 0;
        for (; iStart < _nonHarmonicTasks; iStart++) {
                periods[iStart] = _scale*pow(2,(rand()/(float)RAND_MAX)*_oom);
		periods[iStart] = (periods[iStart]/_oof)*_oof;
        }
        
	if (_periodType == "lHarmonic") {
		periods[iStart] = _scale*pow(10,(rand()/(float)RAND_MAX));
		periods[iStart] = (periods[iStart]/_oof)*_oof;
		for (int i = iStart ; i < size; i++) {
			periods[i] = periods[iStart]*_oom;
		}
	} else if (_periodType == "harmonic") {
		periods[iStart] = _scale*pow(10,(rand()/(float)RAND_MAX));
		periods[iStart] = (periods[iStart]/_oof)*_oof;
		for (int i = iStart ; i < size; i++) {
			periods[i] = periods[iStart]* pow(2,(rand() % _oom));
		}
	} else {
		for (int i = iStart ; i < size; i++) {
			periods[i] = _scale*pow(2,(rand()/(float)RAND_MAX)*_oom);
			periods[i] = (periods[i]/_oof)*_oof;
		}
	}
	
	
	for (int i = 0 ; i < size; i++) {
		priorities[i] = i;
		execTimes[i] = periods[i]*utilVec[i];
		deadlines[i] = _deadlineFac*periods[i];

		if (VERBOSE>3)
		std::cout << " T_" << i << ":"
					<< " U = " << utilVec[i] 
					<< " C = " << execTimes[i]
					<< " D = " << deadlines[i]
					<< " oof = " << _oof 
					<< " T = " << periods[i] << std::endl;
	}

	int* priorityFinal = new int[size];
	longint_t* deadlineFinal = new longint_t[size];
	longint_t* periodFinal = new longint_t[size];
	longint_t* execTimesFinal = new longint_t[size];
	longint_t* offsetFinal = new longint_t[size];
        bool* sporadicFinal = new bool[size];

	// order tasks in deadline monotonic order
	SortTasks(priorities, deadlines, size);

	for (int i = 0; i < size; i++)
	{
		priorityFinal[i] = i;
		deadlineFinal[i] = deadlines[priorities[i]];
		periodFinal[i] = periods[priorities[i]];
		execTimesFinal[i] = execTimes[priorities[i]];
		offsetFinal[i] = ((rand() % periodFinal[i])/_oof)*_oof;
                sporadicFinal[i] = false;
	}

	
        for (int i = 0; i < _nonPeriodicTasks ; i++)
	{
		sporadicFinal[i] = true;
	}
	
	ts.init(tsName, size, priorityFinal, deadlineFinal, periodFinal, execTimesFinal, offsetFinal, sporadicFinal);

	return 0;
}

void Config::UUnifast(int n,float Ut, float *utilVec) {
	float SumU = Ut;
	float nextSumU;
	
	for (int i = 1; i < n; i++)
	{
		float random = rand()/(float)RAND_MAX;
		
		nextSumU = SumU * pow(random, 1/(float)(n-i));
		utilVec[i-1] = SumU - nextSumU;

		SumU = nextSumU;
	}
	utilVec[n-1] = SumU;

	return;
}

void Config::swap(int* value, int i1, int i2) {
	int temp = value[i1];
	value[i1] = value[i2];
	value[i2] = temp;
}

void Config::SortTasks(int prior[], longint_t value[], int len) {
    for (int pass=0; pass<len-1; pass++) {
        int indexSm = pass;  // assume this is smallest

        //--- Look over remaining elements to find smallest.
        for (int i=pass+1; i<len; i++) {
            if (value[prior[i]] < value[prior[indexSm]]) {
                //--- Remember index for latter swap.
                indexSm = i;
            }
        }
        
        //--- Swap smallest remaining element
        int temp = prior[pass];
        prior[pass] = prior[indexSm];
        prior[indexSm] = temp;
    }
}


float Config::getUtilStep() {
	return _utilSteps;
}

float Config::getUtilStart() {
	return _utilStart;
}

float Config::getUtilEnd() {
	return _utilEnd;
}

int Config::getNrOfTaskSets() {
	return _nrOfTaskSets;
}

std::string Config::getOutFile() {
	return _outFile;
}

int Config::getSeed() {
	return _randSeed;
}

int Config::getNrOfSteps() {
	return _optSteps;
}

std::string Config::getOptType() {
		return _optType;
}

std::string Config::getOptValue() {
	return _optValue;
}

int Config::getOOF() {
	return _oof;
}

int Config::getSimulationRuns() {
	return _simulationRuns;
}

float Config::getRTFactor() {
	return _rtFactor;
}

int Config::getSimulLength() {
	return _simulationLength;
}

bool Config::evalEventOrder() {
	return _eventOrder;
}

int Config::getNonPeriodicTasks() {
        return _nonPeriodicTasks;
}

int Config::getNonHarmonicTasks() {
        return _nonHarmonicTasks;
}