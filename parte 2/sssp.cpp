#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

struct shard{
    int posH;            //Posição Horizontal
    int posV;            //Posição Vertical
    float rShard;        //Ganho de Armazenagem
    float cH;            //Custo de armazenagem pelo satelite em rota horizontal
    float cV;            //Custo de armazenagem pelo satelite em rota vertical
    int nsh;
    int lidaPor;
    bool lida;
};

struct satelite{
    int ns;            //Numero do satelite
    int memTotal;      //Memoria total do satelite
    int memRestante;   //Memoria restante do satelite
};

bool compSat(satelite sat1, satelite sat2){
    return sat1.memTotal < sat2.memTotal;
}

bool compShard(shard shard1, shard shard2){
    return max(shard1.rShard/shard1.cH, shard1.rShard/shard1.cV) > max(shard2.rShard/shard2.cH, shard2.rShard/shard2.cV);

}


int build(vector<shard>& obj, vector<satelite>& sat, vector<char>& sol){

    int Objetivo = 0, count = 0;
    int nSat = sat.size()/2;
    int nSh = obj.size(), k;
    k = 12;

    cout << endl << "BUILD PELOS SATELITES" << endl << endl;

    for(int i = 0; i < 2*nSat; i++){
        for(int j = 0; j < nSh; j++){
            if(obj[j].posH == sat[i].ns && obj[j].lida == false){
                if(sat[i].memRestante >= obj[j].cH){
                    /*cout << "Shard lida: [" << obj[j].nsh << "] Posição: "  << obj[j].posH << " " << obj[j].posV << endl;
                    cout << "Satelite leitor: " << sat[i].ns << endl;
                    cout << "Tam Horizontal: " << obj[j].cH << endl;
                    cout << "Mem. Restante Sat Leitor: " << sat[i].memRestante << endl;
                    cout << "pos. no vetor de shards: " << j << endl;*/
                    obj[j].lida = true;
                    obj[j].lidaPor = i;
                    sat[i].memRestante -= obj[j].cH;
                    Objetivo += obj[j].rShard;
                    count += 1;
                    sol[j] = 'h';
                }
	    }

            if((obj[j].posV + nSat) == sat[i].ns && obj[j].lida == false){
                if(sat[i].memRestante >= obj[j].cV){
                    /*cout << "Shard lida: [" << obj[j].nsh << "] Posição: "  << obj[j].posH << " " << obj[j].posV << endl;
                    cout << "Satelite leitor: " << sat[i].ns << endl;
                    cout << "Tam Vertical: " << obj[j].cV << endl;
                    cout << "Mem. Restante Sat Leitor: " << sat[i].memRestante << endl;
                    cout << "pos. no vetor de shards: " << j << endl;*/
                    obj[j].lida = true;
                    obj[j].lidaPor = i;
                    sat[i].memRestante -= obj[j].cV;
                    Objetivo += obj[j].rShard;
                    count += 1;
                    sol[j] = 'v';
		}
	    }
	}  
    }
    cout << endl << "Ganho Obtido: " << Objetivo << endl << "Shards Lidas: " << count << endl;
    return Objetivo;

}


/* A função abre o arquivo de entrada, lê os parâmetros necessários, montando vetores de estruturas de satelites e */ 
/* vetores de estruturas de shards */

int readIn(vector<shard>& obj, vector<satelite>& satH, vector<char>& sol, char* entrada){
    ifstream arqin(entrada);
    int n, i, seq, total = 0;
    string tok, line;
    stringstream iss;

    satelite sat;
    shard sh;

    getline(arqin,tok);                      //linha que contêm o número de satélites de cada cjto
    n = atoi(tok.c_str());                   //transforma o número retirado em um int
    getline(arqin,line);
    iss.str(line);                           //atribui a ultima linha para um stringstream iss
   
    for(i=0;i<n;i++){                        //Monta o vetor de satelites a partir da linha lida
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
    for(i=0;i<n;i++){
        getline(iss,tok,' ');
        getline(iss,tok,' ');
        sat.memTotal = atoi(tok.c_str());
        sat.memRestante = sat.memTotal;
        sat.ns = 1 + i + n;
        satH.insert(satH.end(),sat);

    }

    getline(arqin,tok);                      //linha que contêm o número de shards
    n = atoi(tok.c_str());
    for(i=0;i<n;i++){
        iss.clear();
        getline(arqin,line);
        iss.str(line);
        getline(iss,tok,' ');
        seq = atoi(tok.c_str());
        sh.posH = seq;
        getline(iss,tok,' ');
        seq = atoi(tok.c_str());
        sh.posV = seq;
        getline(iss,tok,' ');
        seq = atoi(tok.c_str());
        sh.rShard = seq;
        getline(iss,tok,' ');
        seq = atoi(tok.c_str());
        sh.cH = seq;
        getline(iss,tok,' ');
        seq = atoi(tok.c_str());
        sh.cV = seq;
        total += sh.rShard;
        sh.nsh = i;
        sh.lida = false;
        sh.lidaPor = -1;
        obj.insert(obj.end(),sh);
        sol.insert(sol.end(),'n');
    
    }

    return total;
}

int testNeighborhood(vector<shard>& obj, vector<satelite>& satH, vector<char>& solAtual, int totalAtual){
    int nSh = obj.size();
    int memRestAtual = 0, satLeitor = 0, novoTotal = 0, custoShard = 0, BestFit = totalAtual;
    vector<char> novaSol = solAtual;

    cout << "TESTE DE VIZINHANCA" << endl << endl;

    for(int i = 0; i < nSh; i++){
        if(obj[i].lida == true){
            satLeitor = obj[i].lidaPor;
            novoTotal = totalAtual - obj[i].rShard;
            if(obj[i].lidaPor < nSh) custoShard = obj[i].cH;      //esse if-else escobre qual o custo que a shard teve 
            else custoShard = obj[i].cV;                          //no satelite que de fato ela ocupou
            memRestAtual = satH[satLeitor].memRestante + custoShard;
            novaSol[i] = 'n';

            for(int j = 0; j < nSh; j++){
                //Se a shard j nao tiver sido lida, sua posH for igual ao sat. que leu a shard que saiu,
                //ela não é a shard que saiu, e ela ocupa uma memoria menor que a restante no satelite,
                //adicionaremos ela a nossa nova possível solução
                if(obj[j].lida == false && obj[j].posH == satLeitor && i != j && obj[j].cH <= memRestAtual){
                    memRestAtual -= obj[j].cH;
                    novoTotal += obj[j].rShard;
                    novaSol[j] = 'h';
                    
                }
                //Idem, porem para posV
                else if(obj[j].lida == false && obj[j].posV == satLeitor && i != j && obj[j].cV <= memRestAtual){
                         memRestAtual -= obj[j].cV;
                         novoTotal += obj[j].rShard;
                         novaSol[j] = 'v';
                     }
            }
            
            if(novoTotal >= BestFit){
            solAtual = novaSol;
            BestFit = novoTotal;
            }
            else novaSol = solAtual;
        
        }
    
    }
    return BestFit;
}


int main(int argc, char* argv[]){
    ifstream entrada;
    ofstream saida;
    int i, n, Z, limS, nZ, tIn = time(NULL), tFin;
    vector<shard> obj;
    vector<satelite> satH;
    vector<char> sol;
    i = 0;
    n = 0;

    limS = readIn(obj, satH, sol, argv[1]);
    sort(satH.begin(),satH.end(),compSat);

    /*cout << "SATELITES: " << endl;
    n = satH.size();
    for(i=0;i<n;i++){
        cout << "satelite H " << satH[i].ns << " Pos Vetor: " << i << ": Memoria Total: " << satH[i].memTotal << endl;
    }
    cout << endl;*/


    sort(obj.begin(), obj.end(), compShard);

    /*n = obj.size();
    cout << "SHARDS: " << endl;
    for(i=0;i<n;i++){
        cout << "Shard " << obj[i].nsh << "  Pos: " << obj[i].posH << " " << obj[i].posV << "  Ganho: " << obj[i].rShard << endl;
        cout << "Custo H: " << obj[i].cH << " Custo V: " << obj[i].cV << endl;
        cout << "Relacao custo memoria: " << max(obj[i].rShard/obj[i].cH, obj[i].rShard/obj[i].cV) << endl << endl;
    }*/
    cout << endl << endl;
    cout << "Limitante superior: " << limS << endl << endl;

    Z = build(obj, satH, sol);
    cout << endl;

    if(Z == limS){
        cout << endl << "Solução ótima usa todas as shards!" << endl << endl;
        return 0;
    }
    nZ = testNeighborhood(obj, satH, sol, Z);
    cout << "Nova Solucao: " << nZ << endl;
    tFin = time(NULL);
    cout << "Tempo de execução (seg): " << difftime(tFin,tIn) << endl << endl;


    return 0;
}

