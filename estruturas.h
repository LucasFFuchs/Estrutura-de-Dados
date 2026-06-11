#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H
#include "minirede.h"

Usuario* retornaUserBuscaUsername(MiniRede& rede, string username);
int hash_function(string name);
void insereUsername(MiniRede* rede, Usuario* user);

Usuario* retornaUserBuscaId(MiniRede& rede, int id);

Usuario* rotacaoDireita(Usuario* p);
Usuario* rotacaoEsquerda(Usuario* p);

Usuario* casoEE(Usuario* p, bool& aumentouAltura);
Usuario* casoED(Usuario* p, bool& aumentouAltura);
Usuario* casoDD(Usuario* p, bool& aumentouAltura);
Usuario* casoDE(Usuario* p, bool& aumentouAltura);

Usuario* insereId(Usuario* raiz, Usuario* user, bool& aumentouAltura);

void imprimirUsuarios(Usuario* user, ostream& saida);

bool postJaExiste(MiniRede rede, int idPost);
void postarPublicacao(MiniRede rede, Publicacao* post);
Publicacao* retornaPublicacao(MiniRede rede, int idPost);


#endif
