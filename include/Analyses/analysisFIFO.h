#ifndef analysisFIFO_H
#define analysisFIFO_H

#include <set>

#include "types.h"
#include "taskset.h"
#include "analysis.h"

class AnalysisFIFO: public Analysis  {
	public:
		AnalysisFIFO();
		~AnalysisFIFO();
		int analyse(Taskset &taskset);
	private:
		int checkSchedulability(int id, longint_t L);
		int computeQ(std::set<longint_t> &Q, longint_t L);
		longint_t computePD(longint_t, longint_t, int i);
		longint_t computeNinc(longint_t, longint_t, int l);
		longint_t computeNexc(longint_t, longint_t, int l);
		longint_t compute_L();
		longint_t compute_L_U();
		longint_t compute_L_R();
		float compute_Util();
		longint_t* OP;
};

#endif