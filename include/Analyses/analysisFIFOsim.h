#ifndef analysisFIFOsim_H
#define analysisFIFOsim_H

#include "types.h"
#include "taskset.h"
#include "analysis.h"

class AnalysisFIFOsim: public Analysis  {
	public:
		AnalysisFIFOsim();
		~AnalysisFIFOsim();
		int analyse(Taskset &taskset);
	private:
		int checkSchedulability(int id);
};

#endif