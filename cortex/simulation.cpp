#include "simulation.h"
#include "tools.cpp"

long rand0::idum;

simulation::simulation () {
	tmax = 2200.0;
	h = 0.01;  
	setSeed(-184503872);
	setNNeuron(3129);
	setRescaleFac(100.0);
	setSizeNet(2.0);						
	listn = new vector <neuron*>;
}

simulation::~simulation () {
	neuron * ptrn = NULL;
	for(int i = 0; i < listn->size(); i++){
		ptrn = listn->at(i);
		delete ptrn;
		ptrn = NULL;
	}
	listn->clear();
	delete listn;
	listn = NULL;
	
	int * posSyn = NULL;
	short int * delayType = NULL;
	short int * numConn = NULL;
	
	for(int i = 0; i < this->nneuron; i++){
		posSyn = mtxPre[i].posSyn;
		delayType = mtxPre[i].delayType;
		numConn = mtxPre[i].numConn;
		delete[] posSyn;
		delete[] delayType;
		delete[] numConn;
		posSyn = NULL;
		delayType = NULL;
		numConn = NULL;
	}					
	delete[] mtxPre;
}

void simulation::run () {
	int Nstep = (int) ((float)tmax/h) + 1, i, j;
	float t = 0.0, stim;	
	bool reset = true;
	
	for(i = 0; i < Nstep; i++){
		t += h;
		
        if (t >= 0.0 && t <= 200){
        	stim = 250.0;            
        }else{
        	if (reset){
        		reset = false;
        		for (int i = 0; i < listn->size(); i++) {
        			(listn->at(i))->clearEvents();
        		}
        	}
        	stim = 0.0;
        }		
		
		for(j = 0; j < nneuron; j++){
			(listn->at(j))->evaluate(stim, t, h);			
		}	
	}	
}

void simulation::setH(float h){
	this->h = h;	
}

void simulation::setTmax(float tmax){
	this->tmax = tmax;	
}

void simulation::setNNeuron(int num){
	this->nneuron = num;	
}

void simulation::setRescaleFac(float num){
	this->rescaleFac = num;
}

void simulation::setSizeNet(float num){
	this->sizeNet = num;
}

void simulation::createNet () {
    int i, j, k, l, p, count, aux;
    float scaleL1, scaleL23, scaleL4, scaleL5, scaleL6, scalePre, scalePos, sort;
    unsigned char type;		
	
    nLs[0] = (int) round(sqrt(0.015*nneuron));
    nLs[1] = (int) round(sqrt(0.337*nneuron));
    nLs[2] = (int) round(sqrt(0.349*nneuron));
    nLs[3] = (int) round(sqrt(0.076*nneuron));
    nLs[4] = (int) round(sqrt(0.223*nneuron));       
    
    nneuron = 0;
    for (i = 0; i < 5; i++)
   		nneuron += nLs[i]*nLs[i];
   	
   	printf("%d\n", nneuron);
   	
   	setNNeuron(nneuron);
    	
	mtxPre = new preNeuron[nneuron]; 		    
    
    //definindo escala por camada
    scaleL1 = sizeNet/(nLs[0] - 1);
    scaleL23 = sizeNet/(nLs[1] - 1);
    scaleL4 = sizeNet/(nLs[2] - 1);
    scaleL5 = sizeNet/(nLs[3] - 1);
    scaleL6 = sizeNet/(nLs[4] - 1);
    
    //carrega arquivos de conexao e arborizacao axonal
    FILE * connFile, * axonFile;
    double connDat[33][21], axonDat[17][6];    
    connFile = fopen("dataConn.dat","r");
    axonFile = fopen("dataAxon.dat","r");    
    for(i = 0; i < 33*21; i++){
    	fscanf(connFile,"%lf", &connDat[i/21][i%21]);
    }   
    for(i = 0; i < 17*6; i++){
    	fscanf(axonFile,"%lf", &axonDat[i/6][i%6]);
    	axonDat[i/6][i%6] *= rescaleFac;    	
    }       
    fclose(connFile);
    fclose(axonFile);
    
    //Definindo por camada, tipo de cada neurônio   
    count = 0;
    for (i = 0; i < nLs[0]*nLs[0]; i++) { 
		mtxPre[count].type = 0;
		mtxPre[count].length = 0;	
		mtxPre[count].posSyn = NULL;
		mtxPre[count].delayType = NULL;
		mtxPre[count].numConn = NULL;
    	count++;
    }
    
    for (i = 0; i < nLs[1]*nLs[1]; i++) { 
    	sort = rand0::next();
    	if (sort < 0.78)
	    	mtxPre[count].type = 1;
    	else if (sort >= 0.78 && sort < 0.873)
	    	mtxPre[count].type = 2;    		
    	else
	   	mtxPre[count].type = 3;    		
    	mtxPre[count].length = 0;   	
        mtxPre[count].posSyn = NULL;
        mtxPre[count].delayType = NULL;
        mtxPre[count].numConn = NULL;
    	count++;  	
    }
    
    for (i = 0; i < nLs[2]*nLs[2]; i++) { 
    	sort = rand0::next();
    	if (sort < 0.27)
	    	mtxPre[count].type = 4;
    	else if (sort >= 0.27 && sort < 0.54)
	    	mtxPre[count].type = 5;    		
    	else if (sort >= 0.54 && sort < 0.81)
	    	mtxPre[count].type = 6;    		
    	else if (sort >= 0.81 && sort < 0.96)
	    	mtxPre[count].type = 7;    		
    	else
	    	mtxPre[count].type = 8;    		
		mtxPre[count].length = 0;	 	
        mtxPre[count].posSyn = NULL;
        mtxPre[count].delayType = NULL;
        mtxPre[count].numConn = NULL;
    	count++;     	
    }
    
    for (i = 0; i < nLs[3]*nLs[3]; i++) {
    	sort = rand0::next();
    	if (sort < 0.64)
	    	mtxPre[count].type = 9;
    	else if (sort >= 0.64 && sort < 0.81)
	    	mtxPre[count].type = 10;    		
    	else if (sort >= 0.81 && sort < 0.89)
	    	mtxPre[count].type = 11;    		
    	else
	 	mtxPre[count].type = 12;    		
		mtxPre[count].length = 0;	  	
        mtxPre[count].posSyn = NULL;
        mtxPre[count].delayType = NULL;
        mtxPre[count].numConn = NULL;
    	count++;     	
    }
    
    for (i = 0; i < nLs[4]*nLs[4]; i++) {
    	sort = rand0::next();
    	if (sort < 0.62)
	    	mtxPre[count].type = 13;
    	else if (sort >= 0.62 && sort < 0.82)
	    	mtxPre[count].type = 14;	
    	else if (sort >= 0.82 && sort < 0.91)
	    	mtxPre[count].type = 15;	
    	else
	    	mtxPre[count].type = 16;
		mtxPre[count].length = 0;
        mtxPre[count].posSyn = NULL;
        mtxPre[count].delayType = NULL;
        mtxPre[count].numConn = NULL;
    	count++;     	
    }
    
	/* A info do arquivo de conexao é lida linha a linha (p). Para cada linha
	varre-se todos o neuronios, aqueles que pertencerem ao tipo especificado
	pela linha é analisadq a info da linha em questao. Com a leitura é coletado 
	o indice dos neuronios potencialmente pre-sinapticos (analisando o tipo e
	distância (arquivo de arborizacao axonal) ). Após isso é sorteado dessa lista
	a quantidade de conexoes especificadas no arquivo de conexao */    
    
	int xPre, yPre, xPos, yPos, num, typePre, typePos, beginI, endI;
	int beginK, endK, initPre, initPos, nlayerPre, nlayerPos, layerPre, layerPos;		
	float re, dist;
	vector<int> listNeuron;
	vector<int> listDelay;
	
	int * _posSyn = NULL;
	short int * _delayType = NULL;
	short int * _numConn = NULL;
	
	// varredura das linhas do arquivo de conexao
	for (p = 0; p < 33; p++){
		typePos = (int) connDat[p][0];
		layerPos = (int) connDat[p][1];	
		// varredura de todos o neuronios
		if (typePos == 0){
			beginI = 0;
			endI = nLs[0]*nLs[0];
		}
		else if (typePos == 1 || typePos == 2 || typePos == 3){
			beginI = nLs[0]*nLs[0];
			endI =  nLs[0]*nLs[0] + nLs[1]*nLs[1];				
		}
		else if (typePos == 4 || typePos == 5 || typePos == 6 || typePos == 7 || typePos == 8){
			beginI = nLs[0]*nLs[0] + nLs[1]*nLs[1];
			endI =  nLs[0]*nLs[0] + nLs[1]*nLs[1] + nLs[2]*nLs[2];		
		}
		else if (typePos == 9 || typePos == 10 || typePos == 11 || typePos == 12){
			beginI = nLs[0]*nLs[0] + nLs[1]*nLs[1] + nLs[2]*nLs[2];	
			endI = nLs[0]*nLs[0] + nLs[1]*nLs[1] + nLs[2]*nLs[2] + nLs[3]*nLs[3];		
		}
		else{
			beginI = nLs[0]*nLs[0] + nLs[1]*nLs[1] + nLs[2]*nLs[2] + nLs[3]*nLs[3];	
			endI = nneuron;	
		}		
		for (i = beginI; i < endI; i++){
			//verifica se neuronio pos-sinaptico é o tipo especificado na linha
			if (mtxPre[i].type == typePos){
				// define a camada, escala e offset na lista de neuronios respectivo ao neuronio pos em questao			
				if (typePos == 0){
					initPos = 0;
					nlayerPos = nLs[0];
					scalePos = scaleL1;
				}
				else if (typePos == 1 || typePos == 2 || typePos == 3){
					initPos = nLs[0]*nLs[0];
					nlayerPos = nLs[1];	
					scalePos = scaleL23;				
				}
				else if (typePos == 4 || typePos == 5 || typePos == 6 || typePos == 7 || typePos == 8){
					initPos = nLs[0]*nLs[0] + nLs[1]*nLs[1];
					nlayerPos = nLs[2];	
					scalePos = scaleL4;				
				}
				else if (typePos == 9 || typePos == 10 || typePos == 11 || typePos == 12){
					initPos = nLs[0]*nLs[0] + nLs[1]*nLs[1] + nLs[2]*nLs[2];
					nlayerPos = nLs[3];	
					scalePos = scaleL5;				
				}
				else{
					initPos = nLs[0]*nLs[0] + nLs[1]*nLs[1] + nLs[2]*nLs[2] + nLs[3]*nLs[3];
					nlayerPos = nLs[4];		
					scalePos = scaleL6;		
				}			
				//define posicao (sem escala) do neuronio pos	
				xPos = (i - initPos)%nlayerPos;
				yPos = (i - initPos)/nlayerPos;	
				//varre lina do arquivo de conexao, coletando os neuronios pre-sinapticos de um determinado tipo no qual axonio atinge pos sinaptico
				for (j = 4; j < 21; j++){					
					if (connDat[p][j] > 0){
						//coleta informacao da arborizacao axonal do neuronio pre na camada informada pelo arquivo de conexao
						re = axonDat[j - 4][layerPos];
						//quanto neuronios pre de um detrminado tipo realizam sinapse neste neuronio pos
						count = (int) round(connDat[p][3]*(connDat[p][j]/(100.0*rescaleFac)));
						if (j == 4){
							beginK = 0;
							endK = nLs[0]*nLs[0];
						}
						else if (j == 5 || j == 6 || j == 7){
							beginK = nLs[0]*nLs[0];
							endK =  nLs[0]*nLs[0] + nLs[1]*nLs[1];				
						}
						else if (j == 8 || j == 9 || j == 10 || j == 11 || j == 12){
							beginK = nLs[0]*nLs[0] + nLs[1]*nLs[1];
							endK =  nLs[0]*nLs[0] + nLs[1]*nLs[1] + nLs[2]*nLs[2];		
						}
						else if (j == 13 || j == 14 || j == 15 || j == 16){
							beginK = nLs[0]*nLs[0] + nLs[1]*nLs[1] + nLs[2]*nLs[2];	
							endK = nLs[0]*nLs[0] + nLs[1]*nLs[1] + nLs[2]*nLs[2] + nLs[3]*nLs[3];		
						}
						else{
							beginK = nLs[0]*nLs[0] + nLs[1]*nLs[1] + nLs[2]*nLs[2] + nLs[3]*nLs[3];	
							endK = nneuron;	
						}						
						for (k = beginK; k < endK; k++){	
							typePre = mtxPre[k].type;	
							// define a camada, escala e offset na lista de neuronios respectivo ao neuronio pos em questao
							if(typePre == j - 4){
								if (typePre == 0){
									initPre = 0;
									nlayerPre = nLs[0];
									layerPre = 1;
									scalePre = scaleL1;
								}
								else if (typePre == 1 || typePre == 2 || typePre == 3){
									initPre = nLs[0]*nLs[0];
									nlayerPre = nLs[1];	
									layerPre = 2;
									scalePre = scaleL23;				
								}
								else if (typePre == 4 || typePre == 5 || typePre == 6 || typePre == 7 || typePre == 8){
									initPre = nLs[0]*nLs[0] + nLs[1]*nLs[1];
									nlayerPre = nLs[2];	
									layerPre = 3;	
									scalePre = scaleL4;			
								}
								else if (typePre == 9 || typePre == 10 || typePre == 11 || typePre == 12){
									initPre = nLs[0]*nLs[0] + nLs[1]*nLs[1] + nLs[2]*nLs[2];
									nlayerPre = nLs[3];		
									layerPre = 4;
									scalePre = scaleL5;			
								}
								else{
									initPre = nLs[0]*nLs[0] + nLs[1]*nLs[1] + nLs[2]*nLs[2] + nLs[3]*nLs[3];
									nlayerPre = nLs[4];		
									layerPre = 5;
									scalePre = scaleL6;		
								}	
								//define posicao (sem escala) do neuronio pre				
								xPre = (k - initPre)%nlayerPre;
								yPre = (k - initPre)/nlayerPre;	
								//verifica se neuronio pre atinge pos	
								dist = pow(xPre*scalePre-xPos*scalePos,2) + pow(yPre*scalePre-yPos*scalePos,2);
								if (dist < re*re){
									listNeuron.push_back(k);
									dist = sqrt(dist + pow(abs(layerPre - layerPos)*0.25,2));	
									if (dist/0.1 < 1.0)
										listDelay.push_back(1); 
									else if (dist/0.1 > 20.0)
										listDelay.push_back(20);
									else
										listDelay.push_back((int) round(dist/0.1)); 
								}
							}
						}
						
						//coletados neuronios potencialmente pre-sinapticos é realizado sorteio 
						for (k = 0; k < count; k++){		
							sort = round(rand0::next()*(listNeuron.size() - 1));					
							num = listNeuron.at((int) sort);
							
							aux = search(&mtxPre[num], i, listDelay.at((int) sort));
						
							if(aux < 0){	
								mtxPre[num].length++;
								type = mtxPre[num].type;
								_posSyn = new int[mtxPre[num].length];
								_delayType = new short int[mtxPre[num].length];
								_numConn = new short int[mtxPre[num].length];
								
								for (l = 0; l < (mtxPre[num].length - 1); l++){			
									_posSyn[l] = mtxPre[num].posSyn[l];
									_numConn[l] = mtxPre[num].numConn[l];
									_delayType[l] = mtxPre[num].delayType[l];									
								}
								
								_posSyn[l] = i;
								_numConn[l] = 1;
								_delayType[l] = listDelay.at((int) sort);
								if (type == 0 || type == 2 || type == 3 || type == 7 || type == 8 || type == 11 || type == 12 || type == 15 || type == 16){
//									if(rand0::next() < 0.5){
//										_delayType[l] = (listDelay.at((int) sort) & 0xff) << 8 | (3 & 0xff);
//									}
//									else{
										_delayType[l] = (listDelay.at((int) sort) & 0xff) << 8 | (2 & 0xff);
//									}								
								}
								else{
//									if (rand0::next() < 0.27){
//										_delayType[l] = (listDelay.at((int) sort) & 0xff) << 8 | (1 & 0xff);
//									}
//									else{
										_delayType[l] = (listDelay.at((int) sort) & 0xff) << 8 | (0 & 0xff);
//									}
								}
								
								if(mtxPre[num].posSyn != NULL)
									delete[] mtxPre[num].posSyn;
								if(mtxPre[num].numConn != NULL)
									delete[] mtxPre[num].numConn;
								if(mtxPre[num].delayType != NULL)
									delete[] mtxPre[num].delayType;
								mtxPre[num].posSyn = _posSyn;
								mtxPre[num].numConn = _numConn;
								mtxPre[num].delayType = _delayType;
							}
							else
								mtxPre[num].numConn[aux]++;				
						}
						listNeuron.clear();	
						listDelay.clear();
					}					
				}	
			}
		}	
	}	
	
	synapse * syn;	
	neuron * cell;
	count = 0;
	for (i = 0; i < nneuron; i++){
		type = mtxPre[i].type;
		if (type == 0){
			cell = new lscell();
		}
		else if (type == 2 || type == 7 || type == 11 || type == 15){
			cell = new fscell();
		}
		else if (type == 3 || type == 8 || type == 12 || type == 16){
			cell = new ltscell();
		}
		else{
			cell = new izhiCom();
		}
		cell->setId(i);
		listn->push_back(cell);
		count++;
	}	
	
	float gmax = 0.0;
	int idx; 
	for (i = 0; i < nneuron; i++){
		for(j = 0; j < mtxPre[i].length; j++){
			idx = mtxPre[i].posSyn[j];
			if ((mtxPre[i].delayType[j] & 0xff) == 0 || (mtxPre[i].delayType[j] & 0xff) == 1)
				gmax = gexc;
			else
				gmax = gin;
			syn = new synapse((mtxPre[i].delayType[j] & 0xff), gmax, (mtxPre[i].delayType[j] & 0xff00) >> 8, false, false);
			(listn->at(i))->makeconnection(listn->at(idx), syn);
		}
	}	
	
	return;
}

int simulation::search(preNeuron * mtxPre, int num, int delay){
	for (int i = 0; i < mtxPre->length; i++){
		if(mtxPre->posSyn[i] == num && ((mtxPre->delayType[i]  & 0xff00) >> 8) == delay)
			return i;
	}
	return -1;
}  

void simulation::setSeed (long seed) {
	rand0::setSeed(seed);
} 

void simulation::setGin(float gin) {
	this->gin = gin;
}

void simulation::setGexc(float gexc) {
	this->gexc = gexc;	
}

void simulation::connData(string name){
	FILE * pfile;
	pfile = fopen(name.c_str(), "a");	
	for (int i = 0; i < nneuron; i++){
		for (int j = 0; j < mtxPre[i].length; j++){
			fprintf(pfile, "%d\t%d\t%d\t%d\n", i, mtxPre[i].posSyn[j], (mtxPre[i].delayType[j] & 0xff), (mtxPre[i].delayType[j] & 0xff00) >> 8);
		}
	}
	fclose(pfile);
}
   
void simulation::rasterdata(string name) {
    ofstream fout, fplot;
    fout.open(name.c_str());
    int i, j, k, max_size = 0;
    
    for (i = 0; i < listn->size(); i++)
        if (!listn->at(i)->getevents().empty())
        	if (listn->at(i)->getevents().size() > max_size)
        		max_size = listn->at(i)->getevents().size();    
	
    for (i = 0; i < listn->size(); i++) {
        if (!listn->at(i)->getevents().empty()) {
        	fout << i << "\t";
            for (j = 0; j < listn->at(i)->getevents().size(); j++) {
                fout << listn->at(i)->getevents().at(j) << "\t";
            }
            if (listn->at(i)->getevents().size() < max_size){
            	k = max_size - listn->at(i)->getevents().size();
            	for (j = 0;j < k; j++)
            		fout << 0.0 << "\t";	
            }
            fout << "\n";
        }
    }
}

void simulation::printTime(string name){
	FILE * pfile;
	pfile = fopen(name.c_str(), "a");
	neuron * nrn;
	double tmp = 0.0, tmp2 = 0.0, aux = 0.0, last = 0.0, std = 0.0;
	for (int i = 0; i < nneuron; i++){
		nrn = listn->at(i);
		if ((nrn->getevents()).empty()) continue;
		aux = (nrn->getevents()).back();
		tmp += aux;
		tmp2 += aux*aux;
		if (aux > last) last = aux;
	}
	tmp = tmp/(listn->size());
	tmp2 = tmp2/(listn->size());
	std = sqrt(tmp2 - tmp*tmp);
	fprintf(pfile,"%lf\t%lf\t%lf\t%lf\t%lf\n",gin, gexc, last, tmp, std);
	fclose(pfile);
}  	

void simulation::printAvgFreq(string name){
	FILE * pfile;
	pfile = fopen(name.c_str(), "a");
	neuron * nrn;
	double tmp = 0.0, tmp2 = 0.0, aux = 0.0, std = 0.0;
	int count = 0;
	for (int i = 0; i < nneuron; i++){
		nrn = listn->at(i);
		if ((nrn->getevents()).size() < 2) continue;
		for (int j = 1; j < (nrn->getevents()).size(); j++){
			aux = 1000/((nrn->getevents()).at(j) - (nrn->getevents()).at(j-1));
			tmp += aux;
			tmp2 += aux*aux;
			count++;		
		}
	}
	if (count > 0){
		tmp = tmp/count;
		tmp2 = tmp2/count;
		std = sqrt(tmp2 - tmp*tmp);
	}
	fprintf(pfile,"%lf\t%lf\t%lf\t%lf\n",gin, gexc, tmp, std);
	fclose(pfile);
}  