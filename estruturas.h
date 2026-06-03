#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H
#include "minirede.h"

Usuario* RetornaUser_BuscaIp();
Usuario* RetornaUser_BuscaUsername(MiniRede& rede, string username);

int hash_function(string name);


#endif
