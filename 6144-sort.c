#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>
  
int array[1000];
int arr_size;

void merge(int l, int m, int h)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = h - m;
  
    int L[n1], R[n2];
  
    for (i = 0; i < n1; i++)
        L[i] = array[l + i];
    for (j = 0; j < n2; j++)
        R[j] = array[m + 1 + j];
        
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            i++;
        }
        else {
            array[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
    }
}
  
void *mergeSort(void *arg)
{

    int *arr = (int *)arg;   
    int l = arr[0];   //retriving the low index
    int h = arr[1];   //retriving the high index
    
    if (l < h) {
        int m = l + (h - l) / 2;
        
        pthread_t thread1,thread2;
        int *arr1 = malloc(2*sizeof(int));
        //storing low and high of the first half in an array to pass it to recursive mergeSort
        arr1[0] = l;
        arr1[1] = m;
        int *arr2 = malloc(2*sizeof(int));
        //storing low and high of the second half in an array to pass it to recursive mergeSort
        arr2[0] = m+1;
        arr2[1] = h;
        
        pthread_create(&thread1, NULL,mergeSort,arr1);
        pthread_create(&thread2, NULL,mergeSort,arr2);
        pthread_join(thread1,NULL);
        pthread_join(thread2,NULL);
        merge(l, m, h);
    }
}

void writeOutput(){

  FILE *outputFile;
  outputFile = fopen("sort-output.txt", "w");
  if (outputFile == NULL){
        printf("Output File Not Found\n");
        exit(EXIT_FAILURE);
        }
  else {
    
    fprintf(outputFile,"sorted array: ");
    for(int i=0;i<arr_size;i++){
      fprintf(outputFile,"%d ",array[i]);
    }
    fclose(outputFile);

}          
}
  

int main(int argc, char *argv[])
{
    FILE *file;
    file = fopen(argv[1], "r");
    if (file == NULL){
        printf("File Not Found\n");
        exit(EXIT_FAILURE);
        }
    else {

    fscanf(file, "%d", &arr_size);
    for(int i=0;i<arr_size;i++){
    
      fscanf(file,"%d",&array[i]);
 
    }
    fclose(file);
}      
    pthread_t thread;
    int *lowAndHigh = malloc(2*sizeof(int));
    lowAndHigh[0] = 0;
    lowAndHigh[1] = arr_size-1;
    pthread_create(&thread, NULL,&mergeSort,lowAndHigh);
    pthread_join(thread,NULL);
    writeOutput();
    
    
    return 0;
}
