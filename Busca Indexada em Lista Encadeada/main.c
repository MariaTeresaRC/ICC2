/* SCC0201 - Introdução à Ciências da Computação II
 * Exercício 03: Busca Indexada com Lista Encadeada
 *
 * Aluno: Maria Teresa Raetano Cardoso
 * n USP: 10748604
 */

#include <stdio.h>
#include <stdlib.h>

#include "lista.h"

int main(){

    int comando;
    int atualizado = 0;    //flag para checar se o vetor de indices esta atualizado
    char* arquivo = (char*)malloc(12*sizeof(char));
    char* palavra = (char*)malloc(15*sizeof(char));
    LISTA* lista = lista_criar();

    while(scanf(" %d", &comando)>0){
        switch(comando){
        case 0:
            lista_apagar(&lista);
            break;
        case 1:
            scanf(" %s", arquivo);
            FILE* fp = fopen(arquivo, "r+");
            while(fscanf(fp, " %s", palavra)>0){
                lista_inserir(lista, palavra);
            }
            fclose(fp);
            lista_print_tres(lista);
            atualizado = 0;
            break;
        case 2:
            printf("%d\n", lista_atualizar_indices(lista));
            atualizado = 1;
            break;
        case 3:
            scanf(" %s", palavra);
            if(atualizado == 0){
                printf("Vetor de indices nao atualizado.\n");
            }else{
                lista_busca(lista, palavra);
            }
            break;
        }
    }

    free(arquivo);
    free(palavra);

    return 0;
}