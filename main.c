//
//  main.c
//  teste
//
//  Created by Hugo Santos Piauilino Neto on 19/06/16.
//  Copyright © 2016 Hugo Santos Piauilino Neto. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#define num_bucket 10

struct timeval tv1, tv2;

typedef struct {
    int topo;
    int* balde;
}bucket;

void bucketSort(int v[],int tam, int type);
int cmpfunc (const void * a, const void * b);
void insertionSort(int * vetorDesordenado, int tamanhoVetor );
void mergeSort(int * vetor, int posicaoInicio, int posicaoFim);
void heapSort(int * a, int n);

void insertionSort(int * vetorDesordenado, int tamanhoVetor ){
   int i, j, valorAtual;

   for( j=1; j < tamanhoVetor; j++ )
   {
      valorAtual = vetorDesordenado[j];
      i = j-1;

      while(i >= 0 && vetorDesordenado[i] > valorAtual)
      {
        vetorDesordenado[i+1] = vetorDesordenado[i];
        i--;
      }

      vetorDesordenado[i+1] = valorAtual;
   }
}

void mergeSort(int * vetor, int posicaoInicio, int posicaoFim) {
    int i, j, k, metadeTamanho, *vetorTemp;

    if(posicaoInicio == posicaoFim) return;

    // ordenacao recursiva das duas metades
    metadeTamanho = (posicaoInicio + posicaoFim ) / 2;
    mergeSort(vetor, posicaoInicio, metadeTamanho);
    mergeSort(vetor, metadeTamanho + 1, posicaoFim);

    // intercalacao no vetor temporario t
    i = posicaoInicio;
    j = metadeTamanho + 1;
    k = 0;
    vetorTemp = (int *) malloc(sizeof(int) * (posicaoFim - posicaoInicio + 1));

    while(i < metadeTamanho + 1 || j  < posicaoFim + 1) {
        if (i == metadeTamanho + 1 ) { // i passou do final da primeira metade, pegar v[j]
            vetorTemp[k] = vetor[j];
            j++;
            k++;
        }
        else {
            if (j == posicaoFim + 1) { // j passou do final da segunda metade, pegar v[i]
                vetorTemp[k] = vetor[i];
                i++;
                k++;
            }
            else {
                if (vetor[i] < vetor[j]) {
                    vetorTemp[k] = vetor[i];
                    i++;
                    k++;
                }
                else {
                    vetorTemp[k] = vetor[j];
                    j++;
                    k++;
                }
            }
        }

    }
    // copia vetor intercalado para o vetor original
    for(i = posicaoInicio; i <= posicaoFim; i++) {
        vetor[i] = vetorTemp[i - posicaoInicio];
    }
    free(vetorTemp);
}

void heapSort(int * a, int n) {
   int i = n / 2, pai, filho, t;
   for (;;) {
      if (i > 0) {
          i--;
          t = a[i];
      } else {
          n--;
          if (n == 0) return;
          t = a[n];
          a[n] = a[0];
      }
      pai = i;
      filho = i * 2 + 1;
      while (filho < n) {
          if ((filho + 1 < n)  &&  (a[filho + 1] > a[filho]))
              filho++;
          if (a[filho] > t) {
             a[pai] = a[filho];
             pai = filho;
             filho = pai * 2 + 1;
          } else {
             break;
          }
      }
      a[pai] = t;
   }
}

void bucketSort(int * v,int tam, int type){
    bucket b[num_bucket];

    int i,j,k;

    for(i=0;i<num_bucket;i++){ //inicializa todos os "topo"
        b[i].topo=0;
        b[i].balde = (int*)malloc(sizeof(int) * tam);
        if(!(b[i].balde)){
            printf("Não conseguiu alocar int[%d]\n",tam);
            exit(1);
        }
    }

    //printf("teste");

    for(i=0;i<tam;i++){ //verifica em que balde o elemento deve ficar
        j=(num_bucket)-1;
        while(1){
            if(j<0)
                break;
            if(v[i]>=j*10){
                b[j].balde[b[j].topo] = v[i];
                (b[j].topo)++;
                break;
            }
            j--;
        }
    }

    for(i=0;i<num_bucket;i++){                     //ordena os baldes
        if(b[i].topo){
            switch (type) {
                case 1:
                    insertionSort(b[i].balde, b[i].topo);
                    break;
                case 2:
                    //mergesort(b[i].balde, b[i].topo, sizeof(int), cmpfunc);
                    mergeSort(b[i].balde, 0, b[i].topo);
                    break;
                case 3:
                    //heapsort(b[i].balde, b[i].topo, sizeof(int), cmpfunc);
                    heapSort(b[i].balde, b[i].topo);
                    break;
                case 4:
                    qsort(b[i].balde, b[i].topo, sizeof(int), cmpfunc);
                    break;
            }
        }
    }

    i=0;

    for(j=0;j<num_bucket;j++){                    //põe os elementos dos baldes de volta no vetor
        for(k=0;k<b[j].topo;k++){
            v[i]=b[j].balde[k];
            i++;
        }
    }

    for(i=0;i<num_bucket;i++){ //libera memoria"
        free(b[i].balde);
    }

}

int cmpfunc (const void * a, const void * b){
    return ( *(int*)a - *(int*)b );
}

int main(int argc, const char * argv[]){

    int tam[9] = {100, 500, 1000, 5000, 30000, 80000, 100000, 150000, 200000}; //Vetor com os tamanhos de vetores para testes
    int i, j, k, qntd = 0;
    int simulations = 20;

    for(j = 0; j < 9; j++){

        qntd = tam[j]; //Armazena o tamanho do vetor escolhido

        int* vetI = (int*)malloc(sizeof(int) * qntd); //Cria o vetor com o tamanho escolhido
        int* vetM = (int*)malloc(sizeof(int) * qntd);
        int* vetH = (int*)malloc(sizeof(int) * qntd);
        int* vetQ = (int*)malloc(sizeof(int) * qntd);

        double mediaI = 0;
        double mediaM = 0;
        double mediaH = 0;
        double mediaQ = 0;

        for(k = 0; k < simulations; k++){

            srand(time(NULL)); //Reinicia a semente aleatória
            for(i = 0; i < qntd; i++){ //Percorre todo o vetor adicionado os valores aleatórios
                vetI[i] = vetM[i] = vetH[i] = vetQ[i] = (rand()%qntd); //Armazena no vetor o valor aleatório gerado
                //printf("%d ", vetI[i]);
            }
            //printf("\n");
            //printf("Iniciando simulacao\n");

            //INSERTION SORT
            gettimeofday(&tv1, NULL);

            bucketSort(vetI, qntd, 1);

            gettimeofday(&tv2, NULL);

            mediaI += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec);


            //MERGE SORT
            gettimeofday(&tv1, NULL);

            bucketSort(vetM, qntd, 2);

            gettimeofday(&tv2, NULL);

            mediaM += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec);


            //HEAP SORT
            gettimeofday(&tv1, NULL);

            bucketSort(vetH, qntd, 3);

            gettimeofday(&tv2, NULL);

            mediaH += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec);


            //QUICK SORT
            gettimeofday(&tv1, NULL);

            bucketSort(vetQ, qntd, 4);

            gettimeofday(&tv2, NULL);

            mediaQ += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec);

        }

        mediaI = mediaI/simulations;
        mediaM = mediaM/simulations;
        mediaH = mediaH/simulations;
        mediaQ = mediaQ/simulations;

        free(vetI);
        free(vetM);
        free(vetH);
        free(vetQ);

        printf("Media %d: %f %f %f %f\n", j, mediaI, mediaM, mediaH, mediaQ);

//        for(i = 0; i < qntd; i++){ //Percorre todo o vetor adicionado os valores aleatórios
//            printf("%d ", vetI[i]);
//        }
//        printf("\n");
//        for(i = 0; i < qntd; i++){ //Percorre todo o vetor adicionado os valores aleatórios
//            printf("%d ", vetM[i]);
//        }
//        printf("\n");
//        for(i = 0; i < qntd; i++){ //Percorre todo o vetor adicionado os valores aleatórios
//            printf("%d ", vetH[i]);
//        }
//        printf("\n");
//        for(i = 0; i < qntd; i++){ //Percorre todo o vetor adicionado os valores aleatórios
//            printf("%d ", vetQ[i]);
//        }
//        printf("\n");

    }

    return(0);
}
