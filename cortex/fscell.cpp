//
//  fscell.cpp
//  cortex
//
//  Created by Diogo Porfirio de Castro Vieira on 27/11/11.
//  Copyright 2011 Universidade de Sao Paulo. All rights reserved.
//

#include "fscell.h"  

fscell::fscell() : neuron() {
    numeq = 2;
    fbuf = new double[numeq];
    w = new double[numeq];
    waux = new double[numeq];
    ks = new double * [numeq]; 
    for (int i = 0; i < numeq; i++)
    	ks[i] = new double[4];         
    startKs();
    cap = 20.0, k = 1.0, vr = -55.0, vt = -40.0;
    setpar(0.15, 8, -55.0, 200.0);
	double w_[2] = {vr,0.0};
    setw0(w_);        
}    

fscell::fscell(float aArg, float bArg, float cArg, float dArg) : neuron() {
    numeq = 2;
    fbuf = new double[numeq];
    w = new double[numeq];
    waux = new double[numeq];
    ks = new double * [numeq]; 
    for (int i = 0; i < numeq; i++)
    	ks[i] = new double[4];         
    startKs();
    cap = 20.0, k = 1.0, vr = -55.0, vt = -40.0, vb = -55.0;
    setpar(aArg, bArg, cArg, dArg);
	double w_[2] = {vr,0.0};
    setw0(w_);          
}

void fscell::fx(float inj, float time) {
	inj -= calcsyncurrent(time);
	fbuf[0] = (k * (waux[0] - vr) * (waux[0] - vt) - waux[1] + inj)/cap;
	fbuf[1] = a * (b * (waux[0] - vr) - waux[1]);
}

void fscell::checkPeak(float time) {
    if (w[0] >= 25.0) {
        w[0] = c;
        w[1] = w[1] + d;
        sendevent(time);
        backevent();
    }
}    

void fscell::setpar(float aArg, float bArg, float cArg, float dArg) {
    a = aArg;
    b = bArg;
    c = cArg;
    d = dArg;
}

void fscell::setvrest(float vrest){
    vr = vrest;
	double w_[2] = {(double) vrest,0.0};
    setw0(w_);
}

void fscell::setvtresh(float vtresh){
    vt = vtresh;
}

void fscell::setk(float k_){
    k = k_;
}

void fscell::setcap(float cap_){
    cap = cap_;
}
