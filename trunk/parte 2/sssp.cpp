#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

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
};

bool compSat(satelite sat1, satelite sat2){
    return sat1.memTotal < sat2.memTotal;
}

bool compShard(shard shard1, shard shard2){
    return max(shard1.rShard/shard1.cH, shard1.rShard/shard1.cV) > max(shard2.rShard/shard2.cH, shard2.rShard/shard2.cV);

}

void build(vector<shard>& obj, vector<satelite>& sat){
    
    int Objetivo = 0;
    int nSat = sat.size()/2;
    int nSh = obj.size();

    for(int i = 0; i < 2*nSat; i++){
        for(int j = 0; j < nSh; j++){
            if(obj[j].posH == sat[i].ns && obj[j].lida == false){
                if(sat[i].memRestante >= obj[j].cH){
                    cout << "Shard lida: " << obj[j].posH << " " << obj[j].posV << endl;
                    cout << "Satelite leitor: " << sat[i].ns << endl;
                    cout << "Tam Horizontal: " << obj[j].cH << endl;
                    cout << "Mem. Restante Sat Leitor: " << sat[i].memRestante << endl << endl;              
                    obj[j].lida = true;
                    obj[j].lidaPor = i;
                    sat[i].memRestante -= obj[j].cH;
                    Objetivo += obj[j].rShard;
                }
	    }
		
            if((obj[j].posV + nSat) == sat[i].ns && obj[j].lida == false){
                if(sat[i].memRestante >= obj[j].cV){
                    cout << "Shard lida: " << obj[j].posH << " " << obj[j].posV << endl;
                    cout << "Satelite leitor: " << sat[i].ns << endl;
                    cout << "Tam Vertical: " << obj[j].cV << endl;
                    cout << "Mem. Restante Sat Leitor: " << sat[i].memRestante << endl << endl;
                    obj[j].lida = true;
                    obj[j].lidaPor = i;
                    sat[i].memRestante -= obj[j].cV;
                    Objetivo += obj[j].rShard;
		}
	    }
	}  
    }
    cout << endl << "Objetivo: " << Objetivo << endl << endl;

}


/* A função abre o arquivo de entrada, lê os parâmetros necessários, montando vetores de estruturas de satelites e */ 
/* vetores de estruturas de shards */

void readIn(vector<shard>& obj, vector<satelite>& satH, vector<satelite>& satV, char* entrada){
    ifstream arqin(entrada);
    int n, i, seq;
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
        obj.insert(obj.end(),sh);
    
    }

    return;
}

int main(int argc, char* argv[]){
    ifstream entrada;
    ofstream saida;
    int i, n;
    vector<shard> obj;
    vector<satelite> satH;
    vector<satelite> satV;
    vector<int> resp;

    readIn(obj, satH, satV, argv[1]);
    
    sort(satH.begin(),satH.end(),compSat);

    cout << "SATELITES: " << endl;
    n = satH.size();
    for(i=0;i<n;i++){
        cout << "satelite H " << satH[i].ns << " Pos Vetor: " << i << ": Memoria Total: " << satH[i].memTotal << endl;
    }
    
    cout << endl;
    

    sort(obj.begin(), obj.end(), compShard);
    
    n = obj.size();
    cout << "SHARDS: " << endl;
    for(i=0;i<n;i++){
        cout << "Shard " << i+1 << "  Pos: " << obj[i].posH << " " << obj[i].posV << "  Ganho: " << obj[i].rShard << endl;
        cout << "Custo H: " << obj[i].cH << " Custo V: " << obj[i].cV << endl;
        cout << "Relacao custo memoria: " << max(obj[i].rShard/obj[i].cH, obj[i].rShard/obj[i].cV) << endl << endl;
    }
    cout << endl;
    
    build(obj, satH);

    return 0;
}

