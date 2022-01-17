#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "SmartArray.h"

#include "papi.h"
void bucketSortPar(int nBuckets, struct SmartArray *unsorted,int sortFlag)
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
        //printf("Thread %d:Bucket %d\n", id, j);
        switch (sortFlag){
            case 0:
                SarrayInsertionSort(&buckets[j]);
                break;
            case 1:
                SarrayQuickSort(&buckets[j],0,(&buckets[j])->tam);
                break;
            default:
                SarrayRadixSort(&buckets[j]);
                break;
        }
        
        
        
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




void bucketSortSeq(int nBuckets, struct SmartArray *unsorted,int sortFlag)
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
        switch (sortFlag){
            case 0:
                SarrayInsertionSort(&buckets[j]);
                break;
            case 1:
                SarrayQuickSort(&buckets[j],0,(&buckets[j])->tam);
                break;
            default:
                SarrayRadixSort(&buckets[j]);
                break;
        }
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

void testSortingAlg(int runs){
    struct SmartArray array;
    struct SmartArray unordered;
    int elems=1000;
    int buckets = 2;
    int algo=0;
    int p;
    double time;
    const char *algos[3];
    algos[0] = "InsertionSort";
    algos[1] = "QuickSort";
    algos[2] = "RadixSort";

    for(elems=1000;elems <= elems * 10000;elems=elems*10){
        array = genRandomArray(elems,8192);
        for(buckets=2;buckets <= 64;buckets=buckets*2)
        for(algo=1;algo<=2;algo++){
            time=0;
            for(p=0;p<runs;p++){
                unordered = *dupSmartArray(&array);
                double start = omp_get_wtime();
                bucketSortSeq(buckets,&unordered,algo);
                double end = omp_get_wtime();
                //isSorted(&unordered);
                time+=end-start;
                freeSmartArray(&unordered);
        }
            printf("Array size: %d | Nºbuckets: %d | Algorithm: %s | time: %lf ms\n",elems,buckets,algos[algo],(time*1000)/runs);
    }
    freeSmartArray(&array);
}
}

#define NUM_EVENTS 4
int Events[NUM_EVENTS] = { PAPI_TOT_CYC, PAPI_TOT_INS, PAPI_L1_DCM, PAPI_L2_DCM};

// PAPI counters' values
long long values[NUM_EVENTS], min_values[NUM_EVENTS];


int main(int argc, char *argv[])
{
    // Valores por defeito
    int nExecs = 1;
    int arraySize = 1000000;
    int nBuckets = 16;
    double parTime=0,seqTime=0;
	int num_hwcntrs = 0;
	double start,end;

    if (argc > 1) // Number of executions
        nExecs = atoi(argv[1]);
    if (argc > 2) // Array Size
        arraySize = atoi(argv[2]);
    if (argc > 3) // nBuckets+Threads
        nBuckets = atoi(argv[3]);

	//PAPI Setup

	fprintf (stdout, "\nSetting up PAPI...");
	// Initialize PAPI 
	PAPI_library_init (PAPI_VER_CURRENT);

	  /* Get the number of hardware counters available */
  	if ((num_hwcntrs = PAPI_num_counters()) <= PAPI_OK)  {
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








    printf("Hello World\n");
    struct SmartArray t = genRandomArray(arraySize, arraySize);
    struct SmartArray unordered;
    int i;
    for(i=0;i<nExecs;i++){
        unordered=*dupSmartArray(&t);
		printf("\nParallel!\n");
        //isSorted(&unordered);
        start = omp_get_wtime();

   		/* Start counting events */
   		if (PAPI_start_counters(Events, num_hwcntrs) != PAPI_OK) {
     		fprintf (stderr, "PAPI error starting counters!\n");
     		return 0;
  		}		

        bucketSortPar(nBuckets, &unordered,3);
        end = omp_get_wtime();

		/* Stop counting events */
		if (PAPI_stop_counters(values, NUM_EVENTS) != PAPI_OK) {
			fprintf (stderr, "PAPI error stoping counters!\n");
			return 0;
		}


		// output PAPI counters' values
		for (i=0 ; i< NUM_EVENTS ; i++) {
			char EventCodeStr[PAPI_MAX_STR_LEN];

			if (PAPI_event_code_to_name(Events[i], EventCodeStr) == PAPI_OK) {
				fprintf (stdout, "%s = %lld\n", EventCodeStr, values[i]);
			} else {
				fprintf (stdout, "PAPI UNKNOWN EVENT = %lld\n", values[i]);
			}
		}

		if ((Events[0] == PAPI_TOT_CYC) && (Events[1] == PAPI_TOT_INS)) {
			float CPI = ((float) values[0]) / ((float) values[1]);
			fprintf (stdout, "CPI = %.2f\n", CPI);
		}



        //isSorted(&unordered);
        parTime+=end-start;
        freeSmartArray(&unordered);
    }
	printf("\nSequential\n");
    for(i=0;i<nExecs;i++){
        unordered=*dupSmartArray(&t);
        //isSorted(&unordered);
        start = omp_get_wtime();
           		/* Start counting events */
   		if (PAPI_start_counters(Events, num_hwcntrs) != PAPI_OK) {
     		fprintf (stderr, "PAPI error starting counters!\n");
     		return 0;
  		}		

        bucketSortSeq(nBuckets, &unordered,3);
        end = omp_get_wtime();

		/* Stop counting events */
		if (PAPI_stop_counters(values, NUM_EVENTS) != PAPI_OK) {
			fprintf (stderr, "PAPI error stoping counters!\n");
			return 0;
		}

		if ((Events[0] == PAPI_TOT_CYC) && (Events[1] == PAPI_TOT_INS)) {
			float CPI = ((float) values[0]) / ((float) values[1]);
			fprintf (stdout, "CPI = %.2f\n", CPI);
		}


        double end = omp_get_wtime();
        //isSorted(&unordered);
        seqTime+=end-start;
        freeSmartArray(&unordered);
    }

		// output PAPI counters' values
		for (i=0 ; i< NUM_EVENTS ; i++) {
			char EventCodeStr[PAPI_MAX_STR_LEN];

			if (PAPI_event_code_to_name(Events[i], EventCodeStr) == PAPI_OK) {
				fprintf (stdout, "%s = %lld\n", EventCodeStr, values[i]);
			} else {
				fprintf (stdout, "PAPI UNKNOWN EVENT = %lld\n", values[i]);
			}
		}



    printf("Average Time to Sequential : %f ms\n",(seqTime*1000)/nExecs);
    printf("Average Time to parallel : %f ms\n",(parTime*1000)/nExecs);
    //printArray(&t);

    //printArray(&t);
    freeSmartArray(&t);
}