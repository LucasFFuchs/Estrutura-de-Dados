#include "minirede.h"
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
        else if(comando == "FIND_USERNAME"){}
        else if(comando == "LIST_USERS"){}
        else if(comando == "FOLLOW"){}
        else if(comando == "LIST_FOLLOWING"){}
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
    Usuario *user = new Usuario;
    user -> id = id;
    user -> username = username;
    user -> name = nomeCompleto;

}

void buscarUsuarioPorId(MiniRede& rede, int id, ostream& saida) {
    // TODO
}

void buscarUsuarioPorUsername(MiniRede& rede, const char username[], ostream& saida) {
    // TODO
}

void listarUsuarios(MiniRede& rede, ostream& saida) {
    // TODO
}

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, ostream& saida) {
    // TODO
}

void listarSeguindo(MiniRede& rede, int idUsuario, ostream& saida) {
    // TODO
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

