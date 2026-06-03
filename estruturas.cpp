#include "estruturas.h"
using namespace std;

Usuario* RetornaUser_BuscaIp(){

    return nullptr;
}

Usuario* RetornaUser_BuscaUsername(MiniRede& rede, string username){
    Usuario* atual = rede.hash[hash_function(username)];

    while(atual != nullptr){
        if(atual -> username == username){
            return atual;
        }
        atual = atual -> prox;
    }

    return nullptr;
}

int hash_function(string name){
    int soma = 0;
    for(int i = 0; i < name.length(); i++){
        soma += name[i];
    }

    return (soma & 10);
}