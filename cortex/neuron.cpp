/* 
 * File:   neuron.cpp
 * Author: diogopcv
 * 
 * Created on 4 de Fevereiro de 2010, 13:22
 */

#include "neuron.h"
using std::vector;

neuron::neuron() {
	_ID = 0;     	
    saxon = new vector <synapse*>;
    sdend = new vector <synapse*>;
    events = new vector <float>;      
}    

neuron::~neuron(){
	delete[] fbuf;
	delete[] w;
	delete[] waux;
	for (int i = 0; i < numeq; i++)
		delete[] ks[i];	
	delete[] ks;	
	events->clear();
	delete events;
	
	synapse * psyn = NULL;
	for(int i = 0; i < sdend->size(); i++){
		psyn = sdend->at(i);
		delete psyn;
		psyn = NULL;
	}
	sdend->clear();	
	saxon->clear();	
	delete sdend;	
	delete saxon;   	    	
}

void neuron::setw0(double * w_) {
	for (int i = 0; i < numeq; i++) {
		w[i] = w_[i];
	}
}

double neuron::getW(int ind) {
    return w[ind];
}

void neuron::clearEvents(){
	events->clear();
	return;
}

double neuron::calcsyncurrent(float time) {
	double isyn = 0.0;
	int i;
	for(i = 0; i < sdend->size(); i++){  
		if ((sdend->at(i))->typeSyn == 0)
			isyn += (sdend->at(i))->waux[0]*(waux[0] - 0.0);			
		if ((sdend->at(i))->typeSyn == 1)
			isyn += (sdend->at(i))->waux[0]*pow((waux[0] + 80)/60,2)/(1 + pow((waux[0] + 80)/60,2))*(waux[0] - 0.0);
		if ((sdend->at(i))->typeSyn == 2)
			isyn += (sdend->at(i))->waux[0]*(waux[0] + 70.0);
		if ((sdend->at(i))->typeSyn == 3)
			isyn += (sdend->at(i))->waux[0]*(waux[0] + 90.0);			
	}
	return isyn;
}    

double neuron::getIsyn(){
	double isyn = 0.0;
	int i;
	for(i = 0; i < sdend->size(); i++){  
		if ((sdend->at(i))->typeSyn == 0)
			isyn += (sdend->at(i))->waux[0]*(waux[0] - 0.0);			
		if ((sdend->at(i))->typeSyn == 1)
			isyn += (sdend->at(i))->waux[0]*pow((waux[0] + 80)/60,2)/(1 + pow((waux[0] + 80)/60,2))*(waux[0] - 0.0);
		if ((sdend->at(i))->typeSyn == 2)
			isyn += (sdend->at(i))->waux[0]*(waux[0] + 70.0);
		if ((sdend->at(i))->typeSyn == 3)
			isyn += (sdend->at(i))->waux[0]*(waux[0] + 90.0);			
	}
	return isyn;	
}

void neuron::evaluate(float inj, float time, float h) {

	// Calculando k1 
	for (int i = 0; i < numeq; i++)
		waux[i] = w[i];
	for(int i = 0; i < sdend->size(); i++)	 
		(sdend->at(i))->calcK(0, h);			            
	fx(inj, time);       
	for (int i = 0; i < numeq; i++)
		ks[i][0] = h * fbuf[i];
	
	// Calculando k2
	for (int i = 0; i < numeq; i++)
		waux[i] = w[i] + ks[i][0]/2;
	for(int i = 0; i < sdend->size(); i++)	 
		(sdend->at(i))->calcK(1, h);	              
	fx(inj, time);
	for (int i = 0; i < numeq; i++)
		ks[i][1] = h * fbuf[i];
	
	// Calculando k3
	for (int i = 0; i < numeq; i++)
		waux[i] = w[i] + ks[i][1]/2;
	for(int i = 0; i < sdend->size(); i++)	 
		(sdend->at(i))->calcK(2, h);  	             
	fx(inj, time);
	for (int i = 0; i < numeq; i++)
		ks[i][2] = h * fbuf[i];
	
	// Calculando k4
	for (int i = 0; i < numeq; i++)
		waux[i] = w[i] + ks[i][2];
	for(int i = 0; i < sdend->size(); i++)	
		(sdend->at(i))->calcK(3, h); 	        
	fx(inj, time);
	for (int i = 0; i < numeq; i++)
		ks[i][3] = h * fbuf[i];
	
	// Calculando w's novos
	for (int i = 0; i < numeq; i++)
		w[i] += (ks[i][0] + 2*ks[i][1] + 2*ks[i][2] + ks[i][3])/6;                                              
	
	for(int i = 0; i < sdend->size(); i++) 
		(sdend->at(i))->evaluate(time, h);             
	
	checkPeak(time);
}

void neuron::addsyndend(synapse * syn) {
	sdend->push_back(syn);
}

void neuron::makeconnection(neuron * dend, synapse * syn) {
    saxon->push_back(syn);
    dend->addsyndend(syn);
}

void neuron::sendevent(float time) {
    int i = 0;
    events->push_back(time);
    while (i < saxon->size()) {
        (saxon->at(i))->addevent(time);
        i++;
    }
}	

void neuron::backevent() {
	int i;
	for(i = 0; i < saxon->size(); i++) 
		(saxon->at(i))->refreshM();
}	

vector<float> neuron::getevents(){
    return *events;
}  

void neuron::startKs(){
	for(int i = 0; i < numeq; i++)
		for(int j = 0; j < 4; j++)
			ks[i][j] = 0.0; 		
}  	

void neuron::setId(int myId){
	_ID = myId;
}

int neuron::getID(){
	return _ID;
}	