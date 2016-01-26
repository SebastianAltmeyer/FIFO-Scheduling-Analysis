#include <iostream>
#include <string>
#include "types.h"
#include "config.h"

#include "taskset.h"
#include "analysis.h"
#include "Analyses/analysisSim.h"
#include "Analyses/analysisFPPS.h"
#include "Analyses/analysisFPNS.h"
#include "Analyses/analysisFIFOsim.h"
#include "Analyses/analysisFIFO.h"


bool optimizeOffsetsExtTool(Config &conf, Analysis &analysis, Taskset &ts) {

// 	std::cout << " optimStart:"<< std::endl;
// 	ts.print();
	
	bool isSchedulable = false;
	longint_t offsetSave[ts.getSize()];
	
	std::string outFileS = "outFile.txt";
	std::string inFileS = "inFile.txt";
	std::string tmpFileS = "tmpFile.txt";
	std::ofstream outFile (outFileS.c_str(),std::ios::out);
	
	for (int i = 0; i < ts.getSize(); i++) { 
		offsetSave[i] = ts.getOffset(i); 
		
		outFile << "\"T" << i << "\";" 
				<< ts.getRuntime(i)<< ";" 
				<< ts.getPeriod(i)<<  std::endl;
	}
	
	std::string execString = "java -jar bin/runnablepacking.jar " 
							+ outFileS 
							+ " " + conf.getOptType() 
							+ " " + conf.getOptValue()
							+ " > " + tmpFileS;
	system(execString.c_str());
// 	std::cout << " " << execString << std::endl;
	
	execString = "sed 's/;/ /g' " + tmpFileS + " > " + inFileS;
	system(execString.c_str());
// 	std::cout << " " << execString << std::endl;
	
	std::ifstream inFile (inFileS.c_str(),std::ios::in);
	for (int i = 0; i < ts.getSize(); i++) { 
		offsetSave[i] = ts.getOffset(i); 
		std::string name;
		longint_t execTime, period, offset;
		
		inFile >> name >> execTime >> period >> offset;
		
// 		std::cout << "Name: " << name 
// 				  << " C: " << execTime 
// 				  << " P: " << period 
// 				  << " O: " << offset << std::endl;
		
		ts.setOffset(i,offset);
	}
	
	execString = "rm " + tmpFileS + " " + outFileS + " " + inFileS;
	system(execString.c_str());
// 	std::cout << " " << execString << std::endl;
	
// 	std::cout << " optimBeforeAnalysis:"<< std::endl;
// 	ts.print();
	
	analysis.analyse(ts);
	isSchedulable = analysis.isSchedulable();

	for (int i = 0; i < ts.getSize(); i++) {ts.setOffset(i,offsetSave[i]);}

// 	std::cout << " optimEnd:"<< std::endl;
// 	ts.print();
	
	return isSchedulable;
}

bool randomizeOffsets(Config &conf, Analysis &analysis, Taskset &ts) {

	bool isSchedulable = false;
	longint_t offsetSave[ts.getSize()];
	
	int runs = conf.getNrOfSteps();
	
	for (int i = 0; i < ts.getSize(); i++) { offsetSave[i] = ts.getOffset(i); }

	while(runs > 0 && !isSchedulable) {
				
		for (int i = 0; i < ts.getSize(); i++) { 
			ts.setOffset(i,rand() % ts.getPeriod(i)); 
		}

// 		std::cout << " New try:" << std::endl;

// 		if (VERBOSE>1) ts.print();
		
		analysis.analyse(ts);
		isSchedulable = analysis.isSchedulable();
		runs--;
		
// 		if (isSchedulable && conf.simulation()) {
// 			AnalysisSim simulator;
// 			simulator.analyse(ts);
// 			if (simulator.isSchedulable()) {
// 				std::cout << " Yupp, validated by simulation." << std::endl;
// 			} else {
// 				if (VERBOSE>1) ts.print();
// 				std::cout << " BIDOBIDOBIDO." << std::endl;
// 			}
// 				
// 		}		
	}

	for (int i = 0; i < ts.getSize(); i++) { ts.setOffset(i,offsetSave[i]); }

	return isSchedulable;
}

bool optimizeOffsets(Config &conf, Analysis &analysis, Taskset &ts) {

	bool isSchedulable;
	
	if (conf.getOptType() == "rand") {
		isSchedulable = randomizeOffsets(conf, analysis, ts);
	}
	else {
		isSchedulable = optimizeOffsetsExtTool(conf, analysis, ts);
	}
	
	return isSchedulable;
}


int analyseTaskSet(Config &conf, Statistics &stats, Taskset &ts) {
	AnalysisFPPS analysisFPPS;
	if (analysisFPPS.analyse(ts)) { std::cerr << "Error: FPPS analysis failed." << std::endl; return 1;
	} else { if (VERBOSE > 2) std::cout << " ResponseTimeAnalysis done." << std::endl; }
	
	if (analysisFPPS.isSchedulable()) {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is schedulable with FPPS!" << std::endl;}
	} else {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is NOT schedulable with FPPS!" << std::endl;}
	}
	stats.FPPS += analysisFPPS.isSchedulable();
	
	AnalysisFPNS analysisFPNS;
	if (analysisFPNS.analyse(ts)) { std::cerr << "Error: FPNS analysis failed." << std::endl; return 1;
	} else { if (VERBOSE > 2) std::cout << " ResponseTimeAnalysis done." << std::endl; }
	
	if (analysisFPNS.isSchedulable()) {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is schedulable with FPNS!" << std::endl;}
	} else {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is NOT schedulable with FPNS!" << std::endl;}
	}
	stats.FPNS_NO += analysisFPNS.isSchedulable();


	AnalysisFPNS analysisFPNS_OFFSETS;
	if (analysisFPNS_OFFSETS.analyse(ts)) { std::cerr << "Error: FPNS analysis failed." << std::endl; return 1;
	} else { if (VERBOSE > 2) std::cout << " ResponseTimeAnalysis done." << std::endl; }
	
	if (analysisFPNS_OFFSETS.isSchedulable()) {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is schedulable with FPNS!" << std::endl;}
	} else {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is NOT schedulable with FPNS!" << std::endl;}
	}
	stats.FPNS_OFFSETS += analysisFPNS_OFFSETS.isSchedulable();

	AnalysisFPNS analysisFPNS_OFFSETS_OPT;
	bool FPNS_OFFSETS_OPT = optimizeOffsets(conf, analysisFPNS_OFFSETS_OPT, ts);
	if (FPNS_OFFSETS_OPT) {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is schedulable with FPNS, opt Offset!" << std::endl;}
	} else {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is NOT schedulable with FPNS, opt Offset!" << std::endl;}
	}
	stats.FPNS_OFFSETS_OPT += FPNS_OFFSETS_OPT;
	
	AnalysisFIFOsim analysisFIFO;
	if (analysisFIFO.analyse(ts)) { std::cerr << "Error: FPNS analysis failed." << std::endl; return 1;
	} else { if (VERBOSE > 2) std::cout << " ResponseTimeAnalysis done." << std::endl; }
	
	if (analysisFIFO.isSchedulable()) {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is schedulable with FIFO (naive analysis)!" << std::endl;}
	} else {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is NOT schedulable with FIFO (naive analysis)!" << std::endl;}
	}
	stats.FIFO_NO += analysisFIFO.isSchedulable();

	AnalysisFIFO analysisFIFO_OFFSETS;
	if (analysisFIFO_OFFSETS.analyse(ts)) { std::cerr << "Error: FIFO analysis failed." << std::endl; return 1;
	} else { if (VERBOSE > 2) std::cout << " ResponseTimeAnalysis done." << std::endl; }
	
	if (analysisFIFO_OFFSETS.isSchedulable()) {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is schedulable with FIFO (complex analysis)!" << std::endl;}
	} else {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is NOT schedulable with FIFO (complex analysis)!" << std::endl;}
	}
	stats.FIFO_OFFSETS += analysisFIFO_OFFSETS.isSchedulable();

	AnalysisFIFO analysisFIFO_OFFSETS_OPT;
	bool FIFO_OFFSETS_OPT = optimizeOffsets(conf, analysisFIFO_OFFSETS_OPT, ts);
	if (FIFO_OFFSETS_OPT) {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is schedulable with FIFO, opt Offset!" << std::endl;}
	} else {
		if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is NOT schedulable with FIFO, opt Offset!" << std::endl;}
	}
	stats.FIFO_OFFSETS_OPT += FIFO_OFFSETS_OPT;

	if (conf.simulation()) {
		AnalysisSim simulator;
		if (simulator.analyse(ts)) { std::cerr << "Error: simulation failed." << std::endl; return 1;
		} else { if (VERBOSE > 2) std::cout << " ResponseTimeAnalysis done." << std::endl; }
		
		if (simulator.isSchedulable()) {
			if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is schedulable on processor acc. to simulation!" << std::endl;}
		} else {
			if (VERBOSE > 1) {std::cout << " Taskset \'" << ts.getName() <<  "\' is NOT schedulable on processor  acc. to simulation!" << std::endl;}
		}
		stats.FIFO_SIM += simulator.isSchedulable();
	}
	
	return 0;
}

int analyseConfiguration(Config &conf) {

// 	std::ios_base::openmode mode = std::ios::app;std::ios::out
	std::ios_base::openmode mode = std::ios::out;
	
	std::string outFPPS, 
				outFIFO_OFFSETS_OPT,
				outFIFO_OFFSETS,
				outFIFO_NO,
				outFIFO_SIM,
				outFPNS_OFFSETS_OPT,
				outFPNS_OFFSETS,
				outFPNS_NO,
				outWE;

	outFPPS = conf.getOutFile() + "_FPPS";
	
	outFIFO_OFFSETS_OPT = conf.getOutFile() + "_FIFO_OFFSETS_OPT";
	outFIFO_OFFSETS = conf.getOutFile() + "_FIFO_OFFSETS";
	outFIFO_NO = conf.getOutFile() + "_FIFO_NO";
	outFIFO_SIM  = conf.getOutFile() + "_FIFO_SIM";
	
	outFPNS_OFFSETS_OPT = conf.getOutFile() + "_FPNS_OFFSETS_OPT";
	outFPNS_OFFSETS = conf.getOutFile() + "_FPNS_OFFSETS";
	outFPNS_NO = conf.getOutFile() + "_FPNS_NO";
	
	outWE = conf.getOutFile() + "_we";

	std::ofstream outFileFPPS (outFPPS.c_str(),mode);
	
	std::ofstream outFileFIFO_OFFSETS_OPT (outFIFO_OFFSETS_OPT.c_str(),mode);
	std::ofstream outFileFIFO_OFFSETS (outFIFO_OFFSETS.c_str(),mode);
	std::ofstream outFileFIFO_NO (outFIFO_NO.c_str(),mode);
	std::ofstream outFileFIFO_SIM (outFIFO_SIM.c_str(),mode);
	
	std::ofstream outFileFPNS_OFFSETS_OPT (outFPNS_OFFSETS_OPT.c_str(),mode);
	std::ofstream outFileFPNS_OFFSETS (outFPNS_OFFSETS.c_str(),mode);
	std::ofstream outFileFPNS_NO (outFPNS_NO.c_str(),mode);
	
	std::ofstream outFileWE (outWE.c_str(),mode);
	
	float W_FPPS = 0;	
	
	float W_FIFO_OFFSETS_OPT = 0;
	float W_FIFO_OFFSETS = 0;
	float W_FIFO_NO = 0;
	float W_FIFO_SIM = 0;

	float W_FPNS_OFFSETS_OPT = 0;
	float W_FPNS_OFFSETS = 0;
	float W_FPNS_NO = 0;

	float UtilSum = 0;

	Statistics stats;
	
	if (VERBOSE > -1)
		std::cout << "Start Analysis." << std::endl;
	
	for (float util = conf.getUtilStart(); util <= conf.getUtilEnd(); util += conf.getUtilStep()) {

		stats.FPPS = 0;

		stats.FIFO_OFFSETS_OPT = 0;
		stats.FIFO_OFFSETS = 0;
		stats.FIFO_NO = 0;
		stats.FIFO_SIM = 0;

		stats.FPNS_OFFSETS_OPT = 0;
		stats.FPNS_OFFSETS = 0;
		stats.FPNS_NO = 0;
		
		int maxRuns = conf.getNrOfTaskSets();

		if (VERBOSE > 0) {std::cout << " Analyse task set utilization = \'" << util <<  "." << std::endl;}
		
		for (int tsIndex = 0; tsIndex < maxRuns; tsIndex++) {
			Taskset ts;
		
			std::stringstream sstream;  
			sstream << "T-" << tsIndex << "-" << util;

			srand(conf.getSeed()+tsIndex);
			conf.genTaskSet(ts, sstream.str(), util);
			
			if (VERBOSE>1) ts.print();

			analyseTaskSet(conf,stats,ts);
		}
		
		outFileFPPS << util << " " << stats.FPPS << std::endl;

		outFileFIFO_OFFSETS_OPT << util << " " << stats.FIFO_OFFSETS_OPT << std::endl;
		outFileFIFO_OFFSETS << util << " " << stats.FIFO_OFFSETS << std::endl;
		outFileFIFO_NO << util << " " << stats.FIFO_NO << std::endl;
		outFileFIFO_SIM << util << " " << stats.FIFO_SIM << std::endl;

		outFileFPNS_OFFSETS_OPT << util << " " << stats.FPNS_OFFSETS_OPT << std::endl;
		outFileFPNS_OFFSETS << util << " " << stats.FPNS_OFFSETS << std::endl;
		outFileFPNS_NO << util << " " << stats.FPNS_NO << std::endl;
		
		W_FPPS += ((float)stats.FPPS/(float)maxRuns)*util;
		
		W_FIFO_OFFSETS_OPT += ((float)stats.FIFO_OFFSETS_OPT/(float)maxRuns)*util;
		W_FIFO_OFFSETS += ((float)stats.FIFO_OFFSETS/(float)maxRuns)*util;
		W_FIFO_NO += ((float)stats.FIFO_NO/(float)maxRuns)*util;
		W_FIFO_SIM += ((float)stats.FIFO_SIM/(float)maxRuns)*util;
		
		W_FPNS_OFFSETS_OPT += ((float)stats.FPNS_OFFSETS_OPT/(float)maxRuns)*util;
		W_FPNS_OFFSETS += ((float)stats.FPNS_OFFSETS/(float)maxRuns)*util;
		W_FPNS_NO += ((float)stats.FPNS_NO/(float)maxRuns)*util;
		
		UtilSum += util;
		
	}
	
	W_FPPS /= UtilSum;
	
	W_FIFO_OFFSETS_OPT /= UtilSum;
	W_FIFO_OFFSETS /= UtilSum;
	W_FIFO_NO /= UtilSum;
	W_FIFO_SIM /= UtilSum;
	
	W_FPNS_OFFSETS_OPT /= UtilSum;
	W_FPNS_OFFSETS /= UtilSum;
	W_FPNS_NO /= UtilSum;
	
	if (VERBOSE > -1)
	std::cout << "Weighted Measure:  " << std::endl
				<< "FPPS:            " << W_FPPS  << std::endl
				<< "FIFO Offset Opt: " << W_FIFO_OFFSETS_OPT << std::endl
				<< "FIFO Offset:     " << W_FIFO_OFFSETS << std::endl
				<< "FIFO No Offset:  " << W_FIFO_NO  << std::endl
				<< "FIFO Sim.:       " << W_FIFO_SIM  << std::endl
				<< "FPNS Offset Opt: " << W_FPNS_OFFSETS_OPT << std::endl
				<< "FPNS Offset:     " << W_FPNS_OFFSETS << std::endl
				<< "FPNS No Offset:  " << W_FPNS_NO << std::endl;
				
	outFileWE << W_FPPS  << " " << W_FIFO_OFFSETS_OPT << " " << W_FIFO_OFFSETS  << " " << W_FIFO_NO  << " " << W_FIFO_SIM  << " " << W_FPNS_OFFSETS_OPT<< " " << W_FPNS_OFFSETS << " " << W_FPNS_NO;
	
	return 0;
}

int analyseExample(Config &conf) {

	Statistics stats;
	
	if (VERBOSE > -1)
		std::cout << "Start Example Analysis." << std::endl;

	stats.FIFO_OFFSETS_OPT = 0;
	stats.FIFO_OFFSETS = 0;
	stats.FIFO_NO = 0;
	stats.FIFO_SIM = 0;

	stats.FPNS_OFFSETS_OPT = 0;
	stats.FPNS_OFFSETS = 0;
	stats.FPNS_NO = 0;

	Taskset ts;	
	conf.fillTaskSet(ts);
	if (VERBOSE>1) ts.print();

	analyseTaskSet(conf,stats,ts);

	return 0;
}


int main(int argc, char **argv) {

	Config conf;

	std::string configFile;
	
	if (conf.parseCommandLine(argc, argv)) {
		std::cerr << "Error while reading command line arguments, abort execution." << std::endl << std::endl;
		exit(1);
	}		
		
	if (conf.checkConfig()) {
		std::cerr << "Error: underspecified configuration." << std::endl;
		return 0;
	}
	
	if (conf.printHelp()) {
		return 0;
	}
	
	if (VERBOSE > -1)
		std::cout 	<< std::endl 
					<< "------------------------------------------------------------------------" << std::endl
					<< "                Welcome to the FIFO Scheduling Analyzer!                " << std::endl
					<< "------------------------------------------------------------------------" << std::endl
					<< std::endl;


	if (conf.example()) {
		analyseExample(conf);
	} else if (analyseConfiguration(conf)) {
		std::cerr << "Error: configuration could not be analysed." << std::endl;
		return 0;
	} else {
		if (VERBOSE > -1)
			std::cout << "Analysis finished successfully." << std::endl;
	}

	if (VERBOSE > -1)
	std::cout 	<< std::endl 
				<< "------------------------------------------------------------------------" << std::endl
				<< "                                Goodbye!                                " << std::endl
				<< "------------------------------------------------------------------------" << std::endl
				<< std::endl;
		
	return 0;
}

