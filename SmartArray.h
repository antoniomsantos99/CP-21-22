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

#endif