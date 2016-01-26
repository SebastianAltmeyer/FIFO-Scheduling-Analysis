#include "Analyses/analysisSim.h"

AnalysisSim::AnalysisSim() {_schedulable = true;}

AnalysisSim::~AnalysisSim() {}

int AnalysisSim::analyse (Taskset &taskset) {
	
	_rt = new longint_t[taskset.getSize()];
	_taskSet = &taskset;
  
	
	simulate();
	
	delete[] _rt;
	return 0;
}


int AnalysisSim::simulate () {

	longint_t hyperPeriod;
	if (!_taskSet->computeHyperperiod()) { 
		hyperPeriod= _taskSet->getHyperperiod();
		if (VERBOSE > 2) { std::cout << "  Hyperperiod =  " << hyperPeriod << " (below bound of " << std::numeric_limits<longint_t>::max() <<  ")." << std::endl;}
	} else  {
		hyperPeriod= std::numeric_limits<longint_t>::max()/2;
		if (VERBOSE > 2) { std::cout << "  Hyperperiod exceeds " << std::numeric_limits<longint_t>::max() << " set to " << hyperPeriod << "." << std::endl;}
	} 
		
	longint_t currentTime = 0;
	
	_schedulable = true;
	
	std::queue<Job> schedulerQueue;
	std::priority_queue<Event> eventQueue;
	
	for (int i = 0; i < _taskSet->getSize(); i++) {
		Event newEvent;
		Job newJob;
		newJob.taskindex = i;
		newJob.jobindex = 0;
		newEvent.time = _taskSet->getOffset(i);
		newEvent.type = RELEASE;
		newEvent.job = newJob;
		eventQueue.push(newEvent);
	}
	
	Job currentlyExecuting;
	currentlyExecuting.taskindex = -1;
	currentlyExecuting.jobindex = -1;
	
	while (currentTime < 2*hyperPeriod && _schedulable) {

		Event currentEvent = eventQueue.top();
		Job currentJob = currentEvent.job;
		eventQueue.pop();
		// forward time
		currentTime = currentEvent.time;

		longint_t deadline = _taskSet->getOffset(currentJob.taskindex) + (currentJob.jobindex)*_taskSet->getPeriod(currentJob.taskindex) + _taskSet->getDeadline(currentJob.taskindex);
		
		if (VERBOSE > 3) {std::cout << "    Simulation at " << currentEvent.time << ": ";}
		switch (currentEvent.type) {
			case DEADLINE:
				if (VERBOSE > 3) {std::cout << "deadline";}
				break;
			case RELEASE:
				if (VERBOSE > 3) {std::cout << "release";}
				schedulerQueue.push(currentJob);
				
				Event newEventRelease;
				Job newJob;
				newJob.taskindex = currentJob.taskindex;
				newJob.jobindex = currentJob.jobindex+1;
				newEventRelease.time = _taskSet->getOffset(newJob.taskindex) + (newJob.jobindex)*_taskSet->getPeriod(newJob.taskindex);
				newEventRelease.type = RELEASE;
				newEventRelease.job = newJob;
				eventQueue.push(newEventRelease);
				break;
			case COMPLETION:
				if (VERBOSE > 3) {std::cout << "completion";}
				if (deadline < currentTime) {
					_schedulable = false;
				}
				currentlyExecuting.taskindex = -1; 
				currentlyExecuting.jobindex = -1; 
				break;
			default:
				break;
		}
		
		if (currentlyExecuting.taskindex == -1 && !schedulerQueue.empty()) {
			currentlyExecuting = schedulerQueue.front();
			Event newEventCompletion;
			newEventCompletion.time = currentTime + _taskSet->getRuntime(schedulerQueue.front().taskindex);
			newEventCompletion.type = COMPLETION;
			newEventCompletion.job = schedulerQueue.front();
			eventQueue.push(newEventCompletion);
			schedulerQueue.pop();
		}
		
		if (VERBOSE > 3) {
			std::cout << " of job J" << currentJob.taskindex << "-" << currentJob.jobindex 
					  << ". (D="<< deadline <<"). Now executing J" << currentlyExecuting.taskindex << "-" << currentlyExecuting.jobindex << "." << std::endl;
		}
		
		if (VERBOSE > 2 && !_schedulable) {
			std::cout << " Deadline miss of job J" << currentJob.taskindex << "-" << currentJob.jobindex << " discovered at "<< currentTime << ". Deadline was at "<< deadline << " " <<  std::endl;
		}

		int progress = (float)((float)currentTime/(float)hyperPeriod)*1000;
		
		if (VERBOSE == 3 && !(progress % 10) ) {std::cout << " Simulation progress: " << currentTime << "/" << 2*hyperPeriod << ": " << (float)((float)currentTime/(float)hyperPeriod)*50 << "%            \r";}

	}
	
	if (VERBOSE == 3) {std::cout << std::endl;}
	
	return 0;
}