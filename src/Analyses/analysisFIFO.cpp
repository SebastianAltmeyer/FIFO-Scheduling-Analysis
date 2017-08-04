#include "Analyses/analysisFIFO.h"

AnalysisFIFO::AnalysisFIFO() {_schedulable = true;}

AnalysisFIFO::~AnalysisFIFO() {}

int AnalysisFIFO::analyse (Taskset &taskset) {

	_rt = new longint_t[taskset.getSize()];
	_taskSet = &taskset;
	_schedulable = true;
	
	float util = compute_Util();
	if (util >= 1) {
		  _schedulable = false;
		return -1;
	}
	
	longint_t L = compute_L();
	if (VERBOSE > 2) {
			std::cout << "  Busy Period L  =  " << L  << ". " << std::endl;
	}
	
	for (int id = 0; id < taskset.getSize() && _schedulable; id++) {
                _rt[id] = 0;
		checkSchedulability(id,L);

		if (VERBOSE > 1) {
			std::cout << " Worst Case Response Time R_" << id << " =  " << _rt[id]  << ". D_" << id << " = " << taskset.getDeadline(id) << "." << std::endl;
		}
	}
  
  
	delete[] _rt;
	return 0;
}

int AnalysisFIFO::checkSchedulability(int i, longint_t L) {
	
	OP = new longint_t[_taskSet->getSize()]; 		// \OP
  
	longint_t rij = L;
	longint_t dij = L + _taskSet->getDeadline(i);
	
	if (VERBOSE > 2) {
			std::cout << "  r_"<<i<<"^j = " << rij << " d"<< i<<"^j = " << dij << ". " << std::endl;
	}

	for (int l = 0; l < _taskSet->getSize(); l++) {
		longint_t minDist = 0;
		if (l <= i) {
		  minDist = _taskSet->computeMinOffset(i, l);
		} else {
		  minDist = _taskSet->computeMinOffsetPrime(i, l);
		}
		
		OP[l] = (rij - minDist) % _taskSet->getPeriod(l);
		
		if (VERBOSE > 2) {
			std::cout << "  r_"<<l<<"^k = " << (rij - minDist) 
			          << " delta" << i<<","<<l<<" = " << minDist 
			          << " OP"<<l<<" = " << OP[l] 
			          << ". " << std::endl;
		}

	}
	
	std::set<longint_t> Q;
	computeQ(Q, L); 

	for(std::set<longint_t>::iterator it=Q.begin(); _schedulable && it!=Q.end(); ++it) {
		longint_t t = *it;;
		longint_t PD = computePD(t,rij,i);
                
                _rt[i] = std::max(PD + t - rij, _rt[i]); 

                
		if (dij - t < PD) {
			if (VERBOSE > 2) {
				std::cout << "Miss:  PD("<<t<<","<<rij<<","<<i<<")=" << PD << " dij=" << dij << " t="<< t << " dij-t=" << dij - t <<  std::endl;
                        }
			_schedulable = false;
		} else {
			if (VERBOSE > 2) {
				std::cout << "Pass:  PD("<<t<<","<<rij<<","<<i<<")=" << PD << " dij=" << dij << " t="<< t << " dij-t=" << dij - t <<  std::endl;
			}
		}
	}
	
	delete[] OP;
	
	return 0;
}

longint_t AnalysisFIFO::computePD(longint_t t1, longint_t t2, int i) {
	longint_t PD = 0;
	
	for (int l = 0; l <=i; l++) {
		PD += computeNinc(t1,t2,l)*_taskSet->getRuntime(l);
	}
	
	for (int l = i+1; l < _taskSet->getSize(); l++) {
		PD += computeNexc(t1,t2,l)*_taskSet->getRuntime(l);
	}
	
	if (VERBOSE > 2) {
		std::cout << "  PD("<<t1<<","<<t2<<","<<i<<") = " << PD
				  << std::endl;
	}
	
	return PD;
}

longint_t AnalysisFIFO::computeNinc(longint_t t1, longint_t t2, int l) {
	
	longint_t diff_t2 = (t2 - OP[l] > 0)?t2 - OP[l]:0;
	longint_t diff_t1 = (t1 - OP[l] > 0)?t1 - OP[l]:0;
	
	float Term1 = (float)(diff_t2)/(float)_taskSet->getPeriod(l);
	float Term2 = (float)(diff_t1)/(float)_taskSet->getPeriod(l);
	
	
	longint_t N = floor(Term1) + 1 - ceil(Term2);
	
	if (VERBOSE > 2) {
		std::cout << "  Ninc("<<t1<<","<<t2<<","<<l<<") = " << N 
// 				  << " diff2 = " << diff_t2 
// 				  << " diff1 = " << diff_t1 
// 				  << " Term1 = " << Term1 
// 				  << " fl(Term1) = " << floor(Term1)
// 				  << " Term2 = " << Term2 
// 				  << " fl(Term2) = " << floor(Term2)
				  << std::endl;
	}
	
	return N;
}

longint_t AnalysisFIFO::computeNexc(longint_t t1, longint_t t2, int l) {
	longint_t diff_t2 = (t2 - OP[l] > 0)?t2 - OP[l]:0;
	longint_t diff_t1 = (t1 - OP[l] > 0)?t1 - OP[l]:0;
	
	float Term1 = (float)(diff_t2)/(float)_taskSet->getPeriod(l);
	float Term2 = (float)(diff_t1)/(float)_taskSet->getPeriod(l);
	
	
	longint_t N = ceil(Term1) - ceil(Term2);
	
	if (VERBOSE > 2) {
		std::cout << "  Nexc("<<t1<<","<<t2<<","<<l<<") = " << N 
// 				  << " diff2 = " << diff_t2 
// 				  << " diff1 = " << diff_t1 
// 				  << " Term1 = " << Term1 
// 				  << " fl(Term1) = " << floor(Term1)
// 				  << " Term2 = " << Term2 
// 				  << " fl(Term2) = " << floor(Term2)
				  << std::endl;
	}
	
	return N;
}

int AnalysisFIFO::computeQ(std::set<longint_t> &Q, longint_t L) {
	
	for (int i = 0; i < _taskSet->getSize(); i++) {
		longint_t t = OP[i];

		while (t <= L) {
// 			if (VERBOSE > 2) {
// 				std::cout << "  computeQ: "<< t <<" added to Q. " << std::endl;
// 			}
			Q.insert(t);
			t = t +  _taskSet->getPeriod(i);
		}
	}
	
	return 0;
}

float AnalysisFIFO::compute_Util() {
	float util = 0;
	
	for (int j = 0; j < _taskSet->getSize() ; j++) { 
	  
		util += ((float)_taskSet->getRuntime(j)/(float)_taskSet->getPeriod(j));
// 		std::cout << " Util[" << j <<"]" << ((float)_taskSet->getRuntime(j)/(float)_taskSet->getPeriod(j)) << std::endl;
	}
	
	if (VERBOSE > 3) {
		std::cout << " UtilResult: "<< util << std::endl;
	}
	
	return util;
}

longint_t AnalysisFIFO::compute_L() {

	longint_t L_U = std::max(compute_L_U(),(longint_t)0);
	longint_t L_R = std::max(compute_L_R(),(longint_t)0);
	
	longint_t L = std::min(L_U, L_R);

	if (VERBOSE > 2) {
		std::cout << " BusyPeriod L = "<< L 
							<< " LR = " << L_R
							<<  " LU = " << L_U << std::endl;
	}

	return L;
}

longint_t AnalysisFIFO::compute_L_U() {
	float util = compute_Util();
	if (util >= 1)
	  return -1;
	
	longint_t L_U = 0;
	longint_t diffDeadlineSum = 0;
	
	for (int j = 0; j < _taskSet->getSize() ; j++) { 
		L_U = std::max(L_U,_taskSet->getDeadline(j));
	  
		diffDeadlineSum += (_taskSet->getPeriod(j) - _taskSet->getDeadline(j));
	}
	
	longint_t diffDeadline = (diffDeadlineSum*util)/(1-util);
	
	L_U = std::max(L_U,diffDeadline);
	
	return L_U;
}

longint_t AnalysisFIFO::compute_L_R() {
	longint_t L_Old = 1;
	longint_t L = 1;
	
	bool finished = false;
	while (!finished) {

		L_Old = L;
		L = 0;
		for (int j = 0; j < _taskSet->getSize() ; j++) { // hp(i)
			L += ceil(float(L_Old)  / float (_taskSet->getPeriod(j)))*_taskSet->getRuntime(j);	
		}
		
		if (L == L_Old) finished = true;
	}

	
	return L;
}
