#ifndef _SIMULATION_H
#define	_SIMULATION_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include "fscell.h"
#include "ltscell.h"  
#include "lscell.h"  
#include "izhiCom.h"
#include "synapse.h"

using namespace std;

typedef struct {
	unsigned char type;	
	int * posSyn;
	short int * delayType;
	short int * numConn;
	int length;
} preNeuron;

class simulation {
    public:
		simulation();
		~simulation();
		void run();
		void setSeed(long seed);
		void setH(float h);
		void setTmax(float tmax);
		void setNNeuron(int num);
		void setRescaleFac(float num);
		void setSizeNet(float num);		
		void rasterdata(string name);	
		void createNet();
		void setGin(float gin);
		void setGexc(float gexc);
		void printTime(string name);
		void printAvgFreq(string name);	
		void connData(string name);	    	
    protected:
    	int search(preNeuron * mtxPre, int num, int delay);
    	vector <neuron*> * listn;
    	preNeuron * mtxPre;
    	int nneuron, nLs[5];
    	float h, tmax, rescaleFac, sizeNet, gin, gexc;
};
    
#endif
