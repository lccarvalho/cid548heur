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
    int memTotal;      //Memoria total do satelite
    int memRestante;   //Memoria restante do satelite
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
    cout << "ShardH ShardV Satelite TamH TamV MemRest" << endl;

    for(int i = 0; i < 2*nSat; i++){
        for(int j = 0; j < nSh; j++){
            if(obj[j].posH == sat[i].ns && obj[j].lida == false){
                if(sat[i].memRestante >= obj[j].cH){   
                    obj[j].lida = true;
                    obj[j].lidaPor = i;
//???????????       sat[i].shardsLidas.insert(shardsLidas.end(),obj[j]);
                    sat[i].memRestante -= (int) obj[j].cH;
                    Objetivo -= (int) obj[j].rShard;
                    cout << obj[j].posH << " " << obj[j].posV << " ";
                    cout << sat[i].ns << " ";
                    cout << obj[j].cH << " " << obj[j].cV << " ";
                    cout << sat[i].memRestante << endl;           
                }
            }
		
            if(obj[j].posV == sat[i].ns && obj[j].lida == false){
                if(sat[i].memRestante >= obj[j].cV){
                    obj[j].lida = true;
                    obj[j].lidaPor = i;
//?????????         sat[i].shardsLidas.insert(shardsLidas.end(),obj[j]);
                    sat[i].memRestante -= (int) obj[j].cV;
                    Objetivo -= (int) obj[j].rShard;
                    cout << obj[j].posH << " " << obj[j].posV << " ";
                    cout << sat[i].ns << " ";
                    cout << obj[j].cH << " " << obj[j].cV << " ";
                    cout << sat[i].memRestante << endl;              

                }
            }
        }  
    }
    
    cout << endl << "ShardsNaoLidas" << endl;
    cout << "ShardH ShardV  Ganho: CustoH:  CustoV: MelhorCustoMemoria: " << endl;
    for(int i = 0; i < nSh; i++){
            if(obj[i].lida == false){
                 cout << obj[i].posH << " " << obj[i].posV << "  " << obj[i].rShard;
                 cout << " " << obj[i].cH << " " << obj[i].cV;
                 cout << " " << max(obj[i].rShard/obj[i].cH, obj[i].rShard/obj[i].cV) << endl;
            }
    }
    
    cout << endl << "MemoriaRestante" << endl;        
    cout << "Num PosVetor:  MemRestante: " << endl;

    for(int i=0;i<nSat;i++){
        cout << sat[i].ns << " " << i << " " << sat[i].memRestante << endl;
    }
    cout << endl << "Objetivo: " << Objetivo << endl << endl;
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
        sh.lidaPor = -1;
        obj.insert(obj.end(),sh);
    
    }

    return totalReward;
} /* readIn */



int main(int argc, char* argv[]){
    ifstream entrada;
    ofstream saida;
    int i, n, totalReward, toBeCollected;
    vector<shard> obj;
    vector<satelite> satH;
    //vector<satelite> satV;
    //vector<int> resp;
    int improved = TRUE;

    totalReward = readIn(obj, satH, argv[1]);
    toBeCollected = totalReward;
    
    sort(satH.begin(),satH.end(),aZSat);

    //impressões para teste
    cout << "SATELITES: " << endl;
    cout << "Num PosVetor:  Memoria Total: " << endl;
    n = satH.size();
    for(i=0;i<n;i++){
        cout << satH[i].ns << " " << i << " " << satH[i].memRestante << endl;
    }
    
    cout << endl;
    

    sort(obj.begin(), obj.end(), compShard);
    
    //impressões para teste 
    n = obj.size();   
    cout << "SHARDS: " << endl;
    cout << "ShardH ShardV  Ganho: CustoH:  CustoV: MelhorCustoMemoria: " << endl;
    
    for(i=0;i<n;i++){
        cout << obj[i].posH << " " << obj[i].posV << "  " << obj[i].rShard;
        cout << " " << obj[i].cH << " " << obj[i].cV;
        cout << " " << max(obj[i].rShard/obj[i].cH, obj[i].rShard/obj[i].cV) << endl;
    }
    cout << endl;
    
    
    while(improved && toBeCollected) {
          improved = false;         
          toBeCollected = build(obj, satH, toBeCollected);
          
          // se tem coisa a coletar:
                // ordena os satélites em ordem decrescente de memRestante
                // puxa o quanto der de shards de outros satélites para este
                        //troca o lidoPor e atualiza as capacidades dos satélites
                        // se puxar alguma coisa fala que improved = true
                //ordena de novo os satélites da menor para a maior capacidade
          if(toBeCollected) {
                sort(satH.begin(),satH.end(),zASat);
                for(i=0;i<n;i++){
                      for(int j=i+1; j<n; j++){
                              //SATÉLITE i ROUBA QUANTAS SHARDS DER DE j 
                              //PRECISA COLOCAR UM PONTEIRO PARA AS SHARDS LIDAS NO SATÉLITE
                      }
                }             
          
          }   
          //COMO RECUPERAR A ORDEM ORIGINAL DOS SATÉLITES ?????         
    }                
                
          
          

    return 0;
}

