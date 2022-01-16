#ifndef SMARTARRAY_H
#define SMARTARRAY_H

//Estrutura de dados
struct SmartArray{
    int tam;
    int *array;
    int cap;
};

//Função que inicializa o array
struct SmartArray* initSmartArray();

//Função que dá free ao array
void freeSmartArray(struct SmartArray *array);
//Adiciona um elemento ao array
void addToArray (struct SmartArray *q, int elem);

void printArray(struct SmartArray *q);

struct SmartArray* dupSmartArray(struct SmartArray *array);

//Sorts the array with InsertionSort
void SarrayInsertionSort(struct SmartArray *q);

void SarrayRadixSort(struct SmartArray *q);

int isSorted(struct SmartArray *q);

int getMax(struct SmartArray *q);

#endif