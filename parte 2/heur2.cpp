#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

#define TRUE 1
#define FALSE 0

using namespace std;

/* defini��o das structs*/
struct shard{
    int posH;            //Posi��o Horizontal
    int posV;            //Posi��o Vertical
    int rShard;        //Ganho de Armazenagem
    int cH;            //Custo de armazenagem pelo satelite em rota horizontal
    int cV;            //Custo de armazenagem pelo satelite em rota vertical
    int lidaPor;
    bool lida;
};

struct satelite{
    int ns;            //Numero do satelite
    int memTotal;      //Memoria total do satelite
    int memRestante;   //Memoria restante do satelite
};


/* impress�es para teste */
void imprimeSat(vector<satelite>& sat){
     
    cout << "SATELITES: " << endl;
    cout << "Num,PosVetor,MemTotal, MemRest" << endl;
    int n = sat.size();
    for(int i=0;i<n;i++){
        cout << sat[i].ns << "," << i << "," << sat[i].memTotal << "," << sat[i].memRestante << endl;
    }    
    cout << endl;
}
void imprimeResp(vector<int>& resp, vector<shard>& shard) {
     cout << "Vetor de respostas:" << endl;
     cout << "RespShard,RespSat,SPosH,SPosV,LidaPor" << endl;
     for (int i=0; i<shard.size(); i++) {
         cout << i << "," << resp[i] << "," << shard[i].posH << "," << shard[i].posV << "," << shard[i].lidaPor << endl;
     }
} 
void imprimeMapSat( map<int,satelite>& satMap){
    map<int,satelite>::iterator it; 
    cout << "SATELITES: " << endl;
    cout << "Num,MemTotal, MemRest" << endl;
    
    for ( it=satMap.begin() ; it != satMap.end(); it++ ){
        cout << it->second.ns << "," << it->second.memTotal << "," << it->second.memRestante << endl;
    }    
    cout << endl;
}         
void imprimeShard(vector<shard>& shard){
     
    int n = shard.size();   
    cout << "SHARDS: " << endl;
    cout << "PosVetor,ShardH,ShardV,Ganho,CustoH,CustoV,Lida,LidaPor" << endl;
    
    for(int i=0;i<n;i++){
        cout << i << "," << shard[i].posH << "," << shard[i].posV << "," << shard[i].rShard;
        cout << "," << shard[i].cH << "," << shard[i].cV << "," << shard[i].lida << "," << shard[i].lidaPor << endl;
    }
    cout << endl;
}/* impress�es para teste */

/* fun��es auxiliares de ordena��o */
bool aZSat(satelite sat1, satelite sat2){
    return sat1.memRestante < sat2.memRestante;
}

bool compShard(shard shard1, shard shard2){
    return max((float)shard1.rShard/(float)shard1.cH, (float)shard1.rShard/(float)shard1.cV) 
           > max((float)shard2.rShard/(float)shard2.cH, (float)shard2.rShard/(float)shard2.cV);
}

/* A fun��o abre o arquivo de entrada, l� os par�metros necess�rios, montando vetores de estruturas de satelites e
   vetores de estruturas de shards 
   */
int readIn(vector<shard>& obj, vector<satelite>& satH, map<int,satelite>& satMap, char* entrada){
    ifstream arqin(entrada);
    int n, m, seq, totalReward = 0, Collected = 0;
    string tok, line;
    stringstream iss;

    satelite sat;
    shard sh;

    getline(arqin,tok);                      //linha que cont�m o n�mero de sat�lites de cada cjto
    n = atoi(tok.c_str());                   //transforma o n�mero retirado em um int
    getline(arqin,line);
    iss.str(line);                           //atribui a ultima linha para um stringstream iss
   
    for(int i=0;i<n;i++){                        //Monta o vetor de satelites a partir da linha lida
        getline(iss,tok,' ');
        getline(iss,tok,' ');
        sat.memTotal = atoi(tok.c_str());
        sat.memRestante = sat.memTotal;
        sat.ns = 1 + i;
        satH.insert(satH.end(),sat);
        satMap.insert(pair<int,satelite>(sat.ns, sat));

    }

    iss.clear();                             //limpa o stringstream para colocar nele uma nova linha
    getline(arqin,line);
    iss.str(line);
    for(int i=0;i<n;i++){
        getline(iss,tok,' ');
        getline(iss,tok,' ');
        sat.memTotal = atoi(tok.c_str());
        sat.memRestante = sat.memTotal;
        sat.ns = 1 + i + n;
        satH.insert(satH.end(),sat);
        satMap.insert(pair<int,satelite>(sat.ns, sat));
    }

    getline(arqin,tok);                      //linha que cont�m o n�mero de shards
    m = atoi(tok.c_str());
    for(int i=0;i<m;i++){
        iss.clear();
        getline(arqin,line);
        iss.str(line);
        getline(iss,tok,' ');
        seq = atoi(tok.c_str());
        sh.posH = seq;
        getline(iss,tok,' ');
        seq = atoi(tok.c_str());
        sh.posV = seq + n;
        getline(iss,tok,' ');
        seq = atoi(tok.c_str());
        sh.rShard = seq;
        totalReward += seq;
        getline(iss,tok,' ');
        seq = atoi(tok.c_str());
        sh.cH = seq;
        getline(iss,tok,' ');
        seq = atoi(tok.c_str());
        sh.cV = seq;
        sh.lida = false;
        sh.lidaPor = -1;
        obj.insert(obj.end(),sh);
    
    }

    return totalReward;
} /* readIn */

/* heur�stica de aloca��o de shards aos sat�lites
   preenche o espa�o dispon�vel em cada sat�lite com
   as shards ainda n�o lidas.
   preenche primeiro os sat�lites com menor espa�o 
   de mem�ria disponivel
   TEM QUE LIMPAR OS couts DE TESTE
   */
int build(vector<shard>& shard, vector<satelite>& sat, vector<int>& resp, int toCollect){    
    int nSat = sat.size();
    int nSh = shard.size();
    
    resp.resize(nSh);

    for(int i = 0; i < nSat; i++){
        for(int j = 0; j < nSh; j++){
            if(shard[j].posH == sat[i].ns && shard[j].lida == false){
                if(sat[i].memRestante >= shard[j].cH){   
                    shard[j].lida = true;
                    shard[j].lidaPor = i;
                    sat[i].memRestante -=  shard[j].cH;
                    toCollect -= (int) shard[j].rShard;
                    resp[j] = sat[i].ns;
                }
            }
		
            if(shard[j].posV == sat[i].ns && shard[j].lida == false){
                if(sat[i].memRestante >= shard[j].cV){
                    shard[j].lida = true;
                    shard[j].lidaPor = i;
                    sat[i].memRestante -=  shard[j].cV;
                    toCollect -= (int) shard[j].rShard;
                    resp[j] = sat[i].ns;
                }
            }
        }  
    }
    
    return toCollect;
}/* build */


int build_shards(vector<shard>& obj, map<int,satelite>& satMap, int toCollect){
    int memH, memV, compH, compV;
    map<int,satelite>::iterator it;
    
    
    //cout << endl << "BUILD PELAS SHARDS" << endl << endl;

    //cout << "Srd.PosH,Srd.posV,Srd.cH,Srd.cV,stH.ns,stH.mRest,stV.ns,stV.mRest" << endl;
    
    for(int k = 0; k < obj.size();k++){
        it = satMap.find(obj[k].posH);
        memH = it->second.memRestante;
        
        it = satMap.find(obj[k].posV);
        memV = it->second.memRestante;

        /*cout << obj[k].posH << "," << obj[k].posV << "," << obj[k].cH << ",";
        cout << obj[k].cV << ","  << iH->second.ns << ",";
        cout << iH->second.memRestante << "," << iV->second.ns << "," << iV->second.memRestante << endl;
          */      
                
        compH = memH - obj[k].cH;
        compV = memV - obj[k].cV;
        
        if(compH > 0 && compH > compV){
            it = satMap.find(obj[k].posH);     
            it->second.memRestante -= obj[k].cH;
            toCollect -= obj[k].rShard;
            obj[k].lida = true;
            obj[k].lidaPor = it->second.ns;                     
        }
        else{ 
            if(compV >= compH){
                it = satMap.find(obj[k].posV);     
                it->second.memRestante -= obj[k].cV;
                toCollect -= obj[k].rShard;
                obj[k].lida = true;
                obj[k].lidaPor = it->second.ns;                     
            }
        }
    }
    return toCollect;

}/* buildShards */


void volta1(vector<shard>& shard, vector<satelite>& sat, vector<int>& resp, int * toBeCollected) {
     
    // percorre os sat�lites na sequencia inversa da ordena��o inicial (maior para menor)            
    for(int i=sat.size()-1 ;i>=0 ;i--){
            // percorre as shards           
            for(int j=0; j<=shard.size()-1; j++){
                    //se for "l�vel" pelo sat�lite e tiver sido lida por um menor
                    if((shard[j].posH == sat[i].ns || shard[j].posV == sat[i].ns) && shard[j].lidaPor < i ){
                                      
                            //sat�lite "vertical", tem espa�o para a shard: faz a troca       
                            if(sat[i].ns >  sat.size()/2 && shard[j].cV <= sat[i].memRestante) {
                                 if(shard[j].lida == true)   
                                     sat[shard[j].lidaPor].memRestante += shard[j].cH;
                                 else *toBeCollected -= shard[j].rShard;
                                 shard[j].lidaPor = i;
                                 shard[j].lida = true;
                                 resp[j] = sat[i].ns;
                                 sat[i].memRestante -= shard[j].cV;
                            }
                            //sat�lite "horizontal", idem acima
                            if (sat[i].ns <= sat.size()/2 && shard[j].cH <= sat[i].memRestante){

                                  if(shard[j].lida == true)        
                                      sat[shard[j].lidaPor].memRestante += shard[j].cV;
                                  else *toBeCollected -= shard[j].rShard;
                                  shard[j].lidaPor = i;
                                  shard[j].lida = true;
                                  resp[j] = sat[i].ns;
                                  sat[i].memRestante -= shard[j].cH;

                            }
                    }
            }
    }             
}

/*void trocaShards(vector<shard>& shard, vector<sat>& sat){
    
    for(int i=0;i<shard.size();i++){
        if(shard.lida == true && sat[shard[i].lidaPor].ns <= sat.size()/2 )
    return;
}*/


int main(int argc, char* argv[]){
    int i, n, totalReward, toBeCollected, previousToBeCollected, loop;
    vector<shard> shard;
    vector<satelite> sat;
    vector<int> resp;
    map<int,satelite> satMap;
    map<int,satelite>::iterator it;

    //int changed = TRUE;

    totalReward = readIn(shard, sat, satMap, argv[1]);
    toBeCollected = totalReward;
    
    //sort(sat.begin(),sat.end(),aZSat);
    //sort(shard.begin(), shard.end(), compShard);

    // TESTE
    cout << "Leitura:" << endl;
    //imprimeMapSat(satMap);
    //imprimeShard(shard);
    cout << "toBeColl:," << toBeCollected << endl;
    
    loop = 0;

    previousToBeCollected = toBeCollected;
    
    
    //toBeCollected = build(shard, sat, resp, toBeCollected);
    toBeCollected = build_shards(shard, satMap, toBeCollected);
    
    
    //imprimeResp(resp, shard);  
          
    //TESTE
    cout << "Ida n.: " << loop << endl;
    //imprimeMapSat(satMap);
    //imprimeShard(shard);
    cout << "toBeColl:," << toBeCollected << endl;
/*        
    while(toBeCollected && toBeCollected < previousToBeCollected) {
          
                  
          // se tem coisa a coletar:          
          if(toBeCollected) {
                            
                 volta1(shard, sat, resp, &toBeCollected);
          }
          
          
          //TESTE
          cout << "Volta n.: " << loop << endl;
          //imprimeSat(sat);
          //imprimeShard(shard);
          cout << "toBeColl:," << toBeCollected << endl;
          
          toBeCollected = build(shard, sat, resp, toBeCollected);
          
          previousToBeCollected = toBeCollected;
          
          //TESTE
          cout << "Ida n.: " << loop << endl;
          //imprimeSat(sat);
          //imprimeShard(shard);
          cout << "toBeColl:," << toBeCollected << endl;
          
          loop++;  
    }                
*/    
    //imprimeResp(resp, shard);                   
    return 0;
 
}

