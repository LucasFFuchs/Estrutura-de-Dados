#include "estruturas.h"
using namespace std;

Usuario* retornaUserBuscaUsername(MiniRede& rede, string username){
    Usuario* atual = rede.hash[hash_function(username)];

    while(atual != nullptr){
        if(atual  ->  username == username){
            return atual;
        }
        atual = atual  ->  prox;
    }

    return nullptr;
}

int hash_function(string name){
    int soma = 0;
    for(unsigned int i = 0; i < name.length(); i++){
        soma += name[i];
    }
    return (soma % 10);
}

void insereUsername(MiniRede* rede, Usuario* user){
    Usuario* atual;
    atual = rede -> hash[hash_function(user -> username)];

    user -> prox = atual;

    rede -> hash[hash_function(user -> username)] = user;
}

Usuario* retornaUserBuscaId(MiniRede& rede, int id){
    Usuario* atual = rede.arvore;
    while (atual != nullptr) {
        if (atual -> id == id) {
            return atual;
        }

        if (id < atual -> id) {
            atual = atual -> esq;
        } else {
            atual = atual -> dir;
        }
    }
    return nullptr;
}


Usuario* rotacaoDireita(Usuario* p) {
    Usuario* u = p->esq;

    p->esq = u->dir;
    u->dir = p;

    return u;
}

Usuario* rotacaoEsquerda(Usuario* p) {
    Usuario* u = p->dir;

    p->dir = u->esq;
    u->esq = p;

    return u;
}

Usuario* casoEE(Usuario* p, bool& aumentouAltura) {
    Usuario* u = p->esq;

    p->FB = 0;
    u->FB = 0;

    p = rotacaoDireita(p);

    aumentouAltura = false;

    return p;
}

Usuario* casoED(Usuario* p, bool& aumentouAltura) {
    Usuario* u = p->esq;
    Usuario* v = u->dir;

    if (v->FB == 1) {
        p->FB = -1;
        u->FB = 0;
    }
    else if (v->FB == -1) {
        p->FB = 0;
        u->FB = 1;
    }
    else {
        p->FB = 0;
        u->FB = 0;
    }

    v->FB = 0;

    p->esq = rotacaoEsquerda(u);
    p = rotacaoDireita(p);

    aumentouAltura = false;

    return p;
}

Usuario* casoDD(Usuario* p, bool& aumentouAltura) {
    Usuario* u = p->dir;

    p->FB = 0;
    u->FB = 0;

    p = rotacaoEsquerda(p);

    aumentouAltura = false;

    return p;
}

Usuario* casoDE(Usuario* p, bool& aumentouAltura) {
    Usuario* u = p->dir;
    Usuario* v = u->esq;

    if (v->FB == -1) {
        p->FB = 1;
        u->FB = 0;
    }
    else if (v->FB == 1) {
        p->FB = 0;
        u->FB = -1;
    }
    else {
        p->FB = 0;
        u->FB = 0;
    }

    v->FB = 0;

    p->dir = rotacaoDireita(u);
    p = rotacaoEsquerda(p);

    aumentouAltura = false;

    return p;
}

Usuario* insereId(Usuario* raiz, Usuario* user, bool& aumentouAltura) {

    if (raiz == NULL) {

        user->esq = NULL;
        user->dir = NULL;
        user->FB = 0;

        aumentouAltura = true;

        return user;
    }

    if (user->id < raiz->id) {

        raiz->esq = insereId(raiz->esq, user, aumentouAltura);

        if (aumentouAltura) {

            switch (raiz->FB) {

                case -1:
                    raiz->FB = 0;
                    aumentouAltura = false;
                    break;

                case 0:
                    raiz->FB = 1;
                    break;

                case 1:

                    if (raiz->esq->FB == 1)
                        raiz = casoEE(raiz, aumentouAltura);
                    else
                        raiz = casoED(raiz, aumentouAltura);

                    break;
            }
        }
    }

    else if (user->id > raiz->id) {

        raiz->dir = insereId(raiz->dir, user, aumentouAltura);

        if (aumentouAltura) {

            switch (raiz->FB) {

                case 1:
                    raiz->FB = 0;
                    aumentouAltura = false;
                    break;

                case 0:
                    raiz->FB = -1;
                    break;

                case -1:

                    if (raiz->dir->FB == -1)
                        raiz = casoDD(raiz, aumentouAltura);
                    else
                        raiz = casoDE(raiz, aumentouAltura);

                    break;
            }
        }
    }

    else {
        aumentouAltura = false;
    }

    return raiz;
}

void imprimirUsuarios(Usuario* user, ostream& saida){
    if(user == nullptr)
        return;
    imprimirUsuarios(user -> esq, saida);
    saida << "USER " << user -> id << " " << user -> username << " " << user -> name << "\n";
    imprimirUsuarios(user -> dir, saida);
}

bool postJaExiste(MiniRede& rede, int idPost){
    Publicacao* postAtual = rede.publicacoes; 
    while(postAtual != nullptr){
        if(postAtual -> id == idPost)
            return true;
        postAtual = postAtual -> prox;
    }
    return false;
}

void postarPublicacao(MiniRede* rede, Publicacao* post){
    if(rede -> publicacoes == nullptr){
        rede -> publicacoes = post;
        return;
    }
    Publicacao* atual = rede -> publicacoes;
    while(atual -> prox != nullptr){
        atual = atual -> prox;
    }

    atual -> prox = post;
}

Publicacao* retornaPublicacao(MiniRede& rede, int idPost){
    Publicacao* atual = rede.publicacoes;
    while(atual != nullptr){
        if(atual -> id == idPost){
            return atual;
        }
        atual = atual -> prox;
    }
    return nullptr;
}

void adicionarNotificacao(Usuario* user, string texto){
    Notificacao* novo = new Notificacao;
    novo -> prox = nullptr;
    novo -> texto = texto;

    Notificacao* atual = user -> frontNotificacoes;
    if(atual == nullptr){
        user -> frontNotificacoes = novo;
        return;
    }

    while(atual -> prox != nullptr){
        atual = atual -> prox;
    }

    atual -> prox = novo;
}

void deletaFilhos(Usuario* no){
    if(no == nullptr)
        return;
    deletaFilhos(no -> esq);
    deletaFilhos(no -> dir);

    UsuarioNo* atual = no -> quemSigo; //Deleta Usuarios nós de quem eu sigo
    UsuarioNo* deletar;

    while(atual != nullptr){
        deletar = atual;
        atual = atual -> prox;
        delete deletar;
    }

    Notificacao* atual2 = no -> frontNotificacoes; //Deleta notificacoes nao vistas
    Notificacao* deletar2;

    while(atual2 != nullptr){
        deletar2 = atual2;
        atual2 = atual2 -> prox;
        delete deletar2;
    }
    delete no; //Deleta usuario
}

