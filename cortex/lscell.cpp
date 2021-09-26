//
//  lscell.cpp
//  cortex
//
//  Created by Diogo Porfirio de Castro Vieira on 24/11/11.
//  Copyright 2011 Universidade de Sao Paulo. All rights reserved.
//

#include "lscell.h"  

lscell::lscell() : neuron() {
    numeq = 3;
    fbuf = new double[numeq];
    w = new double[numeq];
    waux = new double[numeq];
    ks = new double * [numeq]; 
    for (int i = 0; i < numeq; i++)
    	ks[i] = new double[4];         
    startKs();   
    cap = 20.0, k = 0.3, vr = -66.0, vt = -40.0, capd = 100.0; gcoup = 1.2;
    setpar(0.17, 5.0, -45.0, 100.0);
	double w_[3] = {vr,0.0,vr};
    setw0(w_);        
}    

lscell::lscell(float aArg, float bArg, float cArg, float dArg) : neuron() {
    numeq = 3;
    fbuf = new double[numeq];
    w = new double[numeq];
    waux = new double[numeq];
    ks = new double * [numeq]; 
    for (int i = 0; i < numeq; i++)
    	ks[i] = new double[4];         
    startKs();  
    cap = 20.0, k = 0.3, vr = -66.0, vt = -40.0, capd = 100.0; gcoup = 1.2;
    setpar(aArg, bArg, cArg, dArg);
	double w_[3] = {vr,0.0,vr};
    setw0(w_);           
}

void lscell::fx(float inj, float time) {
    inj -= calcsyncurrent(time);
    fbuf[0] = (k * (waux[0] - vr) * (waux[0] - vt)  + gcoup*(waux[2] - waux[0]) - waux[1] + inj)/cap;
    fbuf[1] = a * (b * (waux[0] - vr) - waux[1]);
    fbuf[2] = (waux[0]-waux[2])/capd;
    return;
}

void lscell::checkPeak(float time) {
    if (w[0] >= 30.0) {
        w[0] = c;
        w[1] = w[1] + d;
        sendevent(time);
        backevent();
    }
}

void lscell::setpar(float aArg, float bArg, float cArg, float dArg) {
    a = aArg;
    b = bArg;
    c = cArg;
    d = dArg;
}

void lscell::setvrest(float vrest){
    vr = vrest;
	double w_[3] = {(double) vrest,0.0,vrest};
    setw0(w_);
}

void lscell::setvtresh(float vtresh){
    vt = vtresh;
}

void lscell::setk(float k_){
    k = k_;
}

void lscell::setcap(float cap_){
    cap = cap_;
}

void lscell::setcapd(float cap_){
    capd = cap_;
}

void lscell::setgcoup(float gcoup_) {
    gcoup = gcoup_;
}