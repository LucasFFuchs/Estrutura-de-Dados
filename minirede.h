#ifndef MINI_REDE_H
#define MINI_REDE_H

#include <iostream>
using namespace std;
/*
ADICIONAR NO USUARIO LISTA ENCADEADA DE PUBLICACOES(ACHO QUE TEREI QUE FAZER PUBLICACAO NÓ) 
QUE ELE POSTOU, PARA O FEED SAIR MAIS FACIL, MAS DEIXAR AS PUBLICACOES GERAIS PARA AS TOPS 
MAIS CURTIDAS

REVER USUARIO NÓ, ACHO QUE NAO ESTOU ALOCANDO MEMORIA COM NEW
REVER TODAS FUNCOES QUE PRECISARIAM DE ALOCACAO DE MEMORIA
*/
const int TAM_USERNAME = 50;
const int TAM_NOME = 100;
const int TAM_TEXTO = 280;
const int TAM_COMANDO = 30;

struct Usuario;
struct Publicacao;
struct Notificacao;

struct UsuarioNo{
    UsuarioNo* prox;
    Usuario* user;
};

struct Usuario{
    int id;
    string name;
    string username;

    Usuario* prox; //Para tabela hash
    Usuario* esq;  //Para AVL
    Usuario* dir;

    int FB;

    UsuarioNo* quemSigo;
    Notificacao* frontNotificacoes;
};

struct Publicacao{
    int timestamp;
    int id;
    int id_autor;
    int likes;
    string texto;

    Publicacao* prox;
    UsuarioNo* quemCurtiu;
};

struct Notificacao{
    string texto;
    Notificacao* prox;
};


struct MiniRede {
    Usuario* arvore;
    Usuario* hash[10];
    Publicacao* publicacoes;
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

void cadastrarPublicacao(MiniRede& rede, int idPost, int idAutor, int timestamp, string texto, std::ostream& saida);
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
