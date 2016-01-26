#ifndef analysisSim_H
#define analysisSim_H

#include <limits> 
#include <queue>
#include "types.h"
#include "taskset.h"
#include "analysis.h"

class AnalysisSim: public Analysis  {
	public:
		AnalysisSim();
		~AnalysisSim();
		int analyse(Taskset &taskset);
		int simulate ();
private:
		struct Job {
			int taskindex;
			int jobindex;
		};
		
		enum EventType {
			RELEASE,
			COMPLETION,
			DEADLINE
		};
		
		struct Event {
			EventType type;
			Job job;
			longint_t time;
			
			bool operator<(const Event& rightE) const {
			  return time > rightE.time;
			  
			}
		};
};

#endif