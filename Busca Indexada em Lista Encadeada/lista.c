#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_{
    char* palavra;
    NODE* proximo;
    NODE* anterior;
};

struct lista_{
    NODE* inicio;
    NODE* fim;
    int tamanho;
    NODE* vet_indices[26];
};

LISTA* lista_criar(){
    LISTA* lista = (LISTA*)malloc(sizeof(LISTA));
    if(lista != NULL){
        lista->inicio = NULL;
        lista->fim = NULL;
        lista->tamanho = 0;
        for(int i=0; i<26; i++){
            lista->vet_indices[i] = NULL;
        }
    }
    return lista;
}

int lista_tamanho(LISTA* lista){
    if(lista != NULL){
        return lista->tamanho;
    }else return ERRO;
}

boolean lista_esta_vazia(LISTA* lista){
    if(lista != NULL){
        return lista->tamanho == 0;
    }else return ERRO;
}

//Funcao de insercao: insere a palavra na lista na ordem correta
boolean lista_inserir(LISTA* lista, char* palavra){
    if(lista != NULL){
        NODE* newNode = (NODE*)malloc(sizeof(NODE));
        if(newNode != NULL){
            newNode->palavra = (char*)malloc(15*sizeof(char));
            strcpy(newNode->palavra, palavra);
            if(lista_esta_vazia(lista)){    //se a lista esta vazia, a palavra entra como o inicio da lista
                lista->inicio = newNode;
                lista->fim = newNode;
                newNode->proximo = NULL;
                newNode->anterior = NULL;
                lista->tamanho++;
            }else{
                NODE* noAtual = lista->inicio;
                while(noAtual->palavra[0]<palavra[0] && noAtual->proximo != NULL){  //caso contrario, devemos procurar o lugar correto
                    noAtual = noAtual->proximo;
                }
                if(noAtual->palavra[0] >= palavra[0]){      //aqui encontramos uma palavra que comeca com a mesma letra 
                    newNode->anterior = noAtual->anterior;  //ou com outra letra, ou seja, devemos inserir no node anterior
                    noAtual->anterior = newNode;
                    newNode->proximo = noAtual;
                    if(noAtual != lista->inicio){
                        newNode->anterior->proximo = newNode;
                    }else lista->inicio = newNode;          //se este no e o inicio da lista, a nova palavra deve ser o inicio
                }else{
                    newNode->proximo = noAtual->proximo;    //se a condicao anterior nao for cumprida, devemos inserir a palavra
                    noAtual->proximo = newNode;             //no fim da lista
                    newNode->anterior = noAtual;
                    lista->fim = newNode;
                }
                lista->tamanho++;
            }
        }
    }
    return ERRO;
}

//Printa apenas os tres primeiros itens da lista
void lista_print_tres(LISTA* lista){
    NODE* noAtual = lista->inicio;
    for(int i=0; i<3; i++){
        printf("%s\n", noAtual->palavra);
        noAtual = noAtual->proximo;
    }
}

void lista_print(LISTA* lista){
    NODE* noAtual = lista->inicio;
    while(noAtual->proximo != NULL){
        printf("%s\n", noAtual->palavra);
        noAtual = noAtual->proximo;
    }
    printf("%s\n", noAtual->palavra);
}

//Atualiza o vetor de indices, com a variavel count recebendo o numero de elementos nao nulos
int lista_atualizar_indices(LISTA* lista){
    int count = 0;
    if(lista != NULL){
        NODE* noAtual = lista->inicio;
        lista->vet_indices[noAtual->palavra[0]-'a'] = noAtual;
        count++;
        noAtual = noAtual->proximo;
        while(noAtual != lista->fim){
            if(noAtual->palavra[0] != noAtual->anterior->palavra[0]){
                lista->vet_indices[noAtual->palavra[0]-'a'] = noAtual;
                count++;
            }
            noAtual = noAtual->proximo;
        }
        if(noAtual->palavra[0] != noAtual->anterior->palavra[0]){
            lista->vet_indices[noAtual->palavra[0]-'a'] = noAtual;
            count++;
        }
        return count;
    }
    return ERRO; 
}

void lista_busca(LISTA* lista, char* palavra){
    NODE* noAtual = lista->vet_indices[palavra[0]-'a'];
    int count = 0;
    if(noAtual == NULL){
        printf("Palavra nao existe na lista.\n");
    }else{
        while(noAtual->palavra[0] == palavra[0]){
            if(strcmp(noAtual->palavra, palavra)==0){
                printf("%d\n", count);
                break;
            }else{
                noAtual = noAtual->proximo;
                count++;
            }            
        }
        if(noAtual->palavra[0] != palavra[0]){
            printf("Palavra nao existe na lista.\n");
        }
    }
}

void lista_apagar_aux(NODE* node){
    if (node != NULL){
        if(node->proximo != NULL)
        {
            lista_apagar_aux(node->proximo);
        }
        free(node->palavra);
        free(node);
        node = NULL;
    }
}

boolean lista_apagar(LISTA **lista){
    if (*lista == NULL)
        return FALSE;
    lista_apagar_aux((*lista)->inicio);
    free(*lista);
    *lista = NULL;
    return TRUE;
}