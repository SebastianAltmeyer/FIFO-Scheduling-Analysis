#ifndef analysisSimFPNS_H
#define analysisSimFPNS_H

#include <limits> 
#include <queue>
#include "types.h"
#include "taskset.h"
#include "analysis.h"

class AnalysisSimFPNS: public Analysis  {
	public:
		AnalysisSimFPNS();
		~AnalysisSimFPNS();
		int analyse(Taskset &taskset);
		int simulate ();
		int simulate (Taskset &taskset, float rtFactor, std::vector<int> &order,longint_t length);
private:
		struct Job {
			int taskindex;
			int jobindex;
			int releaseTime;
			int deadline;
			bool operator<(const Job& rightJ) const {
			  return taskindex > rightJ.taskindex;
			}
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
				if (time == rightE.time) {
					return job.taskindex > rightE.job.taskindex;
				}
			  return time > rightE.time;
			}
		};
};

#endif