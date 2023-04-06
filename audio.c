/* SCC0201 - Introdução à Ciências da Computação II
 * Trabalho 01: Compressão de Áudio
 *
 * Aluno: Maria Teresa Raetano Cardoso
 * n USP: 10748604
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <math.h>

/* A funcao read_wav_data foi previamente dada.
 * Realiza a leitura do arquivo e retorna os dados num vetor de unsigned char.
 * Foi feita uma pequena modificacao para obter o tamanho do arquivo lido na main.
 */
unsigned char* read_wav_data(char* fname, int* dataSize) {
    FILE* fp = fopen(fname, "r+");
    unsigned char buf4[4];

    fseek(fp, 40, SEEK_SET);
    fread(buf4, sizeof(buf4), 1, fp);
    *dataSize = buf4[0] | buf4[1]<<8 | buf4[2]<<16 | buf4[3]<<24;

    unsigned char* data = (unsigned char*)malloc(sizeof(*data) * (*dataSize));
    
    int i = 0;
    while (i < *dataSize) {
        fread(&data[i++], sizeof(unsigned char), 1, fp);
    }
    fclose(fp);
    return data;
}

/* A funcao DFT tambem foi previamente dada.
 * Recebe o vetor de dados do arquivo de audio e retorna um vetor com os 
 * coeficientes apos a Transformada de Fourier. 
 */
double complex *DFT(unsigned char *audio, int length) {
    double complex *coef = (double complex *) calloc(length, sizeof(double complex));

    for (int k = 0; k < length; k++) {
        for (int n = 0; n < length; n++) {
            coef[k] += audio[n] * cexp((-2.0 * M_PI * (((k+1) * n * 1.0) / (length * 1.0))) * _Complex_I);
        }
        // printf("%.1lf + %.1lfi\n", creal(coef[k]), cimag(coef[k]));
    }
    return coef;
}

/* struct dados
 * Contem o coeficiente complexo resultante da Transformada, 
 * a magnitude e a posicao original no vetor inicial.
 */
typedef struct dados{
    double complex coef;
    int magnitude;
    int posicao;
}DADOS;

/* mergesort
 * Funcao para ordenar o vetor de dados em ordem crescente, de acordo com um dos parametros fornecidos na entrada.
 * 
 * Parametros:
 *      DADOS* v - o vetor da struct dados, contem os numeros que devem ser ordenados
 *      int ini, int fim - posicoes inicial e final da ordenacao
 *      int type - indica o parametro a ser considerado na ordenacao: 1 - magnitude; 2 - posicao
 */
void mergesort(DADOS* v, int ini, int fim, int type){
    if(fim<=ini) return;
    int c = (ini+fim)/2;

    mergesort(v, ini, c, type);
    mergesort(v, c+1, fim, type);

    DADOS* aux = (DADOS*)malloc((fim-ini+1)*sizeof(DADOS));

    int i = ini;
    int j = c+1;
    int k = 0;

    switch (type){
    case 1:
        while(i <= c && j <= fim){
            if(v[i].magnitude <= v[j].magnitude){
                aux[k] = v[i];
                i++;
            }else{
                aux[k] = v[j];
                j++;
            }
            k++;
        }
        break;
    
    case 2:
        while (i <= c && j <= fim) {
            if(v[i].posicao <= v[j].posicao){
                aux[k] = v[i];
                i++;
            }else{
                aux[k] = v[j];
                j++;
            }
            k++;
        }
        break;
    }

    while(i <= c){
		aux[k] = v[i];
		i++;
        k++;
	}

    while(j <= fim){
		aux[k] = v[j];
		j++; 
        k++;
	}

    for(i = ini, k = 0; i <= fim; i++, k++){
		v[i] = aux[k];
	}

	free(aux);
}

/* inv_DFT
 * Inversa da Transformada Discreta de Fourier.
 * Recebe o vetor de coeficientes complexos e seu tamanho, e transforma em um vetor de unsigned char.
 * 
 * Parametros:
 *      double complex *coef - vetor de coeficientes complexos
 *      int tamanho - tamanho do vetor de coeficientes
 */
unsigned char* inv_DFT(double complex *coef, int tamanho) {
    unsigned char *audio = (unsigned char*)calloc(tamanho, sizeof(unsigned char));
    double complex aux=0;

    for (int n = 0; n < tamanho; n++) {
        for (int k = 0; k < tamanho; k++) {
            aux += coef[k]*cexp((2.0 * M_PI * ((k * n * 1.0) / (tamanho * 1.0))) * _Complex_I);
        }
        audio[n] = (int)creal(aux/tamanho);
        aux = 0;
    }
    return audio;
}

int main(){

    char nome_arquivo[10];  
    int num_coef, tamanho;  //num_coef e o numero de coeficientes que nao serao zerados, de acordo com suas magnitudes, dado na entrada

    scanf(" %s", nome_arquivo);    
    scanf(" %d", &num_coef);

    unsigned char* data = read_wav_data(nome_arquivo, &tamanho);

    printf("%d\n", tamanho);

    double complex* coef = DFT(data, tamanho);

    int count = 0;

    DADOS* vet_magnitudes = (DADOS*)malloc(tamanho*sizeof(DADOS));          // Cria o vetor da struct DADOS a ser usado na ordenacao

    for(int i=0; i<tamanho; i++){                                           // Preenche o vetor com base no vetor de coeficientes da DTF
        vet_magnitudes[i].coef = coef[i];
        vet_magnitudes[i].magnitude = (int)sqrt(pow(creal(coef[i]), 2)+pow(cimag(coef[i]), 2));
        vet_magnitudes[i].posicao = i;
        if(creal(coef[i])<=0 && cimag(coef[i])<=0) count++;                 // count representa o numero de coeficientes com tanto a parte real
    }                                                                       // e a imaginaria menores ou iguais a zero, como pedido no enunciado

    printf("%d\n", count);

    mergesort(vet_magnitudes, 0, tamanho-1, 1);     //ordena o vetor de DADOS em funcao da magnitude

    for(int i = tamanho-1; i>=0; i--){              //zera os coeficientes exceto os com T maiores magnitudes e tambem printa essas magnitudes
        if(i<tamanho-num_coef){
            vet_magnitudes[i].coef=0;       
        }else printf("%d\n", vet_magnitudes[i].magnitude);      
    }

    mergesort(vet_magnitudes, 0, tamanho-1, 2);     //ordena o vetor de dados em funcao da posicao, ou seja, retorna-o as posicoes originais

    double complex* newCoef = (double complex*)malloc(tamanho*sizeof(double complex));
    for(int i=0; i<tamanho; i++){
        newCoef[i] = vet_magnitudes[i].coef;
    }

    unsigned char* newAudio = inv_DFT(newCoef, tamanho);

    unsigned char* header = malloc(44*sizeof(unsigned char));       

    FILE* f1 = fopen(nome_arquivo, "r+");
    fseek(f1, 0, SEEK_SET);
    fread(header, sizeof(unsigned char), 44, f1);       // copia o header do audio original, a ser usado no novo arquivo
    fclose(f1);

    FILE* f2 = fopen("newAudio.wav", "w");
    fseek(f2, 0, SEEK_SET);
    fwrite(header, sizeof(unsigned char), 44, f2);      //escreve o header no arquivo
    fwrite(newAudio, sizeof(unsigned char), tamanho, f2);   //e finalmente, escreve o vetor de unsigned char que representa o novo audio(comprimido)
    fclose(f2);

    free(data);
    free(coef);
    free(vet_magnitudes);
    free(newCoef);
    free(newAudio);
    free(header);

    return 0;
}