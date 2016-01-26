#include "Analyses/analysisFPPS.h"

AnalysisFPPS::AnalysisFPPS() {_schedulable = true;}

AnalysisFPPS::~AnalysisFPPS() {}

int AnalysisFPPS::analyse (Taskset &taskset) {

	for (int id = 0; id < taskset.getSize() && _schedulable; id++) {
	
		longint_t rt = taskset.getRuntime(id);
		longint_t rtOld = 0;

		bool schedulable = true;
		bool finished = false;
		
		while (!finished && schedulable) {
			
			rtOld = rt;
			rt = taskset.getRuntime(id);
			
			for (int j = 0; j < id ; j++) { // hp(i)
				rt += ceil(float(rtOld)  / float (taskset.getPeriod(j)))*taskset.getRuntime(j);
			}	
			
			if (rt > taskset.getDeadline(id)) schedulable = false;
			if (rt == rtOld) finished = true;
		}
	
		_schedulable = _schedulable && schedulable;
	
		taskset.setResponseTime(id,rt);
	
		if (VERBOSE > 2) {
			std::cout << "  Response Time R_" << id << " =  " << rt  << ". D_" << id << " = " << taskset.getDeadline(id) << "." << std::endl;
		}
	}
  
	return 0;
}



