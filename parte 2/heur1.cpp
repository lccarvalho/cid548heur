#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <time.h>

#define TRUE 1
#define FALSE 0

using namespace std;


/* defini��o das structs*/
struct shard{
    int posH;            //Posi��o Horizontal
    int posV;            //Posi��o Vertical
    float rShard;        //Ganho de Armazenagem
    float cH;            //Custo de armazenagem pelo satelite em rota horizontal
    float cV;            //Custo de armazenagem pelo satelite em rota vertical
    int lidaPor;
    bool lida;
};

struct satelite{
    int ns;            //Numero do satelite
    float memTotal;      //Memoria total do satelite
    float memRestante;   //Memoria restante do satelite
};

/* fun��es auxiliares de ordena��o */
bool aZSat(satelite sat1, satelite sat2){
    return sat1.memRestante < sat2.memRestante;
}

bool compShard(shard shard1, shard shard2){
    return max(shard1.rShard/shard1.cH, shard1.rShard/shard1.cV) > max(shard2.rShard/shard2.cH, shard2.rShard/shard2.cV);
}



/* heur�stica de aloca��o de shards aos sat�lites
   preenche o espa�o dispon�vel em cada sat�lite com
   as shards ainda n�o lidas.
   preenche primeiro os sat�lites com menor espa�o 
   de mem�ria disponivel
   TEM QUE LIMPAR OS couts DE TESTE
   */
int build(vector<shard>& shard, vector<satelite>& sat, int toCollect){
    
    int nSat = sat.size()/2;
    int nSh = shard.size();

    for(int i = 0; i < 2*nSat; i++){
        for(int j = 0; j < nSh; j++){
            if(shard[j].posH == sat[i].ns && shard[j].lida == false){
                if(sat[i].memRestante >= shard[j].cH){   
                    shard[j].lida = true;
                    shard[j].lidaPor = i;
                    sat[i].memRestante -=  shard[j].cH;
                    toCollect -= (int) shard[j].rShard;
                }
            }
		
            if(shard[j].posV == sat[i].ns && shard[j].lida == false){
                if(sat[i].memRestante >= shard[j].cV){
                    shard[j].lida = true;
                    shard[j].lidaPor = i;
                    sat[i].memRestante -=  shard[j].cV;
                    toCollect -= (int) shard[j].rShard;
                }
            }
        }  
    }
    
    return toCollect;

}/* build */


/* A fun��o abre o arquivo de entrada, l� os par�metros necess�rios, montando vetores de estruturas de satelites e
   vetores de estruturas de shards 
   */
int readIn(vector<shard>& obj, vector<satelite>& satH,  char* entrada){
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



int main(int argc, char* argv[]){
    int i, n, totalReward, toBeCollected, previousToBeCollected;
    float startTime, totalTime;
    vector<shard> shard;
    vector<satelite> sat;
    int changed = TRUE;

    totalReward = readIn(shard, sat, argv[1]);
    toBeCollected = totalReward;
    
    sort(sat.begin(),sat.end(),aZSat);

    //impress�es para teste
/*    cout << "SATELITES: " << endl;
    cout << "Num,PosVetor,Memoria Total" << endl;
    n = sat.size();
    for(i=0;i<n;i++){
        cout << sat[i].ns << "," << i << "," << sat[i].memRestante << endl;
    }
    
    cout << endl;
  */  

    sort(shard.begin(), shard.end(), compShard);
    
    //impress�es para teste 
    n = shard.size();   
/*    cout << "SHARDS: " << endl;
    cout << "ShardH,ShardV,Ganho,CustoH,CustoV" << endl;
  */  
    for(i=0;i<n;i++){
        cout << shard[i].posH << "," << shard[i].posV << "," << shard[i].rShard;
        cout << "," << shard[i].cH << "," << shard[i].cV << endl;
    }
    cout << endl;

    startTime = clock();
    
    toBeCollected = build(shard, sat, toBeCollected);
    
    totalTime = clock() - startTime;
/*          
          
          cout << "TOTALREWARD: " << totalReward << endl;
          cout << "TOBECOLLECTED: " << toBeCollected << endl;
          cout << "COLLECTED: " << totalReward - toBeCollected << endl;
          
          // se tem coisa a coletar:          
          if(toBeCollected) {
                // percorre os sat�lites na sequencia inversa da ordena��o inicial (maior para menor)            
                for(i=sat.size()-1 ;i>=0 ;i--){
                      // percorre as shards           
                      for(int j=0; j<shard.size(); j++){
                              //se for "l�vel" pelo sat�lite e tiver sido lida por um menor
                              if((shard[j].posH == sat[i].ns || shard[j].posV == sat[i].ns) &&
                                  shard[j].lidaPor < i ){
                                                   
                                      //sat�lite "vertical", tem espa�o para a shard: faz a troca       
                                      if(sat[i].ns >  sat.size()/2 && shard[j].cV <= sat[i].memRestante) {
                                               sat[shard[j].lidaPor].memRestante += shard[j].cH;
                                               shard[j].lidaPor = i;
                                               sat[i].memRestante -= shard[j].cV;
                                               changed = TRUE;
                                      }
                                      //sat�lite "horizontal", idem acima
                                      if (sat[i].ns <= sat.size()/2 && shard[j].cH <= sat[i].memRestante){
                                               sat[shard[j].lidaPor].memRestante += shard[j].cV;
                                               shard[j].lidaPor = i;
                                               sat[i].memRestante -= shard[j].cH;
                                               changed = TRUE;
                                      }
                              }
                      }
                }             
          
          }   
    }                
*/
    cout << endl << "Shards" << endl;
    cout << "ShardH,ShardV,Ganho,CustoH,CustoV,LidaPor" << endl;
    for(int i = 0; i < shard.size(); i++){
                 cout << shard[i].posH << "," << shard[i].posV << "," << shard[i].rShard;
                 cout << "," << shard[i].cH << "," << shard[i].cV;
                 cout << "," << shard[i].lidaPor << endl;
    }
    
    cout << endl << "Satelites" << endl;        
    cout << "Num,PosVetor,MemTotal,MemRestante" << endl;
    for(int i=0;i<sat.size();i++){
        cout << sat[i].ns << "," << i << "," << sat[i].memTotal << "," << sat[i].memRestante << endl;
    }
    
    cout << endl << "Satelites" << sat.size() << endl << endl;
    cout << endl << "Shards" << shard.size() << endl << endl;
    cout << endl << "Objetivo" << totalReward - toBeCollected << endl << endl;
    cout << endl << "Maximo" << totalReward << endl << endl;
    cout << endl << "Time" << totalTime << endl << endl;
    
    
                      
    return 0;
 
}

