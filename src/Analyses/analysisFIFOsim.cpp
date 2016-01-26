#include "Analyses/analysisFIFOsim.h"

AnalysisFIFOsim::AnalysisFIFOsim() {_schedulable = true;}

AnalysisFIFOsim::~AnalysisFIFOsim() {}

int AnalysisFIFOsim::analyse (Taskset &taskset) {

	_rt = new longint_t[taskset.getSize()];
	_taskSet = &taskset;
	_schedulable = true;
	
	for (int id = 0; id < taskset.getSize() && _schedulable; id++) {
	
		checkSchedulability(id);
	
		if (VERBOSE > 2) {
			std::cout << "  Response Time R_" << id << " =  " << _rt[id]  << ". D_" << id << " = " << taskset.getDeadline(id) << "." << std::endl;
		}
	}
  
	return 0;
}

int AnalysisFIFOsim::checkSchedulability(int index) {
	
	longint_t execDemand = 0;
	
	for (int id = 0; id < _taskSet->getSize() && _schedulable; id++) {
		execDemand += _taskSet->getRuntime(id);
	}
	
	if (execDemand > _taskSet->getDeadline(index)) {
		_schedulable = false;
	}

	_rt[index] = execDemand;
	_taskSet->setResponseTime(index,_rt[index]);
  
	return 0;
}

