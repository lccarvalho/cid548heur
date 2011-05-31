#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

struct shard{
    int posH;          //Posição Horizontal
    int posV;          //Posição Vertical
    int rShard;        //Ganho de Armazenagem
    int cH;            //Custo de armazenagem pelo satelite em rota horizontal
    int cV;            //Custo de armazenagem pelo satelite em rota vertical
};

struct satelite{
    int memTotal;      //Memoria total do satelite
    int memRestante;   //Memoria restante do satelite
};


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
        satV.insert(satV.end(),sat);

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

    readIn(obj, satH, satV, argv[1]);
    
    n = satH.size();
    cout << "SATELITES HORIZONTAIS: " << endl;
    for(i=0;i<n;i++){
        cout << "satelite H " << i+1 << ": Memoria Total: " << satH[i].memTotal << endl;
    }

    cout << endl;

    n = satV.size();
    cout << "SATELITES VERTICAIS: " << endl;
    for(i=0;i<n;i++){
        cout << "satelite V " << i+1 << ": Memoria Total: " << satV[i].memTotal << endl;
    }

    cout << endl;

    n = obj.size();
    cout << "SHARDS: " << endl;
    for(i=0;i<n;i++){
        cout << "Shard " << i+1 << "  Pos: " << obj[i].posH << " " << obj[i].posV << "  Ganho: " << obj[i].rShard << endl;
        cout << "Custo H: " << obj[i].cH << " Custo V: " << obj[i].cV << endl << endl;
    }

    return 0;
}

