#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#define TRUE 1
#define FALSE 0

using namespace std;


/* definição das structs*/
struct shard{
    int posH;            //Posição Horizontal
    int posV;            //Posição Vertical
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

/* funções auxiliares de ordenação */
bool aZSat(satelite sat1, satelite sat2){
    return sat1.memRestante < sat2.memRestante;
}

bool compShard(shard shard1, shard shard2){
    return max((float)shard1.rShard/(float)shard1.cH, (float)shard1.rShard/(float)shard1.cV) 
           > max((float)shard2.rShard/(float)shard2.cH, (float)shard2.rShard/(float)shard2.cV);
}

/* A função abre o arquivo de entrada, lê os parâmetros necessários, montando vetores de estruturas de satelites e
   vetores de estruturas de shards 
   */
int readIn(vector<shard>& obj, vector<satelite>& satH,  char* entrada){
    ifstream arqin(entrada);
    int n, m, seq, totalReward = 0, Collected = 0;
    string tok, line;
    stringstream iss;

    satelite sat;
    shard sh;

    getline(arqin,tok);                      //linha que contêm o número de satélites de cada cjto
    n = atoi(tok.c_str());                   //transforma o número retirado em um int
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

    getline(arqin,tok);                      //linha que contêm o número de shards
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

/* heurística de alocação de shards aos satélites
   preenche o espaço disponível em cada satélite com
   as shards ainda não lidas.
   preenche primeiro os satélites com menor espaço 
   de memória disponivel
   TEM QUE LIMPAR OS couts DE TESTE
   */
int build(vector<shard>& shard, vector<satelite>& sat, int toCollect){    
    int nSat = sat.size();
    int nSh = shard.size();

    for(int i = 0; i < nSat; i++){
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

/* impressões para teste */
void imprimeSat(vector<satelite>& sat){
     
    cout << "SATELITES: " << endl;
    cout << "Num,PosVetor,MemTotal, MemRest" << endl;
    int n = sat.size();
    for(int i=0;i<n;i++){
        cout << sat[i].ns << "," << i << "," << sat[i].memTotal << "," << sat[i].memRestante << endl;
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
}/* impressões para teste */




int main(int argc, char* argv[]){
    int i, n, totalReward, toBeCollected, previousToBeCollected, loop;
    vector<shard> shard;
    vector<satelite> sat;
    int changed = TRUE;

    totalReward = readIn(shard, sat, argv[1]);
    toBeCollected = totalReward;
    
    sort(sat.begin(),sat.end(),aZSat);
    sort(shard.begin(), shard.end(), compShard);

    // TESTE
    cout << "Leitura:" << endl;
    imprimeSat(sat);
    imprimeShard(shard);
    cout << "toBeColl:," << toBeCollected << endl;
    
    loop = 0;
        
    while(changed && toBeCollected && toBeCollected < previousToBeCollected) {
          changed = FALSE;
          previousToBeCollected = toBeCollected;
          
          toBeCollected = build(shard, sat, toBeCollected);
          
          //TESTE
          cout << "Ida n.: " << loop << endl;
          imprimeSat(sat);
          imprimeShard(shard);
          cout << "toBeColl:," << toBeCollected << endl;
                  
          // se tem coisa a coletar:          
          if(toBeCollected) {
                // percorre os satélites na sequencia inversa da ordenação inicial (maior para menor)            
                for(i=sat.size()-1 ;i>=0 ;i--){
                      // percorre as shards           
                      for(int j=0; j<shard.size(); j++){
                              //se for "lível" pelo satélite e tiver sido lida por um menor
                              if((shard[j].posH == sat[i].ns || shard[j].posV == sat[i].ns) &&
                                  shard[j].lidaPor < i ){
                                      
                                     /* cout << "i: " << i << " j: " << j << " sat[i].ns: " 
                                           << sat[i].ns << " shard[j].posH: "
                                           << shard[j].posH << " shard[j].posV: "
                                           << shard[j].posV << "  shard[j].lidaPor: "
                                           << shard[j].lidaPor << endl;
                                           system("pause");                                 
                                       */            
                                      //satélite "vertical", tem espaço para a shard: faz a troca       
                                      if(sat[i].ns >  sat.size()/2 && shard[j].cV <= sat[i].memRestante) {
                                              /*     
                                               cout << "shard[" << j << "] sai de sat[" << shard[j].lidaPor
                                               << "] para sat[" << i << "]" << endl;
                                               cout << "posH: " << shard[j].posH << " posV: " << shard[j].posV << endl;
                                               cout << "antigo: " << sat[shard[j].lidaPor].ns << " novo: " <<
                                               sat[i].ns << endl;
                                               
                                               cout << "mem. antigo - antes: " << sat[shard[j].lidaPor].memRestante
                                               << " depois: " << sat[shard[j].lidaPor].memRestante + shard[j].cH << endl;
                                               system("pause");
                                               */
                                               if(shard[j].lida == true)   
                                                       sat[shard[j].lidaPor].memRestante += shard[j].cH;
                                               shard[j].lidaPor = i;
                                               shard[j].lida = true;
                                               sat[i].memRestante -= shard[j].cV;
                                               changed = TRUE;
                                      }
                                      //satélite "horizontal", idem acima
                                      if (sat[i].ns <= sat.size()/2 && shard[j].cH <= sat[i].memRestante){
                                               /*     
                                               cout << "shard[" << j << "] sai de sat[" << shard[j].lidaPor
                                               << "] para sat[" << i << "]" << endl;
                                               cout << "posH: " << shard[j].posH << " posV: " << shard[j].posV << endl;
                                               cout << "antigo: " << sat[shard[j].lidaPor].ns << " novo: " <<
                                               sat[i].ns << endl;
                                               system("pause");
                                               */
                                               if(shard[j].lida == true)        
                                                       sat[shard[j].lidaPor].memRestante += shard[j].cV;
                                               shard[j].lidaPor = i;
                                               shard[j].lida = true;
                                               sat[i].memRestante -= shard[j].cH;
                                               changed = TRUE;
                                      }
                              }
                      }
                }             
          
          }
          //TESTE
          cout << "Volta n.: " << loop << endl;
          imprimeSat(sat);
          imprimeShard(shard);
          cout << "toBeColl:," << toBeCollected << endl; 
          
          loop++;  
    }                
                      
    return 0;
 
}

