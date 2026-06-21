#include "minirede.h"
#include "estruturas.h"
#include <sstream>
using namespace std;

void inicializarMiniRede(MiniRede& rede) {
    rede.arvore = nullptr;
    rede.publicacoes = nullptr;
    for(int i = 0; i < 10; i++){
        rede.hash[i] = nullptr;
    }
}

void liberarMiniRede(MiniRede& rede) {
    deletaFilhos(rede.arvore);
    rede.arvore = nullptr;

    Usuario* atual;
    Usuario* deletar;
    for(int i = 0; i < 10; i++){
        atual = rede.hash[i];

        while(atual != nullptr){
            deletar = atual;
            atual = atual -> prox;
            delete deletar;
        }
    }

    Publicacao* atual2 = rede.publicacoes; 
    Publicacao* deletar2;
    
    while(atual != nullptr){
        UsuarioNo* atual3 = atual2 -> quemCurtiu;
        UsuarioNo* deletar3;
        
        while(atual3 != nullptr){
            deletar3 = atual3;
            atual3 = atual3 -> prox;
            delete deletar3;
        }
        
        deletar2 = atual2;
        atual2 = atual2 -> prox;
        delete deletar2;
    }

}

void processarComandos(MiniRede& rede, istream& entrada, ostream& saida) {
    string comando;

    while (entrada >> comando && comando != "END"){
        string linha;
        getline(entrada, linha);
        istringstream iss(linha);

        if(comando == "ADD_USER"){
            int id;
            string username;
            string nome;

            if (!(iss >> id >> username >> nome)) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            string sobra;
            if (iss >> sobra) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            cadastrarUsuario(rede, id, username, nome, saida);
        }
        else if(comando == "FIND_USER"){
            int id;

            if (!(iss >> id)) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            string sobra;
            if (iss >> sobra) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            buscarUsuarioPorId(rede, id, saida);
        }
        else if(comando == "FIND_USERNAME"){
            string username;

            if (!(iss >> username)) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            string sobra;
            if (iss >> sobra) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            buscarUsuarioPorUsername(rede, username, saida);
        }
        else if(comando == "LIST_USERS"){
            string sobra;
            if (iss >> sobra) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }
            listarUsuarios(rede, saida);
        }
        else if(comando == "FOLLOW"){
            int idSeguidor, idSeguido;

            if (!(iss >> idSeguidor >> idSeguido)) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            string sobra;
            if (iss >> sobra) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            seguirUsuario(rede, idSeguidor, idSeguido, saida);
        }
        else if(comando == "LIST_FOLLOWING"){
            int idSeguido;

            if (!(iss >> idSeguido)) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            string sobra;
            if (iss >> sobra) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            listarSeguindo(rede, idSeguido, saida);
        }
        else if(comando == "ADD_POST"){
            int idPost, idAutor, timestamp;
            string texto;

            if (!(iss >> idPost >> idAutor >> timestamp >> texto)) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            string sobra;
            if (iss >> sobra) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            cadastrarPublicacao(rede, idPost, idAutor, timestamp, texto, saida);
        }
        else if(comando == "LIKE"){
            int idUsuario, idPost;

            if (!(iss >> idUsuario >> idPost)) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            string sobra;
            if (iss >> sobra) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            curtirPublicacao(rede, idUsuario, idPost, saida);
        }
        else if(comando == "GET_NOTIFICATIONS"){
            int idUsuario, k;

            if (!(iss >> idUsuario >> k)) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            string sobra;
            if (iss >> sobra) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            consultarNotificacoes(rede, idUsuario, k, saida);
        }
        else if(comando == "FEED"){
            int idUsuario, k;
            
            if (!(iss >> idUsuario >> k)) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            string sobra;
            if (iss >> sobra) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            gerarFeed(rede, idUsuario, k, saida);
        }
        else if(comando == "TOP_POSTS"){
            int k;
            if (!(iss >>  k)) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            string sobra;
            if (iss >> sobra) {
                saida << "ERROR INVALID_COMMAND\n";
                continue;
            }

            listarTopPosts(rede, k, saida);
        }
        else{
            saida << "ERROR INVALID_COMMAND\n";
        }
    }
}

void cadastrarUsuario(MiniRede& rede, int id, string username, string nomeCompleto, ostream& saida) {
    if(retornaUserBuscaId(rede, id) != nullptr || retornaUserBuscaUsername(rede, username) != nullptr){
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
        adicionarNotificacao(seguido, "NOTIFICATION FOLLOW " + to_string(seguidor -> id) + "\n");
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
    adicionarNotificacao(seguido, "NOTIFICATION FOLLOW " + to_string(seguidor -> id) + "\n");
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
        Usuario* user = retornaUserBuscaId(rede, idUsuario);

    if(user == nullptr){
        saida << "ERROR USER_NOT_FOUND\n";
        return;
    }

    saida << "FEED_BEGIN\n";

    int max = 0;  //CONTA O TOTAL DE PUBLICACOES 
    Publicacao* p = rede.publicacoes;

    while(p != nullptr){
        max++;
        p = p -> prox;
    }


    Publicacao** feed = new Publicacao*[max]; //ARRAY DE PUBLICACOES
    int tam = 0;

    
    UsuarioNo* seg = user -> quemSigo;

    while(seg != nullptr){

        int idSeguido = seg -> user -> id;
        Publicacao* post = rede.publicacoes;

        while(post != nullptr){
            if(post -> id_autor == idSeguido){
                feed[tam] = post;
                tam++;
            }
            post = post -> prox;
        }

        seg = seg -> prox;
    }


    for(int i = 0; i < tam - 1; i++){ // ORDENAÇÃO DO TIMESTAMP ETC...
        for(int j = 0; j < tam - i - 1; j++){

            bool troca = false;

            if(feed[j] -> timestamp < feed[j+1] -> timestamp){
                troca = true;
            }
            else if(feed[j] -> timestamp == feed[j+1] -> timestamp &&
                    feed[j] -> id > feed[j+1] -> id){
                troca = true;
            }

            if(troca){
                Publicacao* aux = feed[j];
                feed[j] = feed[j+1];
                feed[j+1] = aux;
            }
        }
    }

    int limite;
    if(tam < k)
        limite = tam;
    else
        limite = k;

    for(int i = 0; i < limite; i++){
        Publicacao* p = feed[i];

        saida << "POST "
              << p -> id << " "
              << p -> id_autor << " "
              << p -> timestamp << " "
              << p -> likes << " "
              << p -> texto << "\n";
    }

    saida << "FEED_END\n";

    delete[] feed;
}


void listarTopPosts(MiniRede& rede, int k, ostream& saida){
    saida << "TOP_POSTS_BEGIN\n";

   //CONTA OS POSTS
    int max = 0;
    Publicacao* p = rede.publicacoes;

    while(p != nullptr){
        max++;
        p = p->prox;
    }

    Publicacao** arr = new Publicacao*[max];
    int tam = 0;

    p = rede.publicacoes;

    while(p != nullptr){
        arr[tam++] = p;
        p = p -> prox;
    }

    for(int i = 0; i < tam - 1; i++){ //ORDENA OS LIKES
        for(int j = 0; j < tam - i - 1; j++){

            bool troca = false;

            if(arr[j] -> likes < arr[j+1] -> likes){
                troca = true;
            }
            else if(arr[j] -> likes == arr[j+1] -> likes &&
                    arr[j] -> id > arr[j+1] -> id){
                troca = true;
            }

            if(troca){
                Publicacao* aux = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = aux;
            }
        }
    }

    int limite;
    if(tam < k)
        limite = tam;
    else
        limite = k;

    for(int i = 0; i < limite; i++){
        Publicacao* p = arr[i];

        saida << "POST "
              << p->id << " "
              << p->id_autor << " "
              << p->timestamp << " "
              << p->likes << " "
              << p->texto << "\n";
    }

    saida << "TOP_POSTS_END\n";

    delete[] arr;
}

int main() {
    MiniRede rede;

    inicializarMiniRede(rede);
    processarComandos(rede, cin, cout);
    liberarMiniRede(rede);

    return 0;
}

