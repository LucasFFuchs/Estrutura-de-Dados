#ifndef MINI_REDE_H
#define MINI_REDE_H

#include <iostream>
using namespace std;

const int TAM_USERNAME = 50;
const int TAM_NOME = 100;
const int TAM_TEXTO = 280;
const int TAM_COMANDO = 30;

struct Usuario;
struct Publicacao;

struct Usuario{
    int id;
    string name;
    string username;

    Usuario* esq;
    Usuario* dir;

    int FB;

    Usuario* prox;
    Usuario* prox_seguindo;
    Usuario* prox_curtiram;
    
    Usuario* quemSigo;
    Publicacao* publicacoes;
};

struct Publicacao{
    Publicacao* prox;
    int timestamp;
    int id;
    int id_autor;
    string texto;
    int likes;
    Usuario* curtidos;
};

struct Notificacao{
    Notificacao* prox;
    
};

struct Fila{
    Notificacao* front;
}; 



struct MiniRede {
    Usuario* arvore;
    Usuario* hash[10];
};

void inicializarMiniRede(MiniRede& rede);
void liberarMiniRede(MiniRede& rede);
void processarComandos(MiniRede& rede, std::istream& entrada, std::ostream& saida);

void cadastrarUsuario(MiniRede& rede, int id, string username, string nomeCompleto, std::ostream& saida);
void buscarUsuarioPorId(MiniRede& rede, int id, std::ostream& saida);
void buscarUsuarioPorUsername(MiniRede& rede, string username, std::ostream& saida);
void listarUsuarios(MiniRede& rede, std::ostream& saida);

void seguirUsuario(MiniRede& rede, int idSeguidor, int idSeguido, std::ostream& saida);
void listarSeguindo(MiniRede& rede, int idUsuario, std::ostream& saida);

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, const char texto[], std::ostream& saida);
void curtirPublicacao(MiniRede& rede, int idUsuario, int idPost, std::ostream& saida);

void consultarNotificacoes(MiniRede& rede, int idUsuario, int k, std::ostream& saida);
void gerarFeed(MiniRede& rede, int idUsuario, int k, std::ostream& saida);
void listarTopPosts(MiniRede& rede, int k, std::ostream& saida);

// TODO: declarar aqui as funcoes auxiliares escolhidas pelo grupo.
//
// Exemplos de responsabilidades auxiliares:
// - buscar usuario por id
// - buscar usuario por username
// - buscar publicacao por id
// - inserir/listar/liberar arvore
// - inserir/buscar/liberar tabela hash
// - enfileirar/desenfileirar notificacoes
// - manipular listas encadeadas
// - ordenar vetores auxiliares para feed e ranking

#endif
