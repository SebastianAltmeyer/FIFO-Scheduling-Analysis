#include "Analyses/analysisFPNS.h"

AnalysisFPNS::AnalysisFPNS() {_schedulable = true;}

AnalysisFPNS::~AnalysisFPNS() {}

int AnalysisFPNS::analyse (Taskset &taskset) {
	
	_rt = new longint_t[taskset.getSize()];
	_taskSet = &taskset;
  
	if (_taskSet->getSize() < 0) return false;
		
	for (int i = 0; i < _taskSet->getSize(); i++)
	{
		_rt[i] = 0;
	}
	
	for (int i = 0; i < _taskSet->getSize() && _schedulable; i++)
	{
		int blockingTask = -1;
		
		// compute blocking B
		compute_B(i,blockingTask);		

		// compute response time
		_schedulable = compute_R(i, blockingTask, _rt[i]);
	}
	
	delete[] _rt;
	return 0;
}

bool AnalysisFPNS::compute_R (int i, int blockingTask, longint_t& rt) {
	longint_t L;
	int l;
	longint_t s;
	longint_t f;
		
	bool schedulable = true;
	
	// compute intervall L
	L = compute_L(i,blockingTask);

	// compute number of checks
	l = compute_l(i,L);

	// check for k from 0 to l
	rt = 0;
	for (int k = 0; k < l && schedulable; k++) {
		
		s = compute_s(i,k,blockingTask);
		f = compute_f(i,k,s,blockingTask);
		
		rt = std::max(rt, f-k*_taskSet->getPeriod(i));
		
		if (rt > _taskSet->getDeadline(i)) schedulable = false;
	}
	
	return schedulable;
}

longint_t AnalysisFPNS::compute_L(int id, int blockingTask) {
	longint_t L_Old = 1;
	longint_t L = 1;
	longint_t B = get_B(blockingTask);
		
	bool finished = false;
	while (!finished) {

		L_Old = L;
		L = B;
		for (int j = 0; j <= id ; j++) { // hp(i)
			L += ceil(float(L_Old)  / float (_taskSet->getPeriod(j)))*_taskSet->getRuntime(j);
		}
		if (L == L_Old) finished = true;
	}
	
	return L;
}

int AnalysisFPNS::compute_l(int id, longint_t L) {
	
	int l = ceil(float(L)  / float (_taskSet->getPeriod(id)));
	
	return l;
}

longint_t AnalysisFPNS::compute_B(int id, int& blockingTask) {
	longint_t B = 0;
	for (int j = id+1; j < _taskSet->getSize(); j++) {
		
			if (_taskSet->getRuntime(j) > B) { 
				B = _taskSet->getRuntime(j);
				blockingTask = j;
		}
	}
	
	return B;
}

longint_t AnalysisFPNS::get_B (int blockingTask) {
	longint_t B = 0;
	
	if (blockingTask >= 0)
		B = _taskSet->getRuntime(blockingTask);
	
	return B;
}

longint_t AnalysisFPNS::compute_s(int id, int k, int blockingTask) {
	longint_t B = get_B(blockingTask);	
	longint_t s = B + k*_taskSet->getRuntime(id);
	longint_t s_Old = s;

	bool finished = false;
	while (!finished) {
		s_Old = s;
		s = B + k*_taskSet->getRuntime(id);
					
		for (int j = 0; j < id ; j++) { // hp(i)
			if (B)
				s += ceil(float(s_Old)  / float (_taskSet->getPeriod(j)))*_taskSet->getRuntime(j);
			else 
				s += (1 + floor(float(s_Old)  / float (_taskSet->getPeriod(j))))*_taskSet->getRuntime(j);
		}
		if (s == s_Old) finished = true;
	}
	
	return s;
}


longint_t AnalysisFPNS::compute_f(int id, int k, longint_t s,  int blockingTask) {
// 	longint_t B = get_B(blockingTask);	
	longint_t f = s + _taskSet->getRuntime(id);
	longint_t f_Old = 0;

	bool finished = false;
	while (!finished) {
		f_Old = f;
		f = s + _taskSet->getRuntime(id);

		if (f == f_Old) finished = true;
	}

	return f;
}