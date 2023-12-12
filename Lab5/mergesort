#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 100

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) array[i] = rand()%100;
}

void printArray(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

void merge(int l, int middle, int r) {
    int left_size = middle - l + 1; //Size of left subarray
    int right_size = r - middle; //Size of right subarray

    //Temporary subarrays
    int change_left[left_size];
    int change_right[right_size]; 

    for (int i = 0; i < left_size; i++){
      change_left[i] = array[i + l];
    }

    for (int x = 0; x < right_size; x++){
      change_right[x] = array[x + middle + 1];
    }
    int i = 0;
    int j = 0;
    int k = l;

    while (i < left_size && j < right_size) {
        if (change_left[i] <= change_right[j]) {
            array[k] = change_left[i];
            i++;
        } else {
            array[k] = change_right[j];
            j++;
        }
        k++;
    } 

    while (i < left_size) {
        array[k] = change_left[i];
        i++;
        k++;
    }

    while (j < right_size) {
        array[k] = change_right[j];
        j++;
        k++;
    }
}

// Runs mergesort on the array segment described in the
// argument. Spawns two threads to mergesort each half
// of the array segment, and then merges the results.
void* mergeSort(void* args) {
    StartEndIndexes* sei = (StartEndIndexes*)args;
    int start = sei->start;
    int end = sei->end;

    if (start < end) {
      int mid = start + (end - start) / 2;

      StartEndIndexes sei1 = {start, mid};
      StartEndIndexes sei2 = {mid + 1, end};

      pthread_t thread1;
      pthread_t thread2;

      int number_of_threads = (end - start + 1) <= SIZE / 3 ? (end - start + 1) : SIZE / 3;

      pthread_create(&thread1, NULL, mergeSort, &sei1);
      pthread_create(&thread2, NULL, mergeSort, &sei2);

      pthread_join(thread1, NULL);
      pthread_join(thread2, NULL);

      merge(start, mid, end);
    }
    return NULL;
}


int main() {
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;
    
    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);
    
    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);
    
    // 3. Create a 2 threads for merge sort.
    pthread_t merge_threads;
    pthread_create(&merge_threads, NULL, mergeSort, &sei);
    
    // 4. Wait for mergesort to finish.
    pthread_join(merge_threads, NULL);
    
    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);

    return 0;
}
