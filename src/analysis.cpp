#include "analysis.h"

Analysis::Analysis() {};
Analysis::~Analysis() {};
int Analysis::analyse (Taskset &taskset) {return 0;};
bool Analysis::isSchedulable() {return _schedulable;};
