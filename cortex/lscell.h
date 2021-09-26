//
//  lscell.h
//  cortex
//
//  Created by Diogo Porfirio de Castro Vieira on 27/11/11.
//  Copyright 2011 Universidade de Sao Paulo. All rights reserved.
//

#ifndef cortex_lscell_h
#define cortex_lscell_h

#include "neuron.h"

class lscell: public neuron {
public:
    lscell();
    lscell(float aArg, float bArg, float cArg, float dArg);
    void setpar(float aArg, float bArg, float cArg, float dArg);        
    void setvrest(float vrest);
    void setvtresh(float vtresh);
    void setk(float k_);
    void setcap(float cap_);     
    void setcapd(float cap_); 
    void setgcoup(float gcoup_);
    
protected:
    void checkPeak(float time);
    void fx(float inj,float time);
    float a, b, c, d;
    float cap, k, vr, vt, capd, gcoup;
};

#endif
