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

void ordenaNotificacao(Usuario* user, string conteudo){
     Notificacao* nova = new Notificacao;
     nova -> texto = conteudo;
     nova -> prox = nullptr;

    if(user -> frontNotificacoes == nullptr){
        user -> frontNotificacoes = nova;
        return;

    }

    Notificacao* atual = user -> frontNotificacoes;
    while(atual -> prox != nullptr){
        atual =atual -> prox;
    }
    atual -> prox = nova;
}

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, ostream& saida) {
    Usuario* user = retornaUserBuscaId(rede, idUsuario);
    if(user == nullptr){
        saida << " ERROR_USER_/NOT_FOUD";
        return;
    }

    saida << "NOTIFICATIONS_BEGIN\n";

    int count = 0;

    while(user -> frontNotificacoes != nullptr && count < k){
        Notificacao* atual = user -> frontNotificacoes;

        saida << "NOTIFICATION" << atual -> texto << "\n";

        user -> frontNotificacaoes = atual -> prox;
        delete atual;

        count++;

        
    }

    saida << "NOTIFICATIONS_END";   

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
        p = p->prox;
    }


    Publicacao** feed = new Publicacao*[max]; //ARRAY DE PUIBLIVCACOES
    int tam = 0;

    
    UsuarioNo* seg = user->quemSigo;

    while(seg != nullptr){

        int idSeguido = seg->user->id;
        Publicacao* post = rede.publicacoes;

        while(post != nullptr){
            if(post->id_autor == idSeguido){
                feed[tam++] = post;
            }
            post = post->prox;
        }

        seg = seg->prox;
    }


    for(int i = 0; i < tam - 1; i++){ // ORDENAÇÃO DO TIMESTAMP ETC...
        for(int j = 0; j < tam - i - 1; j++){

            bool troca = false;

            if(feed[j]->timestamp < feed[j+1]->timestamp){
                troca = true;
            }
            else if(feed[j]->timestamp == feed[j+1]->timestamp &&
                    feed[j]->id > feed[j+1]->id){
                troca = true;
            }

            if(troca){
                Publicacao* aux = feed[j];
                feed[j] = feed[j+1];
                feed[j+1] = aux;
            }
        }
    }

    
    int limite = (tam < k ? tam : k); //IMPRIME ATE O LIMITE DE K

    for(int i = 0; i < limite; i++){
        Publicacao* p = feed[i];

        saida << "POST "
              << p->id << " "
              << p->id_autor << " "
              << p->timestamp << " "
              << p->likes << " "
              << p->texto << "\n";
    }

    saida << "FEED_END\n";

    delete[] feed;
}


void listarTopPosts(MiniRede& rede, int k, ostream& saida) {
    saida << "TOP_POSTS_BEGIN\n";

   //CONFA OS POSTS
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
        p = p->prox;
    }

    for(int i = 0; i < tam - 1; i++){ //ORDENA OS LIKES
        for(int j = 0; j < tam - i - 1; j++){

            bool troca = false;

            if(arr[j]->likes < arr[j+1]->likes){
                troca = true;
            }
            else if(arr[j]->likes == arr[j+1]->likes &&
                    arr[j]->id > arr[j+1]->id){
                troca = true;
            }

            if(troca){
                Publicacao* aux = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = aux;
            }
        }
    }

    int limite = (tam < k ? tam : k);

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

