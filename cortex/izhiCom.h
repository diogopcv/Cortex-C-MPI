/*
 *  izhi.h
 *  
 *
 *  Created by Diogo Porfirio de Castro Vieira on 02/12/09.
 *  Copyright 2009 Universidade de Sao Paulo. All rights reserved.
 *
 */
#ifndef _IZHI_
#define _IZHI_
#include "neuron.h"

class izhiCom : public neuron {
   public:
    izhiCom();
    izhiCom(float aArg, float bArg, float cArg, float dArg);
    void setpar(float aArg, float bArg, float cArg, float dArg);        
    void setvrest(float vrest);
    void setvtresh(float vtresh);
    void setk(float k_);
    void setcap(float cap_);   
    void turnRS();
    void turnIBS();
    void turnCHS();  
   protected:
    void checkPeak(float time);
    void fx(float inj,float time);
    float a, b, c, d;
    float cap, k, vr, vt;
};
    
#endif
