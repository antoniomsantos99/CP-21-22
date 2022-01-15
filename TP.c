#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "SmartArray.h"

void bucketSortPar(int nBuckets, struct SmartArray *unsorted)
{
    struct SmartArray buckets[nBuckets];
    int maxValue = getMax(unsorted);
    int interval = maxValue / nBuckets;
    int bucketNo;

    for (int i = 0; i < nBuckets; i++)
        buckets[i] = *initSmartArray(1);

    for (int i = 0; i < unsorted->tam; i++)
    {
        bucketNo = (unsorted->array[i]) / interval;
        if (bucketNo == nBuckets)
            bucketNo = nBuckets-1;
        addToArray(&buckets[bucketNo], (*unsorted).array[i]);
    }

    for (int j = 0; j < nBuckets; j++)
    {
        SarrayRadixSort(&buckets[j]);
    };

    int index = 0;
    for (int n = 0; n < nBuckets; n++)
    {
        for (int m = 0; m < (&buckets[n])->tam; m++)
        {
            unsorted->array[index] = (&buckets[n])->array[m];
            index++;
        }
    }

    for (int j = 0; j < nBuckets; j++)
    {
        freeSmartArray(&buckets[j]);
    };
}



void bucketSortSeq(int nBuckets, struct SmartArray *unsorted)
{
    struct SmartArray buckets[nBuckets];
    int maxValue = getMax(unsorted);
    int interval = maxValue / nBuckets;
    int bucketNo;

    for (int i = 0; i < nBuckets; i++)
        buckets[i] = *initSmartArray(1);

    for (int i = 0; i < unsorted->tam; i++)
    {
        bucketNo = (unsorted->array[i]) / interval;
        if (bucketNo == nBuckets)
            bucketNo = nBuckets-1;
        addToArray(&buckets[bucketNo], (*unsorted).array[i]);
    }

    for (int j = 0; j < nBuckets; j++)
    {
        SarrayRadixSort(&buckets[j]);
    };

    int index = 0;
    for (int n = 0; n < nBuckets; n++)
    {
        for (int m = 0; m < (&buckets[n])->tam; m++)
        {
            unsorted->array[index] = (&buckets[n])->array[m];
            index++;
        }
    }

    for (int j = 0; j < nBuckets; j++)
    {
        freeSmartArray(&buckets[j]);
    };
}

struct SmartArray genRandomArray(int size, int range)
{
    srand(2);
    struct SmartArray array = *initSmartArray(size);
    for (int i = 0; i < size; i++)
        addToArray(&array, rand() % (range + 1));
    return array;
}

int main()
{
    printf("Hello World\n");
    struct SmartArray t = genRandomArray(1000, 1000);
    printArray(&t);
    isSorted(&t);
    bucketSort(5, &t);
    isSorted(&t);
    printArray(&t);
    freeSmartArray(&t);
}