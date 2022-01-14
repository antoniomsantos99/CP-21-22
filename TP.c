#include <stdio.h>
#include <stdlib.h>
#include "SmartArray.h"

void bucketSort(int nBuckets, struct SmartArray unsorted){
    struct SmartArray buckets[nBuckets];
    for(int i = 0; i < nBuckets;i++)
        buckets[i]=*initSmartArray(1);

}

struct SmartArray genRandomArray(int size, int range){
    srand(2);
    struct SmartArray array = *initSmartArray(size);
    for(int i = 0;i<size;i++)
        addToArray(&array,rand()%range);
    return array;
}

int main(){
    printf("Hello World\n");
    struct SmartArray t = genRandomArray(20,50);
    printArray(&t);
    freeSmartArray(&t);
}