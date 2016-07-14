#ifndef TYPES_H
#define TYPES_H

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <stdlib.h>
#include <time.h>
#include <assert.h> 

#define VERSION 0.1



extern std::string INPUTDIR;
extern std::string OUTPUTDIR;
extern std::string TMPDIR;
extern std::string TASKDIR;
extern int VERBOSE;

typedef unsigned int uint_t;
typedef long longint_t;

struct Statistics {
	int FPPS;
	int FIFO_OFFSETS_OPT;
	int FIFO_OFFSETS;
	int FIFO_NO;
	int FIFO_SIM;
	int FPNS;
};

// add performance stat struct

#endif
