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
    q->array = malloc(sizeof(int *)*cap);
    return q;
};

//Adiciona um elemento ao array
void addToArray (struct SmartArray *q, int elem){
    if(q->tam == q->cap){
        q->array = realloc(q->array,sizeof(int *)*(q->cap*2));
        q->cap = q->cap*2;
        }
    q->array[q->tam] = elem;
    q->tam++;  
}

struct SmartArray* dupSmartArray(struct SmartArray *array){
    int i;
    struct SmartArray* q = initSmartArray(array->tam);
    for(i=0;i<array->tam;i++) addToArray(q,array->array[i]);
    return q;
};

//Função que dá free ao array
void freeSmartArray(struct SmartArray *array){
   free(array->array);
}

//Print array
void printArray(struct SmartArray *q){
    int i;
    for(i = 0;i<q->tam;i++)
        printf("%d ",q->array[i]);
    printf("\n");
}

//Sorts the array with InsertionSort
void SarrayInsertionSort(struct SmartArray *q){
    int i, key, j;
    for (i = 1; i < q->tam; i++) {
        key = q->array[i];
        j = i - 1;

        while (j >= 0 && q->array[j] > key) {
            q->array[j + 1] = q->array[j];
            j = j - 1;
        }
        q->array[j + 1] = key;
    }
}

int isSorted(struct SmartArray *q){
    int i;
    for(i=1;i<q->tam;i++){
        if(q->array[i]<q->array[i-1]){
            printf("Array not ordenado\n");
            return 0;
        }
    }
        printf("Array ordenado\n");
        return 1;
}

//Tira o maior elemento do array
int getMax(struct SmartArray *q){
    int i;
    int max = q->array[0];
    for(i=1;i<q->tam;i++)
        if(q->array[i]>max) max=q->array[i];
    return max;
}

//Auxiliar para o radixSort
void countSort(int arr[], int n, int exp)
{
    int *output = malloc(sizeof(int *)*n); // output array
    int i, count[10] = { 0 };
 
    // Store count of occurrences in count[]
    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;
 
    // Change count[i] so that count[i] now contains actual
    //  position of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];
 
    // Build the output array
    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
 
    for (i = 0; i < n; i++)
        arr[i] = output[i];
    free(output);
}

//Sort com radixsort
void SarrayRadixSort(struct SmartArray *q)
{
    int m = getMax(q);
    int exp;
 
    for (exp = 1; m / exp > 0; exp *= 10)
        countSort(q->array, q->tam, exp);
}