#ifndef TASKSET_H
#define TASKSET_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

#include "types.h"

class Taskset
{
public:
	Taskset();
	~Taskset();
	
	int init(std::string name, 	int size, int* priority, longint_t* deadline, longint_t* period, longint_t* execTime, longint_t* offset, bool* sporadic);

	void setName(std::string);
	std::string getName();
	
	void setTaskName(std::string taskName, int i);
	std::string getTaskName(int i);
	
	int getPriority(int id);
	void setPriority(int id, int priority);

	longint_t getPeriod(int id); 
	void setPeriod(int id, longint_t period);

	longint_t getDeadline(int id);
	void setDeadline(int id, longint_t deadline);
		
	longint_t getRuntime(int id);
	void setRuntime(int id, longint_t execTime);
	
	longint_t getOffset(int id);
	void setOffset(int id, longint_t offset);	

	longint_t computeMinOffset(int id1, int id2);
	longint_t computeMinOffsetPrime(int id1, int id2);
	
	void setSize(int size);
	int getSize();

	void setResponseTime(int id, longint_t rt);
	longint_t getResponseTime(int id);

        bool isSporadic(int id);

        void setSporadic(int id, bool sporadic);
        
	int print();
	
	int print(std::string fileName);
	
	longint_t getHyperperiod();
	int computeHyperperiod();
	
private:
	longint_t computeGCD(longint_t a, longint_t b);
	longint_t computeLCM(longint_t a, longint_t b);
	
	std::string _name;
	int _size; //n
	int* _priority; 		// pr
	longint_t* _deadline; 		// D_i
	longint_t* _period; 			// T_i
	longint_t* _execTime;  	// C_i 
	longint_t* _offset; 		// O
	longint_t* _responseTime;  	// R_i
	bool* _sporadic;  	    // sporadic
	
	longint_t _hyperperiod;
};

#endif