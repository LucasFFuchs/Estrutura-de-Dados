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
        else if(comando == "ADD_POST"){
            int idPost, idAutor, timestamp;
            string texto;

            entrada >> idPost >> idAutor >> timestamp, texto;

            cadastrarPublicacao(rede, idPost, idAutor, timestamp, texto, saida);
        }
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
    if(retornaUserBuscaId(rede, id) != nullptr){
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

    user -> frontNotificacoes = nullptr;

    bool aumentouAltura = false;

    insereId(rede.arvore, user, aumentouAltura);
    insereUsername(&rede, user);

    saida << "USER_ADDED";

}

void buscarUsuarioPorId(MiniRede& rede, int id, ostream& saida) {
    Usuario* user = retornaUserBuscaId(rede, id);
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
    if(retornaUserBuscaId(rede, idSeguidor) == nullptr || retornaUserBuscaId(rede, idSeguido) == nullptr){
        saida << "ERROR USER_NOT_FOUND";
        return;
    }
    if (idSeguidor == idSeguido){
        saida << "ERROR CANNOT_FOLLOW_SELF";
        return;
    }
    
    Usuario* seguido = retornaUserBuscaId(rede, idSeguido);
    Usuario* seguidor =  retornaUserBuscaId(rede, idSeguidor);
    UsuarioNo* seguidoNo;
    UsuarioNo* seguidorNo;

    seguidoNo -> prox = nullptr;
    seguidorNo -> prox = nullptr;
    seguidoNo -> user = seguido;
    seguidorNo -> user = seguidor;

    UsuarioNo* atual = seguidor -> quemSigo;
    if(atual == nullptr){
        seguidor -> quemSigo = seguidoNo;
        return;
    }
    

    while(atual -> prox != nullptr){
        if(atual -> user -> id == idSeguido){
            saida << "ERROR CANNOT_FOLLOW_SELF";
            return;
        }
        atual = atual -> prox;
    }
    if(atual -> user -> id == idSeguido){
        saida << "ERROR CANNOT_FOLLOW_SELF";
        return;
    }
    
    atual -> prox = seguidoNo;
    saida << "FOLLOWED";    
}

void listarSeguindo(MiniRede& rede, int idUsuario, ostream& saida) {
    Usuario* user = retornaUserBuscaId(rede, idUsuario);

    if(user == nullptr){
        saida << "ERROR USER_NOT_FOUND";
        return;
    }

    saida << "FOLLOWING_BEGIN";

    UsuarioNo* atual = user -> quemSigo;
    while(atual != nullptr){
        saida << "USER " << atual -> user -> id << " " << 
        atual -> user -> username << " " << atual -> user -> name;
        atual  = atual -> prox;
    }

    saida << "FOLLOWING_END";
}

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, string texto, ostream& saida) {
    Usuario* user = retornaUserBuscaId(rede, idAutor);
    if(user == nullptr){
        saida << "ERROR USER_NOT_FOUND";
        return;
    }
    if(postJaExiste(rede, idPost)){
        saida << "ERROR POST_EXISTS";
        return;
    }
    Publicacao* post = new Publicacao;
    post -> timestamp = timestamp;
    post -> id = idPost;
    post -> id_autor = idAutor;
    post -> likes = 0;
    post -> texto = texto;

    post -> prox = nullptr;
    post -> quemCurtiu = nullptr;

    postarPublicacao(rede, post);
    saida << "POST_ADDED";
}

void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, ostream& saida) {
    Usuario* user = retornaUserBuscaId(rede, idUsuario);
    UsuarioNo* userNo;
    userNo -> prox = nullptr;
    userNo -> user = user;

    if(user == nullptr){
        saida << "ERROR USER_NOT_FOUND";
        return;
    }
    if(!postJaExiste(rede, idPost)){
        saida << "ERROR POST_NOT_FOUND";
        return;
    }
    Publicacao* post = retornaPublicacao(rede, idPost);
    UsuarioNo* atual = post -> quemCurtiu;
    if(atual == nullptr){
        post -> quemCurtiu = userNo;
        post -> likes++;
        return;
    }
    while(atual -> prox != nullptr){
        if(atual -> user -> id == idUsuario){
            saida << "ERROR ALREADY_LIKED";
        }
        atual = atual -> prox;
    }
    post -> likes++;
    atual -> prox = userNo;
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

