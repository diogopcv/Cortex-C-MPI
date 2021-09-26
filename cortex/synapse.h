/* 
 * File:   synapse.h
 * Author: diogopcv
 *
 * Created on 4 de Fevereiro de 2010, 13:24
 */

#ifndef _synapse_H
#define	_synapse_H

#include <vector>
#include <math.h>
#include "neuron.h"

class neuron;
	
class synapse {
public:
    synapse(bool shortArg, bool longArg);
    synapse(unsigned char typeSyn, float gmaxArg, float delayArg, bool shortArg, bool longArg);
    ~synapse();
    void calcK(short int indK, float h);
    void evaluate(float time, float h);
    void refreshM();	    
    void fx();
    unsigned char typeSyn;
    double * w, * waux;
    void addevent(float spk); 
protected:    	  
	std::vector <float> * spikes; 
	int numeq;
    double ** ks, * fbuf;
    float taux, p, delay, gmax;
    bool SHORTERM, LONGTERM;
};

#endif	/* _synapse_H */

