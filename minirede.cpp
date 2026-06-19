#include "minirede.h"
#include "estruturas.h"
using namespace std;

void inicializarMiniRede(MiniRede& rede) {
    rede.arvore = nullptr;
    rede.publicacoes = nullptr;
    for(int i = 0; i < 10; i++){
        rede.hash[i] = nullptr;
    }
}

void liberarMiniRede(MiniRede& rede) {
    // TODO
}

void processarComandos(MiniRede& rede, istream& entrada, ostream& saida) {
    string comando;

    while (entrada >> comando && comando != "END"){

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

            entrada >> idPost >> idAutor >> timestamp >> texto;
            cadastrarPublicacao(rede, idPost, idAutor, timestamp, texto, saida);
        }
        else if(comando == "LIKE"){
            int idUsuario, idPost;

            entrada >> idUsuario >> idPost;
            curtirPublicacao(rede, idUsuario, idPost, saida);
        }
        else if(comando == "GET_NOTIFICATIONS"){
            int idUsuario, k;

            entrada >> idUsuario >> k;
            consultarNotificacoes(rede, idUsuario, k, saida);
        }
        else if(comando == "FEED"){}
        else if(comando == "TOP_POSTS"){}
    }
}

void cadastrarUsuario(MiniRede& rede, int id, string username, string nomeCompleto, ostream& saida) {
    if(retornaUserBuscaId(rede, id) != nullptr){
        saida << "ERROR USER_EXISTS\n";
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

    rede.arvore = insereId(rede.arvore, user, aumentouAltura);
    insereUsername(&rede, user);

    saida << "USER_ADDED\n";

}

void buscarUsuarioPorId(MiniRede& rede, int id, ostream& saida) {
    Usuario* user = retornaUserBuscaId(rede, id);
    if(user != nullptr){
        saida << "USER " << user -> id << " " << user -> username << " " << user -> name << "\n";
    }
    else{
        saida << "ERROR USER_NOT_FOUND\n";
    }
}

void buscarUsuarioPorUsername(MiniRede& rede, string username, ostream& saida) {
    Usuario* user = retornaUserBuscaUsername(rede, username);
    if(user != nullptr){
        saida << "USER " << user -> id << " " << user -> username << " " << user -> name << "\n";
    }
    else{
        saida << "ERROR USER_NOT_FOUND\n";
    }
}

void listarUsuarios(MiniRede& rede, ostream& saida) {
    saida << "USERS_BEGIN\n";
    imprimirUsuarios(rede.arvore, saida);
    saida << "USERS_END\n";
}

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, ostream& saida) {
    Usuario* seguido = retornaUserBuscaId(rede, idSeguido);
    Usuario* seguidor =  retornaUserBuscaId(rede, idSeguidor);
    if(seguidor == nullptr || seguido == nullptr){
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    if (idSeguidor == idSeguido){
        saida << "ERROR CANNOT_FOLLOW_SELF\n";
        return;
    }
    
    UsuarioNo* seguidoNo = new UsuarioNo;
    seguidoNo -> prox = nullptr;
    seguidoNo -> user = seguido;

    UsuarioNo* atual = seguidor -> quemSigo;
    if(atual == nullptr){
        seguidor -> quemSigo = seguidoNo;
        adicionarNotificacao(seguido, "NOTIFICATIONS FOLLOW " + to_string(seguidor -> id) + "\n");
        saida << "FOLLOWED\n";
        return;
    }
    
    while(atual -> prox != nullptr){
        if(atual -> user -> id == idSeguido){
            saida << "ERROR ALREADY_FOLLOWING\n";
            delete seguidoNo;
            return;
        }
        atual = atual -> prox;
    }
    if(atual -> user -> id == idSeguido){
        saida << "ERROR ALREADY_FOLLOWING\n";
        delete seguidoNo;
        return;
    }
    
    atual -> prox = seguidoNo;
    adicionarNotificacao(seguido, "NOTIFICATIONS FOLLOW " + to_string(seguidor -> id) + "\n");
    saida << "FOLLOWED\n";    
}

void listarSeguindo(MiniRede& rede, int idUsuario, ostream& saida) {
    Usuario* user = retornaUserBuscaId(rede, idUsuario);

    if(user == nullptr){
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }

    saida << "FOLLOWING_BEGIN\n";

    UsuarioNo* atual = user -> quemSigo;
    while(atual != nullptr){
        saida << "USER " << atual -> user -> id << " " << 
        atual -> user -> username << " " << atual -> user -> name << "\n";
        atual  = atual -> prox;
    }

    saida << "FOLLOWING_END\n";
}

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, string texto, ostream& saida) {
    Usuario* user = retornaUserBuscaId(rede, idAutor);
    if(user == nullptr){
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    if(postJaExiste(rede, idPost)){
        saida << "ERROR POST_EXISTS\n";
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

    postarPublicacao(&rede, post);
    saida << "POST_ADDED\n";
}

void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, ostream& saida) {
    Usuario* user = retornaUserBuscaId(rede, idUsuario);

    if(user == nullptr){
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    if(!postJaExiste(rede, idPost)){
        saida << "ERROR POST_NOT_FOUND\n";
        return;
    }

    Publicacao* post = retornaPublicacao(rede, idPost);
    UsuarioNo* atual = post -> quemCurtiu;
    UsuarioNo* userNo = new UsuarioNo;
    userNo -> prox = nullptr;
    userNo -> user = user;

    if(atual == nullptr){
        post -> quemCurtiu = userNo;
        post -> likes++;
        saida << "LIKED\n";
        adicionarNotificacao(retornaUserBuscaId(rede, post -> id_autor), 
                         "NOTIFICATION LIKE " + to_string(idUsuario) + " " + to_string(idPost) + "\n");
        return;
    }
    while(atual -> prox != nullptr){
        if(atual -> user -> id == idUsuario){
            saida << "ERROR ALREADY_LIKED\n";
            delete userNo;
            return;
        }
        atual = atual -> prox;
    }
    if(atual -> user -> id == idUsuario){
        saida << "ERROR ALREADY_LIKED\n";
        delete userNo;
        return;
    }
    post -> likes++;
    atual -> prox = userNo;
    saida << "LIKED\n";
    adicionarNotificacao(retornaUserBuscaId(rede, post -> id_autor), 
                         "NOTIFICATION LIKE " + to_string(idUsuario) + " " + to_string(idPost) + "\n");
}

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, ostream& saida) {
    Usuario* user = retornaUserBuscaId(rede, idUsuario);
    if(user == nullptr){
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }
    if(k < 0){
        saida << "ERROR K_IS_NEGATIVE\n";
        return;
    }
    Notificacao* atual = user -> frontNotificacoes;
    Notificacao* deletar;
    saida << "NOTIFICATIONS_BEGIN\n";
    for(int i = 0; i < k; i++){
        if(atual == nullptr){
            break;
        }
        saida << atual -> texto;
        
        deletar = atual;
        atual = atual -> prox;
        user -> frontNotificacoes = atual;
        delete deletar;
    }

    saida << "NOTIFICATIONS_END\n";
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

