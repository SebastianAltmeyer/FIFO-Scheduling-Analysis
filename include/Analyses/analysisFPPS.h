#ifndef analysisFPPS_H
#define analysisFPPS_H

#include "types.h"
#include "taskset.h"
#include "analysis.h"

class AnalysisFPPS: public Analysis  {
	public:
		AnalysisFPPS();
		~AnalysisFPPS();
		int analyse(Taskset &taskset);
};

#endif