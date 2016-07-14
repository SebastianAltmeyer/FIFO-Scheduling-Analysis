#include "taskset.h"

Taskset::Taskset() {
// 	std::cout << "TaskSet Constructor" << std::endl; 
	_priority = NULL;
	_deadline = NULL;
	_period = NULL;
	_execTime = NULL;
	_offset = NULL;
	_responseTime = NULL;
        _sporadic = NULL;
};

Taskset::~Taskset() {
// 	std::cout << "TaskSet Destructor" << std::endl; 
// 	std::cout << "   Destroy taskset " << _name << " with " << _size << " tasks." << std::endl; 
	delete[] _priority;
	delete[] _deadline;
	delete[] _period;
	delete[] _offset;
	delete[] _execTime;
	delete[] _responseTime;
        delete[] _sporadic;
};

int Taskset::init(std::string name, int size, int* priority, longint_t* deadline, longint_t* period, longint_t* execTime, longint_t* offset, bool* sporadic) {
	
	_size = size;
	_priority = priority;
	_deadline = deadline;
	_period = period;
	_execTime = execTime;
	_offset = offset;
	_name = name;
	_responseTime = new longint_t[size]();
        _sporadic = sporadic;
        
	return 0;
}

void Taskset::setName(std::string name) {
	_name = name;
}

std::string Taskset::getName() {
	return _name;
}	

int Taskset::getPriority(int id) {
	return _priority[id];
}

void Taskset::setPriority(int id, int priority) {
	_priority[id] = priority;
}

longint_t Taskset::getPeriod(int id) {
	return _period[id];
}

void Taskset::setRuntime(int id, longint_t execTime) {
	_execTime[id] = execTime;
}

longint_t Taskset::getRuntime(int id) {
	return _execTime[id];
}

void Taskset::setPeriod(int id, longint_t period) {
	_period[id] = period;
}

longint_t Taskset::getDeadline(int id) {
	return _deadline[id];
}

void Taskset::setDeadline(int id, longint_t deadline) {
	_deadline[id] = deadline;
}

int Taskset::getSize() {
	return _size;
}

bool Taskset::isSporadic(int id) {
        return _sporadic[id];
}

void Taskset::setSporadic(int id, bool sporadic) {
        _sporadic[id] = sporadic;
}

void Taskset::setSize(int size) {
	_size = size;
}

void Taskset::setResponseTime(int id, longint_t rt) {
	_responseTime[id] = rt;
}

longint_t Taskset::getResponseTime(int id) {
	return _responseTime[id];
}

void Taskset::setOffset(int id, longint_t rt) {
	_offset[id] = rt;
}

longint_t Taskset::getOffset(int id) {
	return _offset[id];
}

int Taskset::print() {

	std::cout << "Taskset " << _name << "" << std::endl;
	
	for (int i = 0; i < _size; i++) {
		std::cout << "  T_" << i << ":";
		std::cout << " C: " << _execTime[i] << " ";
		std::cout << " D: " << _deadline[i] << " ";
		std::cout << " T: " << _period[i] << " ";
		std::cout << " O: " << _offset[i] << "." << std::endl;
	}

	std::cout << "Done printing: " << _name << "" << std::endl;
	return 0;
}

longint_t Taskset::getHyperperiod() {
	return _hyperperiod;
}

int Taskset::computeHyperperiod() {
  
	if (_size == 0) 
	  return 1;

	_hyperperiod = _period[0];  
	
	longint_t newHyperperiod = _hyperperiod;
   
	for (int i = 1; i < _size; i++) {
		newHyperperiod = computeLCM(_hyperperiod, _period[i]);
		if (newHyperperiod < _hyperperiod)
			return 1;
		_hyperperiod = newHyperperiod;
	}
 
	return 0;
}

longint_t Taskset::computeGCD(longint_t a, longint_t b)
{
    for (;;)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

longint_t Taskset::computeLCM(longint_t a, longint_t b)
{
    longint_t temp = computeGCD(a, b);

    return temp ? (a / temp * b) : 0;
}

longint_t Taskset::computeMinOffset(int id1, int id2) {
  
        if (_sporadic[id1] || _sporadic[id2])
                return 0;
        
	int gcd = computeGCD(getPeriod(id1), getPeriod(id2));
	int diff = getOffset(id1) - getOffset(id2);
	int diffInv = getOffset(id2) - getOffset(id1);
	int minDist = 0;
	
	if (VERBOSE > 2) {
	}
	
	minDist = diff + ceil(((float)diffInv)/((float)gcd))*gcd; 
	
	return minDist;
}

longint_t Taskset::computeMinOffsetPrime(int id1, int id2) {
  
        if (_sporadic[id1] || _sporadic[id2])
                return 1;
        
	int gcd = computeGCD(getPeriod(id1), getPeriod(id2));
	int diff = getOffset(id1) - getOffset(id2);
	int diffInv = getOffset(id2) - getOffset(id1);
	int minDist = 0;
	
	minDist = diff + ceil(((float)diffInv+1)/((float)gcd))*gcd; 

	if (VERBOSE > 2) {
			std::cout << "  DeltaP: ("<<id1<<","<<id2<<"') = " << minDist 
									<< " diff = "<< diff 
									<< " diffInv = " << diffInv 
									<< " gcd = "<< gcd 
									<< " ceil(((float)diffInv)/((float)gcd)+1) = " << ceil(((float)diffInv+1)/((float)gcd))
									<< ". " << std::endl;
	}
	
	return minDist;
}

