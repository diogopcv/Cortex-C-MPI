/*
 *  neuron.h
 *  
 *
 *  Created by Diogo Porfirio de Castro Vieira on 02/12/09.
 *  Copyright 2009 Universidade de Sao Paulo. All rights reserved.
 *
 */
#ifndef _NEURON_
#define _NEURON_
#include <iostream>
#include <sstream>
#include <math.h>
#include <string>
#include <complex>
#include <vector>
#include "synapse.h"

class synapse;

class neuron {
public:
    neuron();
    ~neuron();     
    void evaluate(float inj, float time, float h) ;
    void setw0(double * w);
    double getW(int ind);
    void addsyndend(synapse * syn);
    void setId(int myId);
    int getID();
    std::vector<float> getevents();  
    void makeconnection(neuron * dend, synapse * syn);    
	double getIsyn(); 
	void clearEvents();    	
    
protected:
    virtual void checkPeak(float time) = 0;    	
    virtual void fx(float inj, float time) = 0;
	void startKs();  
	void backevent();      
    void sendevent(float time);
    double calcsyncurrent(float time);
    std::vector <float> * events; 
    std::vector <synapse*> * saxon;
    std::vector <synapse*> * sdend;
    double ** ks;
    double * fbuf;
    double * w, * waux;
    int _ID;
    int numeq;
    int typesyn;   
};

#endif
