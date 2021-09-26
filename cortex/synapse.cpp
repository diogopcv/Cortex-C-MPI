#include "synapse.h"

using std::vector;

synapse::synapse(bool shortArg, bool longArg) {
    if (!shortArg && !longArg){
    	numeq = 1;
    	ks = new double*[1];
    	for(int i = 0; i < 1; i++)
    		ks[i] = new double[4];
    	w = new double[1];
    	waux = new double[1];
    	fbuf = new double[1];
    	w[0] = 0;
		for(int i = 0; i < 4; i++)
			ks[0][i] = 0.0;
    }
    if (shortArg && !longArg){
    	numeq = 2;    	
    	ks = new double*[2];
    	w = new double[2];
    	waux = new double[2];  
    	fbuf = new double[2];  	
    	for(int i = 0; i < 2; i++)
    		ks[i] = new double[4];
    	for(int i = 0; i < 2; i++){
    		w[i] = 0;
 	    	for(int j = 0; j < 4; j++) 
 	    		ks[i][j] = 0.0;  
    	}
    	w[1] = 1.0; 			
    }
    if (!shortArg && longArg){
    	numeq = 3;    	
    	ks = new double*[3];
    	w = new double[3];
    	waux = new double[3];  
    	fbuf = new double[3];  	
    	for(int i = 0; i < 3; i++)
    		ks[i] = new double[4];
    	for(int i = 0; i < 3; i++){
    		w[i] = 0;
 	    	for(int j = 0; j < 4; j++) 
 	    		ks[i][j] = 0.0;
    	}
	}
    if (shortArg && longArg){
    	numeq = 4;    	
    	ks = new double*[4];
    	w = new double[4];
    	waux = new double[4];  
    	fbuf = new double[4];  	
    	for(int i = 0; i < 4; i++)
    		ks[i] = new double[4];
    	for(int i = 0; i < 4; i++){
    		w[i] = 0;
 	    	for(int j = 0; j < 4; j++) 
 	    		ks[i][j] = 0.0;  
    	} 
    	w[1] = 1.0;   	
    }
	spikes = new vector <float>;
    SHORTERM = shortArg;
    LONGTERM = longArg;	 	    	    	    	
    gmax = 1.0;
    delay = 5.0;
    p = 0.6; 
    typeSyn = 0;
}

synapse::synapse(unsigned char typeSynArg, float gmaxArg, float delayArg, bool shortArg, bool longArg) {
    if (!shortArg && !longArg){
    	numeq = 1;    	
    	ks = new double*[1];
    	for(int i = 0; i < 1; i++)
    		ks[i] = new double[4];
    	w = new double[1];
    	waux = new double[1];
    	fbuf = new double[1];
    	w[0] = 0;
		for(int i = 0; i < 4; i++)
			ks[0][i] = 0.0;
    }
    if (shortArg && !longArg){
    	numeq = 2;    	
    	ks = new double*[2];
    	w = new double[2];
    	waux = new double[2];
    	fbuf = new double[2];    	
    	for(int i = 0; i < 2; i++)
    		ks[i] = new double[4];
    	for(int i = 0; i < 2; i++){
    		w[i] = 0;
 	    	for(int j = 0; j < 4; j++) 
 	    		ks[i][j] = 0.0;  
    	}
    	w[1] = 1.0; 			
    }
    if (!shortArg && longArg){
    	numeq = 3;    	
    	ks = new double*[3];
    	w = new double[3];
    	waux = new double[3];    
    	fbuf = new double[3];	
    	for(int i = 0; i < 3; i++)
    		ks[i] = new double[4];
    	for(int i = 0; i < 3; i++){
    		w[i] = 0;
 	    	for(int j = 0; j < 4; j++) 
 	    		ks[i][j] = 0.0;
    	}
	}
    if (shortArg && longArg){
    	numeq = 4;    	
    	ks = new double*[4];
    	w = new double[4];
    	waux = new double[4];   
    	fbuf = new double[4]; 	
    	for(int i = 0; i < 4; i++)
    		ks[i] = new double[4];
    	for(int i = 0; i < 4; i++){
    		w[i] = 0;
 	    	for(int j = 0; j < 4; j++) 
 	    		ks[i][j] = 0.0;  
    	} 
    	w[1] = 1.0;   	
    }
    spikes = new vector <float>;
    gmax = gmaxArg;
    delay = delayArg;
    SHORTERM = shortArg;  
    LONGTERM = longArg;
    taux = 150;
    p = 0.6;
    typeSyn = typeSynArg;    
}   

synapse::~synapse(){
	delete[] fbuf;
	delete[] w;
	delete[] waux;
	spikes->clear();
	delete spikes;
	for (int i = 0; i < numeq; i++)
		delete[] ks[i];
	delete[] ks;   	
} 

void synapse::evaluate(float time, float h) {
    
    if (!LONGTERM && !SHORTERM){
        w[0] += (ks[0][0] + 2*ks[0][1] + 2*ks[0][2] + ks[0][3])/6; 
        if (!spikes->empty()){
            float s = time - spikes->at(0) - delay;          
            if (s>0){
                w[0] += gmax;
                spikes->erase(spikes->begin());
            }
        }        
        return;
    } 
    
    if (SHORTERM && !LONGTERM){ 
        w[0] += (ks[0][0] + 2*ks[0][1] + 2*ks[0][2] + ks[0][3])/6; 
        w[1] += (ks[1][0] + 2*ks[1][1] + 2*ks[1][2] + ks[1][3])/6; 
        if (!spikes->empty()){
            float s = time - spikes->at(0) - delay;          
            if (s>0){	        		 
                w[0] += w[1]*gmax;
                w[1] = p*w[1];
                spikes->erase(spikes->begin());
            }
        }    
        return;
    }
    
    if (LONGTERM && !SHORTERM){ 
        w[0] += (ks[0][0] + 2*ks[0][1] + 2*ks[0][2] + ks[0][3])/6; 
        w[1] += (ks[1][0] + 2*ks[1][1] + 2*ks[1][2] + ks[1][3])/6;
        w[2] += (ks[2][0] + 2*ks[2][1] + 2*ks[2][2] + ks[2][3])/6;
        if (!spikes->empty()){
            float s = time - spikes->at(0) - delay;          
            if (s>0){	        		 
                w[0] += gmax;
//                printf("%lf\t%lf\n",w[0],gmax);
                w[1] += 1; //Aplus
//                gmax += Aplus*pow((1-gmax),mu)*w[2]*0.5;
                gmax += w[2]*0.5;	        		
                if(gmax<0.0) gmax=0.0;
                spikes->erase(spikes->begin());
            }
        }            
        return;
    }	
    
    if (LONGTERM && SHORTERM){
        w[0] += (ks[0][0] + 2*ks[0][1] + 2*ks[0][2] + ks[0][3])/6; 
        w[1] += (ks[1][0] + 2*ks[1][1] + 2*ks[1][2] + ks[1][3])/6;
		w[2] += (ks[2][0] + 2*ks[2][1] + 2*ks[2][2] + ks[2][3])/6;  
        w[3] += (ks[3][0] + 2*ks[3][1] + 2*ks[3][2] + ks[3][3])/6;
        if (!spikes->empty()){
            float s = time - spikes->at(0) - delay;          
            if (s>0){	        		 
                w[0] += w[1]*gmax;
                w[1] = p*w[1];
                w[2] += 1; //Aplus
//                gmax += Aplus*pow((1-gmax),mu)*w[3]*0.5;
                gmax += w[3]*0.5;	        		
                if(gmax<0.0) gmax=0.0;
                spikes->erase(spikes->begin());
            }
        }            
        return;
    }    		
}

void synapse::refreshM(){
    if (LONGTERM && SHORTERM){
        w[3] -= 2; //Aminus
//        gmax += Aminus*pow(getGmax(),mu)*Pa*0.5;	
        gmax += w[2]*0.5;
        if(gmax>200.0) gmax=200.0;
    } 
     
    if (LONGTERM && !SHORTERM){
        w[2] -= 2; //Aminus
//        gmax += Aminus*pow(getGmax(),mu)*Pa*0.5;	
        gmax += w[1]*0.5;
        if(gmax>200.0) gmax=200.0;
    }       	
}   

void synapse::fx(){
    if (typeSyn == 0)
	    fbuf[0] = - (waux[0]/5.0);	
    else if (typeSyn == 1)
    	fbuf[0] = - (waux[0]/150.0);
    else if (typeSyn == 2)
    	fbuf[0] = - (waux[0]/6.0);
    else if (typeSyn == 3)   
    	fbuf[0] = - (waux[0]/150.0);     
    
    if (!LONGTERM && SHORTERM){
		fbuf[1] = - ((waux[1]-1)/taux);	
        return;
    }
    if (LONGTERM && !SHORTERM){   
        fbuf[1] = - (waux[1]/20.0); // tauPlus
        fbuf[2] = - (waux[2]/20.0); // tauMinxus
        return;
    }
    if (LONGTERM && SHORTERM){
        fbuf[1] = - ((waux[1]-1)/taux);
        fbuf[2] = - (waux[2]/20.0); // tauPlus
        fbuf[3] = - (waux[3]/20.0); // tauMinxus
        return;
    }	
}

void synapse::addevent(float spk) {
	spikes->push_back(spk);
}

void synapse::calcK(short int indK, float h){
	if (indK == 0)
		waux[0] = w[0];
	if (indK == 1 || indK == 2)
		waux[0] = w[0] + ks[0][indK - 1]/2;
	if (indK == 3)
		waux[0] = w[0] + ks[0][indK - 1];
	
	fx();	
    ks[0][indK] = h * fbuf[0];	
    
    if (!LONGTERM && SHORTERM){
        waux[1] = w[1];
        ks[1][indK] = h * fbuf[1];	
        return;
    }
    if (LONGTERM && !SHORTERM){   
        waux[1] = w[1];
        waux[2] = w[2];
        ks[1][indK] = h * fbuf[1];
        ks[2][indK] = h * fbuf[2];
        return;
    }
    if (LONGTERM && SHORTERM){
        waux[1] = w[1];
        waux[2] = w[2];
        waux[3] = w[3];
        ks[1][indK] = h * fbuf[1];
        ks[2][indK] = h * fbuf[2];
        ks[3][indK] = h * fbuf[3];
        return;
    }
}