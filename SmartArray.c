#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estrutura de dados
struct SmartArray{
    int tam;
    int *array;
    int cap;
};

//Função que inicializa o array (inicia o array com apenas um espaço livre)
struct SmartArray* initSmartArray(int cap){
    struct SmartArray* q;
    q = malloc(sizeof(struct SmartArray));
    q->tam = 0;
    q->cap= cap;
    q->array = malloc(sizeof(int *));
    return q;
};

//Função que dá free ao array
void freeSmartArray(struct SmartArray *array){
    int i = 0;
    for(i = 0; i < array->tam;i++){
      free(&array->array[i]);
   }
   free(array->array);
}

//Adiciona um elemento ao array
void addToArray (struct SmartArray *q, int elem){
    if(q->tam == q->cap){
        q->array = realloc(q->array,sizeof(int *)*(q ->tam+1));
        q->cap = q->cap*2;
        }
    q->array[q->tam] = elem;
    q->tam++;
}

//Print array
void printArray(struct SmartArray *q){
    for(int i = 0;i<q->tam;i++)
        printf("%d ",q->array[i]);
    printf("\n");
}

