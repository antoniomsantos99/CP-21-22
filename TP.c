#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "SmartArray.h"

void bucketSortPar(int nBuckets, struct SmartArray *unsorted)
{

    
    struct SmartArray buckets[nBuckets];
    int maxValue = getMax(unsorted);
    int interval = maxValue / nBuckets;
    int bucketNo,i,j,n,m;

    for (i = 0; i < nBuckets; i++)
        buckets[i] = *initSmartArray(1);

    for (i = 0; i < unsorted->tam; i++)
    {
        bucketNo = (unsorted->array[i]) / interval;
        if (bucketNo >= nBuckets)
            bucketNo = nBuckets-1;
        addToArray(&buckets[bucketNo], (*unsorted).array[i]);
    }
    
    #pragma omp parallel num_threads(nBuckets)
    {
    #pragma omp for
    for (j = 0; j < nBuckets; j++)
    {
        int id = omp_get_thread_num();
        printf("T%d:i%d\n", id, j);
        SarrayRadixSort(&buckets[j]);
    };
    }

    int index = 0;
    for (n = 0; n < nBuckets; n++)
    {
        for (m = 0; m < (&buckets[n])->tam; m++)
        {
            unsorted->array[index] = (&buckets[n])->array[m];
            index++;
        }
    }

    for (j = 0; j < nBuckets; j++)
    {
        freeSmartArray(&buckets[j]);
    };
}




void bucketSortSeq(int nBuckets, struct SmartArray *unsorted)
{
    struct SmartArray buckets[nBuckets];
    int maxValue = getMax(unsorted);
    int interval = maxValue / nBuckets;
    int bucketNo,i,j,n,m;

    for (i = 0; i < nBuckets; i++)
        buckets[i] = *initSmartArray(1);

    for (i = 0; i < unsorted->tam; i++)
    {
        bucketNo = (unsorted->array[i]) / interval;
        if (bucketNo >= nBuckets)
            bucketNo = nBuckets-1;
       // printf("%d - %d | %d\n",bucketNo,buckets[bucketNo].tam,(buckets[bucketNo].cap));
        addToArray(&buckets[bucketNo], (*unsorted).array[i]);
    }

    for (j = 0; j < nBuckets; j++)
    {
        SarrayRadixSort(&buckets[j]);
    };

    int index = 0;
    for (n = 0; n < nBuckets; n++)
    {
        for (m = 0; m < (&buckets[n])->tam; m++)
        {
            unsorted->array[index] = (&buckets[n])->array[m];
            index++;
        }
    }

    for (j = 0; j < nBuckets; j++)
    {
        freeSmartArray(&buckets[j]);
    };
}

struct SmartArray genRandomArray(int size, int range)
{
    int i;
    srand(2021);
    struct SmartArray array = *initSmartArray(size);
    for (i = 0; i < size; i++)
        addToArray(&array, rand() % (range + 1));
    return array;
}

int main(int argc, char *argv[])
{
    // Valores por defeito
    int nExecs = 10;
    int arraySize = 1000000;
    int nBuckets = 16;

    if (argc > 1) // Number of executions
        nExecs = atoi(argv[1]);
    if (argc > 2) // Array Size
        arraySize = atoi(argv[2]);
    if (argc > 3) // nBuckets+Threads
        nBuckets = atoi(argv[3]);

    printf("Hello World\n");
    struct SmartArray t = genRandomArray(arraySize, arraySize);
    struct SmartArray unordered;
    int i;
    for(i=0;i<nExecs;i++){
        isSorted(&t);
        bucketSortPar(nBuckets, &t);
        isSorted(&t);
    }
    //printArray(&t);

    //printArray(&t);
    freeSmartArray(&t);
}