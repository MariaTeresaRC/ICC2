#ifndef LISTA_H
#define LISTA_H

#define TRUE 1
#define FALSE 0
#define ERRO -24000
typedef int boolean;

typedef struct node_ NODE;
typedef struct lista_ LISTA;

LISTA* lista_criar();
int lista_tamanho(LISTA* lista);
boolean lista_esta_vazia(LISTA* lista);
boolean lista_inserir(LISTA* lista, char* palavra);
void lista_print_tres(LISTA* lista);
void lista_print(LISTA* lista);
int lista_atualizar_indices(LISTA* lista);
void lista_busca(LISTA* lista, char* palavra);
boolean lista_apagar(LISTA **lista);

#endif