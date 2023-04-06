/* SCC0201 - Introdução à Ciências da Computação II
 * Trabalho 01: Campo Minado
 *
 * Aluno: Maria Teresa Raetano Cardoso
 * n USP: 10748604
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tabuleiro_{          // Struct que armazenará o tabuleiro
    char** campo;                   // Contendo a matriz de char que é o tabuleiro propriamente dito
    int linhas, colunas;            // E também o número de linhas e colunas, ou seja,
}TABULEIRO;                         // o tamanho do tabuleiro

/* ler_tabuleiro
 * Funcao responsavel por fazer a leitura do tabuleiro do arquivo .board
 * Aloca a matriz dinamicamente, aumentando o número de linhas conforme o necessario
 * 
 * Parametros:
 *      char* nome_arquivo - nome do arquivo que contem o tabuleiro
 * Retorno:
 *      TABULEIRO* tabuleiro - ponteiro apontando para a struct tabuleiro gerada
 */
TABULEIRO* ler_tabuleiro(char* nome_arquivo){
    FILE* ft = fopen(nome_arquivo, "r+");

    char linha_aux[50];     //variavel auxiliar usada para ler as linhas da matriz
    TABULEIRO* tabuleiro = (TABULEIRO*)malloc(sizeof(TABULEIRO));
    tabuleiro->linhas = 0;
    tabuleiro->colunas = 0;
    tabuleiro->campo = (char**)malloc(10*sizeof(char*));    //a principio a matriz tem 10 linhas

    while (0<fscanf(ft, " %[^(\r|\n)]", linha_aux)){
        tabuleiro->campo[tabuleiro->linhas] = (char*)malloc(strlen(linha_aux)*sizeof(char));
        strcpy(tabuleiro->campo[tabuleiro->linhas], linha_aux);
        tabuleiro->linhas++;
        if(tabuleiro->linhas%10==0){
            tabuleiro->campo = (char**)realloc(tabuleiro->campo, (tabuleiro->linhas/10 + 1)*10*sizeof(char*));
            //adiciona 10 linhas a matriz caso seja necessario
        }
    }
    tabuleiro->colunas = strlen(linha_aux);

    fclose(ft);

    return tabuleiro;
}

void print_tabuleiro(TABULEIRO* tabuleiro){
    for(int i=0; i<tabuleiro->linhas; i++){
        for(int j=0; j<tabuleiro->colunas; j++){
            printf("%c", tabuleiro->campo[i][j]);
        }
        printf("\n");
    }
}

/* apaga_tabuleiro
 * Função para liberar a memoria utilizada pelo tabuleiro
 */
void apaga_tabuleiro(TABULEIRO* tabuleiro){
    for(int i=0; i<tabuleiro->linhas; i++){
        free(tabuleiro->campo[i]);
    }
    free(tabuleiro->campo);
    free(tabuleiro);
}

/* tabuleiro_padding
 * Funcao que cria um novo tabuleiro, com uma "camada" a mais 
 * para facilitar a aplicacao da funcao que produz as dicas.
 * Cria-se uma matriz com uma borda externa, assim o algoritmo
 * das dicas sera utilizado apenas na matriz "interna" e nao precisaremos
 * tratar das bordas da matriz original separadamente.
 * 
 * Parametros:
 *      TABULEIRO* tabuleiro - tabuleiro original
 * Retorno:
 *      TABULEIRO* novo_tabuleiro - tabuleiro com padding, ou seja, com a borda vazia
 */
TABULEIRO* tabuleiro_padding(TABULEIRO* tabuleiro){
    TABULEIRO* novo_tabuleiro = (TABULEIRO*)malloc(sizeof(TABULEIRO));
    novo_tabuleiro->linhas = tabuleiro->linhas + 2;
    novo_tabuleiro->colunas = tabuleiro->colunas + 2;
    novo_tabuleiro->campo = (char**)malloc(novo_tabuleiro->linhas*sizeof(char*));
    for(int i=0; i<novo_tabuleiro->linhas; i++){
        novo_tabuleiro->campo[i] = (char*)calloc(novo_tabuleiro->colunas,sizeof(char));
    }
    for(int i=1; i<novo_tabuleiro->linhas-1; i++){
        for(int j=1; j<novo_tabuleiro->colunas-1; j++){
            novo_tabuleiro->campo[i][j] = tabuleiro->campo[i-1][j-1];
        }
    }
    return novo_tabuleiro;
}

/* adiciona_dica
 * Funcao que será chamada na funcao dicas abaixo, adiciona uma dica na posicao dada na entrada
 *
 * Parametro:
 *      TABULEIRO* tabuleiro - tabuleiro a receber a dica
 *      int i, int j - posicao do quadrado a receber a dica
 */
void adiciona_dica(TABULEIRO* tabuleiro, int i, int j){
    if(tabuleiro->campo[i][j]== '.'){
       tabuleiro->campo[i][j] = '1';
    }else if(tabuleiro->campo[i][j]>'0') tabuleiro->campo[i][j]++;
}

/* dicas
 * Funcao que modifica o tabuleiro para que ele mostre as dicas do campo minado.
 * Deve receber o tabuleiro com padding.
 * Percorre a matriz interna e quando encontra uma bomba, adiciona dica nos quadrados em volta.
 * 
 * Parametros:
 *      TABULEIRO* tabuleiro - tabuleiro a receber as dicas. Deve ser o tabuleiro com padding.
 */
void dicas(TABULEIRO* tabuleiro){
    for(int i=1; i<tabuleiro->linhas-1; i++){
        for(int j=1; j<tabuleiro->colunas-1; j++){
            if(tabuleiro->campo[i][j]=='*'){
                adiciona_dica(tabuleiro, i-1, j);
                adiciona_dica(tabuleiro, i-1, j+1);
                adiciona_dica(tabuleiro, i, j+1);
                adiciona_dica(tabuleiro, i+1, j+1);
                adiciona_dica(tabuleiro, i+1, j);
                adiciona_dica(tabuleiro, i+1, j-1);
                adiciona_dica(tabuleiro, i, j-1);
                adiciona_dica(tabuleiro, i-1, j-1);
            }
        }
    }
}

/* print_sem_padding
 * Recebe uma matriz que contem padding e imprime apenas a matriz interna
 */
void print_sem_padding(TABULEIRO* tabuleiro){
    for(int i=1; i<tabuleiro->linhas-1; i++){
        for(int j=1; j<tabuleiro->colunas-1; j++){
            printf("%c", tabuleiro->campo[i][j]);
        }
        printf("\n");
    }
}

/* revela_dica
 * Funcao que sera usada no controle de usuario para revelar as dicas de acordo com a posicao selecionada
 * Recebe o tabuleiro que contem as dicas (feito pela funcao dicas) e o "tabuleiro vazio", que é um tabuleiro
 * marcado apenas com 'X' a principio
 * Recebe tambem a posicao (x,y) a ser revelada
 * Se a posicao contem uma dica, coloca essa dica no tabuleiro vazio e retorna
 * Se a posicao contem um espaco vazio, revela esse espaco vazio, mas tambem chama ela mesma em todos os quadrados
 * em volta, ate encontrar uma dica, sendo assim um efeito de 'flood fill'
 * 
 * Parametros:
 *      TABULEIRO* tabuleiro - ponteiro para o tabuleiro com a matriz com dicas (com padding)
 *      TABULEIRO* tabuleiro - ponteiro para o tabuleiro vazio, sera alterado por essa funcao
 *      int x, int y - posicao a ser revelada 
 */
void revela_dica(TABULEIRO* tabuleiro, TABULEIRO* tabuleiro_vazio, int x, int y){
    if(tabuleiro->campo[x+1][y+1]>'0' && tabuleiro_vazio->campo[x+1][y+1]=='X'){
        tabuleiro_vazio->campo[x+1][y+1]=tabuleiro->campo[x+1][y+1];
        return;
    }
    if(tabuleiro->campo[x+1][y+1]=='.' && tabuleiro_vazio->campo[x+1][y+1]=='X'){
        tabuleiro_vazio->campo[x+1][y+1]=tabuleiro->campo[x+1][y+1];
        if(x=='0' || y=='0' || x==(tabuleiro->linhas-2) || y==(tabuleiro->colunas-2)){
            return;
        }
        revela_dica(tabuleiro, tabuleiro_vazio, x-1, y);
        revela_dica(tabuleiro, tabuleiro_vazio, x-1, y+1);
        revela_dica(tabuleiro, tabuleiro_vazio, x, y+1);
        revela_dica(tabuleiro, tabuleiro_vazio, x+1, y+1);
        revela_dica(tabuleiro, tabuleiro_vazio, x+1, y);
        revela_dica(tabuleiro, tabuleiro_vazio, x+1, y-1);
        revela_dica(tabuleiro, tabuleiro_vazio, x, y-1);
        revela_dica(tabuleiro, tabuleiro_vazio, x-1, y-1);
    }
}

/* controle_usuario
 * Primeiramente, cria o tabuleiro vazio que sera usado para revelar as dicas e preenche com 'X'
 * Em seguida, printa o tabuleiro de acordo com a acao do usuario:
 *  Se foi selecionada uma bomba, apenas printa o tabuleiro com as dicas, assim como a opcao 2
 *  Se foi selecionada uma dica ou um espaco vazio, chama a funcao revela_dica e em seguida 
 *  printa o "tabuleiro vazio" modificado, com as dicas reveladas com base na posicao dada
 * Por fim, chama a funcao apaga_tabuleiro para desalocar a memoria utilizada pelo tabuleiro_vazio
 * 
 * Parametros:
 *      TABULEIRO* tabuleiro - tabuleiro que contem as dicas (com padding)
 *      int x, int y - posicao dada pelo usuario
 */
void controle_usuario(TABULEIRO* tabuleiro, int x, int y){
    TABULEIRO* tabuleiro_vazio = (TABULEIRO*)malloc(sizeof(TABULEIRO));
    tabuleiro_vazio->linhas = tabuleiro->linhas;
    tabuleiro_vazio->colunas = tabuleiro->colunas;
    tabuleiro_vazio->campo = (char**)malloc(tabuleiro_vazio->linhas*sizeof(char*));
    for(int i=0; i<tabuleiro_vazio->linhas; i++){
        tabuleiro_vazio->campo[i] = (char*)calloc(tabuleiro_vazio->colunas,sizeof(char));
    }
    for(int i=0; i<tabuleiro_vazio->linhas; i++){
        for(int j=0; j<tabuleiro_vazio->colunas; j++){
            tabuleiro_vazio->campo[i][j]='X';
        }
    }
    if(tabuleiro->campo[x+1][y+1]=='*') print_sem_padding(tabuleiro);
    if(tabuleiro->campo[x+1][y+1]>'0' || tabuleiro->campo[x+1][y+1]=='.'){
        revela_dica(tabuleiro, tabuleiro_vazio, x, y);
        print_sem_padding(tabuleiro_vazio);
    }
    apaga_tabuleiro(tabuleiro_vazio);
}

int main(){

    char nome_arquivo[10];
    int opcao;
    int pos_x, pos_y;       //receberao a posicao escolhida pelo usuario na opcao 3

    scanf(" %d", &opcao);
    scanf(" %s", nome_arquivo);

    TABULEIRO* tabuleiro = ler_tabuleiro(nome_arquivo);
    TABULEIRO* novo_tabuleiro = tabuleiro_padding(tabuleiro);
    dicas(novo_tabuleiro);      //novo_tabuleiro agora contém a matriz com as dicas
    switch (opcao){
    case 1:
        print_tabuleiro(tabuleiro);
        break;
    case 2:
        print_sem_padding(novo_tabuleiro);
        break;
    case 3:
        scanf("%d %d", &pos_x, &pos_y);
        controle_usuario(novo_tabuleiro, pos_x, pos_y);
        break;
    }

    apaga_tabuleiro(novo_tabuleiro);
    apaga_tabuleiro(tabuleiro);

    return 0;
}