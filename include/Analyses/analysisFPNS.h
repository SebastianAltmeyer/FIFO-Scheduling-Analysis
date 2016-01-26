#ifndef analysisFPNS_H
#define analysisFPNS_H

#include "types.h"
#include "taskset.h"
#include "analysis.h"

class AnalysisFPNS: public Analysis  {
	public:
		AnalysisFPNS();
		~AnalysisFPNS();
		int analyse(Taskset &taskset);
		
		bool compute_R (int id, int blockingTask, longint_t& R);
		longint_t compute_B (int id, int& blockingTask);
		longint_t get_B (int blockingTask);	
		int compute_l (int id, longint_t L);
		longint_t compute_L (int id, int blockingTask);
		longint_t compute_s (int id, int k, int blockingTask);
		longint_t compute_f (int id, int k, longint_t s, int blockingTask);
};

#endif