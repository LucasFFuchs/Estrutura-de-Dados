#include "minirede.h"
#include "estruturas.h"
using namespace std;

void inicializarMiniRede(MiniRede& rede) {
    // TODO
}

void liberarMiniRede(MiniRede& rede) {
    // TODO
}

void processarComandos(MiniRede& rede, istream& entrada, ostream& saida) {
    string comando;

    entrada >> comando;

    while(comando != "END"){

        if(comando == "ADD_USER"){
            int id;
            string username;
            string nome;

            entrada >> id >> username >> nome;

            cadastrarUsuario(rede, id, username, nome, saida);
        }
        else if(comando == "FIND_USER"){
            int id;

            entrada >> id;
            
            buscarUsuarioPorId(rede, id, saida);
        }
        else if(comando == "FIND_USERNAME"){
            string username;

            entrada >> username;

            buscarUsuarioPorUsername(rede, username, saida);
        }
        else if(comando == "LIST_USERS"){
            listarUsuarios(rede, saida);
        }
        else if(comando == "FOLLOW"){
            int idSeguidor, idSeguido;
            entrada >> idSeguidor >> idSeguido;

            seguirUsuario(rede, idSeguidor, idSeguido, saida);
        }
        else if(comando == "LIST_FOLLOWING"){
            int idSeguido;
            entrada >> idSeguido;

            listarSeguindo(rede, idSeguido, saida);
        }
        else if(comando == "ADD_POST"){}
        else if(comando == "LIKE"){}
        else if(comando == "GET_NOTIFICATIONS"){}
        else if(comando == "FEED"){}
        else if(comando == "TOP_POSTS"){}
    }
    
    // TODO: ler comandos da entrada padrao ate END.
    // Para cada comando, chamar a funcao correspondente.
    // Nao imprimir menu, prompt ou texto extra.
}

void cadastrarUsuario(MiniRede& rede, int id, string username, string nomeCompleto, ostream& saida) {
    if(retornaUserBuscaIp(rede, id) != nullptr){
        saida << "ERROR USER_EXISTS";
        return;
    }

    Usuario* user = new Usuario;
    user -> id = id;
    user -> username = username;
    user -> name = nomeCompleto;

    user -> esq = nullptr;
    user -> dir = nullptr;
    user -> prox = nullptr;
    user -> quemSigo = nullptr;
    user -> publicacoes = nullptr;

    user -> prox_curtiram = nullptr;
    user -> prox_seguindo = nullptr;

    bool aumentouAltura = false;

    insereId(rede.arvore, user, aumentouAltura);
    insereUsername(&rede, user);

    saida << "USER_ADDED";

}

void buscarUsuarioPorId(MiniRede& rede, int id, ostream& saida) {
    Usuario* user = retornaUserBuscaIp(rede, id);
    if(user != nullptr){
        saida << "USER " << user -> id << " " << user -> username << " " << user -> name;
    }
    else{
        saida << "ERROR USER_NOT_FOUND";
    }
}

void buscarUsuarioPorUsername(MiniRede& rede, string username, ostream& saida) {
    Usuario* user = retornaUserBuscaUsername(rede, username);
    if(user != nullptr){
        saida << "USER " << user -> id << " " << user -> username << " " << user -> name;
    }
    else{
        saida << "ERROR USER_NOT_FOUND";
    }
}

void listarUsuarios(MiniRede& rede, ostream& saida) {
    saida << "USERS_BEGIN";
    imprimirUsuarios(rede.arvore, saida);
    saida << "USERS_END";
}

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, ostream& saida) {
    if(retornaUserBuscaIp(rede, idSeguidor) == nullptr || retornaUserBuscaIp(rede, idSeguido) == nullptr){
        saida << "ERROR USER_NOT_FOUND";
        return;
    }
    if (idSeguidor == idSeguido){
        saida << "ERROR CANNOT_FOLLOW_SELF";
        return;
    }
    
    Usuario* seguido = retornaUserBuscaIp(rede, idSeguido);
    Usuario* seguidor =  retornaUserBuscaIp(rede, idSeguidor);
    Usuario* atual = seguidor -> quemSigo;
    if(atual == nullptr){
        seguidor -> quemSigo = seguido;
        return;
    }
    

    while(atual -> prox_seguindo != nullptr){
        if(atual -> id == idSeguido){
            saida << "ERROR CANNOT_FOLLOW_SELF";
            return;
        }
        atual = atual -> prox_seguindo;
    }
    if(atual -> id == idSeguido){
        saida << "ERROR CANNOT_FOLLOW_SELF";
        return;
    }
    
    atual -> prox_seguindo = seguido;
    saida << "FOLLOWED";    
}

void listarSeguindo(MiniRede& rede, int idUsuario, ostream& saida) {
    Usuario* user = retornaUserBuscaIp(rede, idUsuario);

    if(user == nullptr){
        saida << "ERROR USER_NOT_FOUND";
        return;
    }

    saida << "FOLLOWING_BEGIN";

    Usuario* atual = user -> quemSigo;
    while(atual != nullptr){
        saida << "USER " << atual -> id << " " << atual -> username << " " << atual -> name;
        atual = atual -> prox_seguindo;
    }

    saida << "FOLLOWING_END";
}

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, const char texto[], ostream& saida) {
    // TODO
}

void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, ostream& saida) {
    // TODO
}

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, ostream& saida) {
    // TODO
}

void gerarFeed(MiniRede& rede, int idUsuario, int k, ostream& saida) {
    // TODO
}

void listarTopPosts(MiniRede& rede, int k, ostream& saida) {
    // TODO
}

int main() {
    MiniRede rede;

    inicializarMiniRede(rede);
    processarComandos(rede, cin, cout);
    liberarMiniRede(rede);

    return 0;
}

