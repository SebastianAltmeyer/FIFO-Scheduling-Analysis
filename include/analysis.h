#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "types.h"
#include "taskset.h"
#include <cmath>
#include <iostream>

class Analysis
{
public:
	Analysis();
	virtual ~Analysis();
	virtual int analyse (Taskset& _taskset);
	bool isSchedulable();
protected:
	bool _schedulable;
	longint_t* _rt;
	Taskset* _taskSet;
};

#endif