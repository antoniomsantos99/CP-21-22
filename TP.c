#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "SmartArray.h"

#include "papi.h"

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
    
    #pragma omp parallel  //num_threads(nBuckets)
    {
    #pragma omp for schedule(dynamic)
    for (j = 0; j < nBuckets; j++)
    {
        int id = omp_get_thread_num();
        //printf("T%d:i%d\n", id, j);
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




// PAPI events to monitor
#define NUM_EVENTS 5
int Events[NUM_EVENTS] = { PAPI_TOT_CYC, PAPI_TOT_INS, PAPI_L1_DCM, PAPI_L2_DCM, PAPI_L3_DCM };

// PAPI counters values
long long values[NUM_EVENTS], min_values[NUM_EVENTS];
int retval, EventSet = PAPI_NULL;

int main(int argc, char *argv[])
{
    // PAPI variables
    long long start_usec, end_usec, elapsed_usec_par, elapsed_usec_seq, min_usec=0L;
    int num_hwcntrs = 0;

    fprintf(stdout, "Setting up PAPI...");

    // Initialize PAPI
    retval = PAPI_library_init (PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        fprintf(stderr,"PAPI library init error!\n");
        return 0;
    }
    // create event set
    if (PAPI_create_eventset(&EventSet) != PAPI_OK) {
        fprintf(stderr,"PAPI create event set error\n");
        return 0;
    }

    // Get the number of hardware counters available
    if ((num_hwcntrs = PAPI_num_hwctrs()) <= PAPI_OK)  {
        fprintf (stderr, "PAPI error getting number of available hardware counters!\n");
        return 0;
    }
    fprintf(stdout, "done!\nThis system has %d available counters.\n\n", num_hwcntrs);

    // We will be using at most NUM_EVENTS counters
    if (num_hwcntrs >= NUM_EVENTS) {
        num_hwcntrs = NUM_EVENTS;
    } else {
        fprintf (stderr, "Error: there aren't enough counters to monitor %d events!\n", NUM_EVENTS);
        return 0;
    }
    
    /* Bug
    if (PAPI_add_events(EventSet,Events,NUM_EVENTS) != PAPI_OK)  {
        fprintf(stderr,"PAPI library add events error!\n");
        return 0;
    }
    */



    // Valores por defeito
    int nExecs = 10;
    int arraySize = 1000000;
    int nBuckets = 16;
    double parTime=0,seqTime=0;

    if (argc > 1) // Number of executions
        nExecs = atoi(argv[1]);
    if (argc > 2) // Array Size
        arraySize = atoi(argv[2]);
    if (argc > 3) // nBuckets+Threads
        nBuckets = atoi(argv[3]);

    // fprintf(stdout, "Hello World\n");
    struct SmartArray t = genRandomArray(arraySize, arraySize);
    struct SmartArray unordered;
    int i, j;

    for(i=0;i<nExecs;i++){
        unordered=*dupSmartArray(&t);

        start_usec = PAPI_get_real_usec();
        
        /* Start counting events : Bug 
        if (PAPI_start(EventSet) != PAPI_OK) {
            fprintf (stderr, "PAPI error starting counters!\n");
            return 0;
        }
        */

        // isSorted(&unordered);
        double start = omp_get_wtime();
        bucketSortPar(nBuckets, &unordered);
        double end = omp_get_wtime();
        // isSorted(&unordered);
        parTime+=end-start;

        /* Bug
        if (PAPI_stop(EventSet,values) != PAPI_OK) {
            fprintf (stderr, "PAPI error stoping counters!\n");
            return 0;
        }
        */

        end_usec = PAPI_get_real_usec();
        
        fprintf (stdout, "Parallel execution #%d done\n", i);
        
        elapsed_usec_par = end_usec - start_usec;

        /*
        if ((nExecs==0) || (elapsed_usec < min_usec)) {
            min_usec = elapsed_usec;
            for (j=0 ; j< NUM_EVENTS ; i++) min_values[j] = values [j];
        }
        */
        fprintf (stdout,"Wall clock time for run %d: %lld usecs\n", i, elapsed_usec_par);
        

        freeSmartArray(&unordered);
    }

    fprintf(stdout, "\ndebug par-seq\n");

    for(i=0;i<nExecs;i++){
        unordered=*dupSmartArray(&t);

        start_usec = PAPI_get_real_usec();

        //isSorted(&unordered);
        double start = omp_get_wtime();
        bucketSortSeq(nBuckets, &unordered);
        double end = omp_get_wtime();
        //isSorted(&unordered);
        seqTime+=end-start;

        end_usec = PAPI_get_real_usec();
        
        fprintf (stdout, "Sequential execution #%d done\n", i);
        
        elapsed_usec_seq = end_usec - start_usec;
        fprintf (stdout,"Wall clock time for run %d: %lld usecs\n", i, elapsed_usec_seq);

        freeSmartArray(&unordered);
    }

    fprintf(stdout, "\ndebug post-seq\n\n");


    fprintf(stdout, "Average Time to Sequential : %f ms\n",(seqTime*1000)/nExecs);
    fprintf(stdout, "Average Time to parallel : %f ms\n",(parTime*1000)/nExecs);
    //printArray(&t);
    //printArray(&t);



    // output PAPI counters' values
    /*
    for (i=0 ; i< NUM_EVENTS ; i++) {
        char EventCodeStr[PAPI_MAX_STR_LEN];
        
        if (PAPI_event_code_to_name(Events[i], EventCodeStr) == PAPI_OK) {
            fprintf (stdout, "%s = %lld\n", EventCodeStr, min_values[i]);
        } else {
            fprintf (stdout, "PAPI UNKNOWN EVENT = %lld\n", min_values[i]);
        }
    }
    */


    freeSmartArray(&t);
}
