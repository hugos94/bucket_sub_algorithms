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
#define tam_bucket 5000
#define num_bucket 10

struct timeval tv1, tv2;

typedef struct {
    int topo;
    int* balde;
}bucket;

void bucket_sort(int v[],int tam, int type);
int cmpfunc (const void * a, const void * b);
int *insertionsort(int* original, int length);

int *insertionsort(int* original, int length){
    int i, j, atual;

    for (i = 1; i < length; i++){
        atual = original[i];
        j = i - 1;

        while ((j >= 0) && (atual < original[j])){
            original[j + 1] = original[j];
            j = j - 1;
        }

        original[j + 1] = atual;
    }

    return (int*)original;
}


void bucket_sort(int* v,int tam, int type){
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
                    insertionsort(b[i].balde, b[i].topo);
                    break;
                case 2:
                    //mergesort(b[i].balde, b[i].topo, sizeof(int), cmpfunc);
                    break;
                case 3:
                    //heapsort(b[i].balde, b[i].topo, sizeof(int), cmpfunc);
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
    int simulations = 1;


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

            bucket_sort(vetI, qntd, 1);

            gettimeofday(&tv2, NULL);

            mediaI += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec);


            //MERGE SORT
            gettimeofday(&tv1, NULL);

            bucket_sort(vetM, qntd, 2);

            gettimeofday(&tv2, NULL);

            mediaM += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec);


            //HEAP SORT
            gettimeofday(&tv1, NULL);

            bucket_sort(vetH, qntd, 3);

            gettimeofday(&tv2, NULL);

            mediaH += (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec);


            //QUICK SORT
            gettimeofday(&tv1, NULL);

            bucket_sort(vetQ, qntd, 4);

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

        printf("Media: %f %f %f %f\n", mediaI, mediaM, mediaH, mediaQ);

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
