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

/* definição das structs*/
struct shard{
    int posH;          //Posição Horizontal
    int posV;          //Posição Vertical
    int rShard;        //Ganho de Armazenagem
    int cH;            //Custo de armazenagem pelo satelite em rota horizontal
    int cV;            //Custo de armazenagem pelo satelite em rota vertical
    int lidaPorNs;     //Número original do satélite que leu a shard
    int lidaPorPos;    //Posição no vetor de satélites do satélite que leu a shard
    bool lida;         //Indica se a shard foi lida ou não
};

struct satelite{
    int ns;            //Numero do satelite
    int memTotal;      //Memoria total do satelite
    int memRestante;   //Memoria restante do satelite
    int cover;         //Marca o número de shards que o satélite cobre
};


/* impressões para teste */
void imprimeSat(vector<satelite>& sat){
    int n = sat.size();
   
    cout << "SATELITES: " << endl;
    cout << "Num,PosVetor,MemTotal, MemRest,Cobertas" << endl;
    for(int i=0;i<n;i++){
        cout << sat[i].ns << "," << i << "," << sat[i].memTotal << "," 
             << sat[i].memRestante << "," << sat[i].cover << endl;
    }    
    cout << endl;
}

void imprimeResp(vector<int>& resp, vector<shard>& shard, int nSat, int total, int rdShards, char* saida) {
     ofstream arqout(saida);
     int n = resp.size();

     arqout << total << endl;
     arqout << rdShards << endl;
     for (int i=0;i<n;i++) {
         if(resp[i] != -1){
             arqout << shard[i].posH << " " << shard[i].posV;
             if(shard[i].lidaPorNs > nSat/2) arqout << " v" << endl;
             else arqout << " h" << endl;
         }
     }
     arqout.close();

}
                 
void imprimeShard(vector<shard>& shard, int totalShards){
    int n = shard.size();   
    
    cout << "SHARDS: "<< totalShards << endl;
    cout << "PosVetor,ShardH,ShardV,Ganho,CustoH,CustoV,Lida,LidaPor Posição,Lidapor Numero" << endl;    
    for(int i=0;i<n;i++){
        cout << i << "," << shard[i].posH << "," << shard[i].posV << "," << shard[i].rShard;
        cout << "," << shard[i].cH << "," << shard[i].cV << "," << shard[i].lida << "," << shard[i].lidaPorPos
        << "," << shard[i].lidaPorNs << endl;
    }
    cout << endl;
}/* impressões para teste */



/* funções auxiliares de ordenação */
bool aZSat(satelite sat1, satelite sat2){    
    return sat1.memRestante < sat2.memRestante;
}

bool compShard(shard shard1, shard shard2){
    return max((float)shard1.rShard/(float)shard1.cH, (float)shard1.rShard/(float)shard1.cV) 
           > max((float)shard2.rShard/(float)shard2.cH, (float)shard2.rShard/(float)shard2.cV);
}
bool compShard2(shard shard1, shard shard2){
    return shard1.rShard > shard2.rShard;
}



/* A função abre o arquivo de entrada, lê os parâmetros necessários, montando vetores de estruturas de satelites e
   vetores de estruturas de shards 
*/
int readIn(vector<shard>& shards, vector<satelite>& satelites, map<int,int>& satMap, char* entrada){
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
        sat.cover = 0;
        satelites.insert(satelites.end(),sat);
        satMap.insert(pair<int,int>(sat.ns, i));

    }

    iss.clear();                             //limpa o stringstream para colocar nele uma nova linha
    getline(arqin,line);
    iss.str(line);
    for(int i=0;i<n;i++){                    //leitura dos satélites
        getline(iss,tok,' ');
        getline(iss,tok,' ');
        sat.memTotal = atoi(tok.c_str());
        sat.memRestante = sat.memTotal;
        sat.ns = 1 + i + n;
        sat.cover = 0;
        satelites.insert(satelites.end(),sat);
        satMap.insert(pair<int,int>(sat.ns, i+n));
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
        getline(iss,tok,' ');
        seq = atoi(tok.c_str());
        sh.cH = seq;
        getline(iss,tok,' ');
        seq = atoi(tok.c_str());
        sh.cV = seq;
        sh.lida = false;
        sh.lidaPorPos = -1;
        sh.lidaPorNs = -1;
        
        // Se a shard cabe em pelo menos um dos satélites que a cobre, soma às shards cobertas pelos satélites leitores,
        // adiciona o custo da shard ao total e insere ela no vetor de shards 
        /*if((satelites[satMap.find(sh.posH)->second].memTotal > sh.cH) ||
           (satelites[satMap.find(sh.posV)->second].memTotal > sh.cV)    ) {*/
              
              satelites[satMap.find(sh.posH)->second].cover++;    
              satelites[satMap.find(sh.posV)->second].cover++;                                      
              shards.insert(shards.end(),sh);
              totalReward += sh.rShard;
        //}
        
    }
    arqin.close();

    return totalReward;
} /* readIn */

/* heurística de alocação de shards aos satélites
   preenche o espaço disponível em cada satélite com
   as shards ainda não lidas.
   preenche primeiro os satélites com menor espaço 
   de memória disponivel
   TEM QUE LIMPAR OS couts DE TESTE
   */
int build(vector<shard>& shard, vector<satelite>& sat, vector<int>& resp, int toCollect, int * rdShards){    
    int nSat = sat.size();
    int nSh = shard.size();    
    resp.resize(nSh,-1);

    for(int i = 0; i < nSat; i++){
        for(int j = 0; j < nSh; j++){
            if(shard[j].posH == sat[i].ns && shard[j].lida == false){
                if(sat[i].memRestante >= shard[j].cH){   
                    shard[j].lida = true;
                    shard[j].lidaPorPos = i;
                    shard[j].lidaPorNs = sat[i].ns;
                    sat[i].memRestante -=  shard[j].cH;
                    toCollect -= (int)shard[j].rShard;
                    *rdShards = *rdShards + 1;
                    resp[j] = sat[i].ns;
                }
            }
            
            if(shard[j].posV == sat[i].ns && shard[j].lida == false){
                if(sat[i].memRestante >= shard[j].cV){
                    shard[j].lida = true;
                    shard[j].lidaPorPos = i;
                    shard[j].lidaPorNs = sat[i].ns;
                    sat[i].memRestante -=  shard[j].cV;
                    toCollect -= (int)shard[j].rShard;
                    *rdShards = *rdShards + 1;
                    resp[j] = sat[i].ns;
                }
            }
        }  
    }
    
    return toCollect;
}/* build */


void volta1(vector<shard>& shard, vector<satelite>& satelites, vector<int>& resp, int * toBeCollected, int * rdShards) {
     int nSat = satelites.size();
     int nSh = shard.size();
     
    //percorre os satélites na sequencia inversa da ordenação inicial (maior para menor)            
    for(int i=nSat-1 ;i>=0 ;i--){
        //percorre as shards
        for(int j=0; j<=nSh-1; j++){
        
            //se for "lível" pelo satélite e tiver sido lida por um de menos memória
            if((shard[j].posH == satelites[i].ns || shard[j].posV == satelites[i].ns) && shard[j].lidaPorPos < i ){
                                               
                //satélite "vertical", tem espaço para a shard: faz a troca       
                if(satelites[i].ns >  nSat/2 && shard[j].cV <= satelites[i].memRestante) {
                
                    if(shard[j].lida == true) satelites[shard[j].lidaPorPos].memRestante += shard[j].cH;
                    else{ 
                        *toBeCollected -= shard[j].rShard;
                        *rdShards = *rdShards + 1;
                    }
                    shard[j].lidaPorPos = i;
                    shard[j].lidaPorNs = satelites[i].ns;
                    shard[j].lida = true;
                    resp[j] = satelites[i].ns;
                    satelites[i].memRestante -= shard[j].cV;
                }
                
                //satélite "horizontal", idem acima
                if (satelites[i].ns <= nSat/2 && shard[j].cH <= satelites[i].memRestante){

                    if(shard[j].lida == true) satelites[shard[j].lidaPorPos].memRestante += shard[j].cV;
                    else{
                        *toBeCollected -= shard[j].rShard;
                        *rdShards = *rdShards + 1;
                    }
                        shard[j].lidaPorPos = i;
                        shard[j].lidaPorNs = satelites[i].ns;
                        shard[j].lida = true;
                        resp[j] = satelites[i].ns;
                        satelites[i].memRestante -= shard[j].cH;

                            }
                    }
            }
    }             
}

int main(int argc, char* argv[]){
    int totalReward, toBeCollected, previousToBeCollected, loop = 0, rdShards = 0, TIME;
    vector<shard> shard;
    vector<satelite> satelites;
    vector<int> resp;
    map<int,int> satMap;

    totalReward = readIn(shard, satelites, satMap, argv[5]);
    toBeCollected = totalReward;
 
    sort(satelites.begin(),satelites.end(),aZSat);
    sort(shard.begin(), shard.end(), compShard);

    previousToBeCollected = toBeCollected;
    toBeCollected = build(shard, satelites, resp, toBeCollected, &rdShards);    
        
    while(toBeCollected && toBeCollected < previousToBeCollected) {
          
          // se tem coisa a coletar:          
          if(toBeCollected) {
              volta1(shard, satelites, resp, &toBeCollected, &rdShards);
          }
          previousToBeCollected = toBeCollected;                              
          toBeCollected = build(shard, satelites, resp, toBeCollected, &rdShards);          
          loop++;
  
    }                
    imprimeResp(resp, shard, satelites.size(), totalReward - toBeCollected, rdShards, argv[4]);                  
    return 0;
 
}

