//
//  ltscell.h
//  cortex
//
//  Created by Diogo Porfirio de Castro Vieira on 27/11/11.
//  Copyright 2011 Universidade de Sao Paulo. All rights reserved.
//

#ifndef cortex_ltscell_h
#define cortex_ltscell_h

#include "neuron.h"

class ltscell: public neuron {
public:
    ltscell();
    ltscell(float aArg, float bArg, float cArg, float dArg);
    void setpar(float aArg, float bArg, float cArg, float dArg);        
    void setvrest(float vrest);
    void setvtresh(float vtresh);
    void setk(float k_);
    void setcap(float cap_);     
    
protected:
    void checkPeak(float time);
    void fx(float inj,float time);
    float a, b, c, d;
    float cap, k, vr, vt;
};
#endif
