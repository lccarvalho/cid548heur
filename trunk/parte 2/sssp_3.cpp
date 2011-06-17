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
//???    vector<shard> shardsLidas;
};

/* funções auxiliares de ordenação */
bool aZSat(satelite sat1, satelite sat2){
    return sat1.memRestante < sat2.memRestante;
}

bool zASat(satelite sat1, satelite sat2){
    return sat1.memRestante > sat2.memRestante;
}

bool compShard(shard shard1, shard shard2){
    return max(shard1.rShard/shard1.cH, shard1.rShard/shard1.cV) > max(shard2.rShard/shard2.cH, shard2.rShard/shard2.cV);

}



/* heurística de alocação de shards aos satélites
   preenche o espaço disponível em cada satélite com
   as shards ainda não lidas.
   preenche primeiro os satélites com menor espaço 
   de memória disponivel
   TEM QUE LIMPAR OS couts DE TESTE
   */
int build(vector<shard>& obj, vector<satelite>& sat, int Objetivo){
    
    int nSat = sat.size()/2;
    int nSh = obj.size();
    
    //cout << endl << "BUILD PELOS SATELITES" << endl << endl;
//    cout << "ShardH ShardV Satelite TamH TamV MemRest" << endl;

    for(int i = 0; i < 2*nSat; i++){
        for(int j = 0; j < nSh; j++){
            if(obj[j].posH == sat[i].ns && obj[j].lida == false){
                if(sat[i].memRestante >= obj[j].cH){   
                    obj[j].lida = true;
                    obj[j].lidaPor = i;
                    sat[i].memRestante -=  obj[j].cH;
                    Objetivo -= obj[j].rShard;
//                    cout << obj[j].posH << " " << obj[j].posV << " ";
  //                  cout << sat[i].ns << " ";
    //                cout << obj[j].cH << " " << obj[j].cV << " ";
      //              cout << sat[i].memRestante << endl;           
                }
            }
		
            if(obj[j].posV == sat[i].ns && obj[j].lida == false){
                if(sat[i].memRestante >= obj[j].cV){
                    obj[j].lida = true;
                    obj[j].lidaPor = i;
//?????????         sat[i].shardsLidas.insert(shardsLidas.end(),obj[j]);
                    sat[i].memRestante -=  obj[j].cV;
                    Objetivo -= obj[j].rShard;
//                    cout << obj[j].posH << " " << obj[j].posV << " ";
  //                  cout << sat[i].ns << " ";
    //                cout << obj[j].cH << " " << obj[j].cV << " ";
      //              cout << sat[i].memRestante << endl;              

                }
            }
        }  
    }
    
    return Objetivo;

}/* build */


/* A função abre o arquivo de entrada, lê os parâmetros necessários, montando vetores de estruturas de satelites e
   vetores de estruturas de shards 
   MODIFICADA PARA COLOCAR EM shardV o n + numSat
   */
int readIn(vector<shard>& obj, vector<satelite>& satH,  char* entrada){
    ifstream arqin(entrada);
    int n, m, seq, totalReward = 0;
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
//        sh.lidaPor = -1;
        sh.lidaPor = 0;
        obj.insert(obj.end(),sh);
    
    }

    return totalReward;
} /* readIn */



int main(int argc, char* argv[]){
    ifstream entrada;
    ofstream saida;
    int i, nSh, nSat, totalReward, toBeCollected, lastCollected = 0;
    vector<shard> obj;
    vector<satelite> satH;
    //vector<satelite> satV;
    //vector<int> resp;
    int improved = TRUE;

    totalReward = readIn(obj, satH, argv[1]);
    toBeCollected = totalReward;
    
    sort(satH.begin(),satH.end(),aZSat);

    //impressões para teste
    nSat = satH.size();
    cout << "SATELITES: " << nSat << endl;
    cout << "Num,PosVetor,Memoria Total" << endl;
    for(i=0;i<nSat;i++){
        cout << satH[i].ns << "," << i << "," << satH[i].memRestante << endl;
    }
    
    cout << endl;
    

    sort(obj.begin(), obj.end(), compShard);
    
    //impressões para teste 
    nSh = obj.size();   
    cout << "SHARDS: " << nSh << endl;
    cout << "ShardH,ShardV,Ganho,CustoH,CustoV,MelhorCustoMemoria" << endl;
    
    for(i=0;i<nSh;i++){
        cout << obj[i].posH << "," << obj[i].posV << "," << obj[i].rShard;
        cout << "," << obj[i].cH << "," << obj[i].cV;
        cout << "," << max(obj[i].rShard/obj[i].cH, obj[i].rShard/obj[i].cV) << endl;
    }
    cout << endl;
    
    toBeCollected = build(obj, satH, toBeCollected);
    while(improved && toBeCollected) {
          improved = false;
          cout << "TOTALREWARD: " << totalReward << endl;
          cout << "TOBECOLLECTED: " << toBeCollected << endl;
          cout << "COLLECTED: " << totalReward - toBeCollected << endl << endl;
          // se tem coisa a coletar:
                // percorre os satélites na sequencia inversa da ordenação (maior para menor)
                   // percorre as shards
                      //se for "lível" pelo satélite e se tamanho < memRestante
                           //troca lida por para o satélite
                           //subtrai tamanho de memRestante
                           //soma o tamanho (na direção correta) no satélite que tinha lido antes
                           //improved = true                           
                    
          if(toBeCollected) {
                for(i=nSat ;i>0 ;i--){
                      for(int j=0; j<nSh; j++){
                              if(obj[j].posH == satH[i].ns || obj[j].posV == satH[i].ns){
                                      if(satH[i].ns >=  nSat/2 && obj[j].cV <= satH[i].memRestante) {
                                               satH[obj[j].lidaPor].memRestante += obj[j].cH;
                                               obj[j].lidaPor = i;
                                               satH[i].memRestante -= obj[j].cV;
                                      }
                                      if (satH[i].ns < nSat/2 && obj[j].cH <= satH[i].memRestante){
                                               satH[obj[j].lidaPor].memRestante += obj[j].cV;
                                               obj[j].lidaPor = i;
                                               satH[i].memRestante -= obj[j].cH;
                                      }
                              }
                      }
                }             
          
          }
          toBeCollected = build(obj, satH, toBeCollected);
          if(lastCollected < (totalReward - toBeCollected)) improved = true;
          else improved = false;
          lastCollected = totalReward - toBeCollected;
    }

    cout << endl <<"FINAL" << endl;
    cout << "[TOTALREWARD]: " << totalReward << endl;
    cout << "[TOBECOLLECTED]: " << toBeCollected << endl;
    cout << "[COLLECTED]: " << totalReward - toBeCollected << endl << endl;

    cout << endl << "Shards" << endl;
    cout << "ShardH,ShardV,Ganho,CustoH,CustoV,LidaPor" << endl;
    for(int i = 0; i < nSh; i++){
                 cout << obj[i].posH << "," << obj[i].posV << "," << obj[i].rShard;
                 cout << "," << obj[i].cH << "," << obj[i].cV;
                 cout << "," << satH[obj[i].lidaPor].ns << endl;
    }
    
    cout << endl << "MemoriaRestante" << endl;        
    cout << "Num,PosVetor,MemRestante" << endl;
    for(int i = 0;i < nSat;i++){
        cout << satH[i].ns << "," << i << "," << satH[i].memRestante << endl;
    }
    cout << endl << "Objetivo" << totalReward - toBeCollected << endl << endl;
                      
    return 0;
 
}

