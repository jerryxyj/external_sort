#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

int NUM_FILE_PARTITIONS  = 1;
long PARTITION_SIZE = 10;
int NUM_THREADS=2;
int PROG_TYPE=1;
int FILE_INDEX=0;
//char tem[100];
//int OUT_FILE_NUM=0;

void swap(char **a, char **b)
{
    char *t = *a;
    *a = *b;
    *b = t;
}

void clearFiles() {
    int i;
    for (i=0;i<NUM_FILE_PARTITIONS;i++) {
        char *fpout=(char*) malloc(sizeof(char) * 20);
        sprintf(fpout, "/tmp/out%d.txt", i);
        if(access(fpout,F_OK ) != -1 ) {
            remove(fpout);
        }
    }
}




char *minArray(char *arr[]) {
    char *tem = (char*) malloc(sizeof(char) *100);
    //char *tem;
    strcpy(tem, arr[0]);
    FILE_INDEX=0;
    for(int i=0;i<NUM_FILE_PARTITIONS;i++){
        if(strcmp(arr[i],tem)<0){
            strcpy(tem, arr[i]);
            FILE_INDEX=i;
        }
    }
    free(tem);
    return arr[FILE_INDEX];
}


void mergeInputFiles() {
    printf("start merging file!\n");
    char *output = (char*) malloc(sizeof(char) * 20);
    output="/tmp/data-20GB.in";
    FILE *output_file=fopen(output, "w");
    printf("write file created!\n");
    int DONE_FILE=0;
    char *fpout=(char*) malloc(sizeof(char) * 20);
    char **array=(char**) malloc(sizeof(char*) * 100*NUM_FILE_PARTITIONS);
    for(int i=0;i<NUM_FILE_PARTITIONS;i++){
        array[i] = (char*) malloc(sizeof(char) *100);
    }
    printf("memory allocated finished!\n");
    FILE* in[NUM_FILE_PARTITIONS];
    printf("open sorted split file!\n");
    for(int i=0;i<NUM_FILE_PARTITIONS;i++){
        sprintf(fpout, "/tmp/out%d.txt", i);
        in[i] = fopen(fpout, "r");
    }
    printf("finished open sorted file!\n");
    for(int i=0;i<NUM_FILE_PARTITIONS;i++){
        fgets(array[i],101,in[i])==NULL;
    }
    printf("finished read first line in each file!\n");
    char *tem_arr = (char*) malloc(sizeof(char) *100);
    while(DONE_FILE<NUM_FILE_PARTITIONS){
        //tem_arr=minArray(array);
        strcpy(tem_arr, minArray(array));
        if(fgets(array[FILE_INDEX],101,in[FILE_INDEX])==NULL) {
            DONE_FILE++;
            array[FILE_INDEX]="~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
            printf("out%d.txt finished merged",FILE_INDEX);
        }
        fprintf(output_file,"%s",tem_arr);
        //printf("%s\n",tem_arr);
    }
    printf("finished all the data input!\n");
    for(int i=0;i<NUM_FILE_PARTITIONS;i++){
        fclose(in[i]);
    }
    free(tem_arr);
    free(array);
    fclose(output_file);

}
/******************************
quick sort code is using source code from: https://www.geeksforgeeks.org/quick-sort/

******************************/


int partition (char *arr[], int low, int high)
{   char* pivot=(char*) malloc(10);// pivot
    strncpy(pivot,arr[high],10);
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (strcmp(arr[j],pivot)<=0)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(char *arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


void *startSortArrays(void *arg) {
    long NUM_FILE = (long)arg;
    //printf("enter thread%ld\n",NUM_FILE);
    char *file = (char*) malloc(sizeof(char) * 20);
    sprintf(file, "/tmp/out%ld.txt", NUM_FILE);
    NUM_FILE++;
    FILE *fp=fopen(file, "r");
    char **input_array = (char**)malloc(PARTITION_SIZE * 100*sizeof(char));
    for(long k = 0; k < PARTITION_SIZE; k++){
        input_array[k] = (char*) malloc(100*sizeof(char));
    }
    for(long j=0;j<PARTITION_SIZE;j++) {
        fgets(input_array[j],101,fp);
    }
    fclose(fp);
    //pthread_mutex_lock(&lock);
    //struct arg_struct *args = (struct arg_struct *)arg;
    quickSort(input_array,0,PARTITION_SIZE-1);
    //pthread_mutex_unlock(&lock);
    FILE *fpout = fopen(file, "w");
    for(long i=0;i<PARTITION_SIZE;i++){
        fprintf(fpout,"%s",input_array[i]);
    }
    printf("already sorted out%ld.txt\n", NUM_FILE-1);
    fclose(fpout);
    for(int i=0; i<PARTITION_SIZE;i++)
    {
        free(input_array[i]);
    }
    free(input_array);
    pthread_exit((void*) arg);
}
void sortArrays() {
    int rc;
    long NUM_FILE=0;
    int m=0;
    for (m=0;m<NUM_FILE_PARTITIONS/NUM_THREADS;m++) {
        pthread_t thread_id[NUM_THREADS];
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        for (int i=0;i<NUM_THREADS;i++) {
            rc=pthread_create(&(thread_id[i]),&attr,startSortArrays,(void *)NUM_FILE);
            NUM_FILE++;
            if (rc) {
                printf("ERROR; return code from pthread_create() is %d\n", rc);
                exit(-1);
            }
        }
        pthread_attr_destroy(&attr);
        int n;
        for (n=0;n<NUM_THREADS;n++) {
          pthread_join(thread_id[n], NULL);
        }
        //pthread_exit(NULL);
    }

}
void outputFile() {
    char *file = (char*) malloc(sizeof(char) * 20);

    file="/input/data-2GB.in";
    FILE *fp=fopen(file, "r");
    int i;
    char **input_array = (char**)malloc(PARTITION_SIZE * 100);
    for(i = 0; i < PARTITION_SIZE; i++) {
        input_array[i] = (char*) malloc(100);
    }
    int j;
    for(j=0;j<PARTITION_SIZE;j++) {
        if(fgets(input_array[j],101,fp)==NULL) {
            break;
        }
        //printf("test\n");
    }
    quickSort(input_array,0,j-1);
    char *outfile=(char*) malloc(sizeof(char) * 20);
    sprintf(outfile, "/tmp/data-2GB.in");
    FILE *fpout = fopen(outfile, "w");
    long k;
    for(k=0;k<PARTITION_SIZE;k++){
        fprintf(fpout,"%s",input_array[k]);
    }
    fclose(fp);
}

void splitInputFile() {
    char *file = (char*) malloc(sizeof(char) * 20);

    file="/input/data-20GB.in";

    FILE *fp=fopen(file, "r");
    char **input_array = (char**)malloc(PARTITION_SIZE * 101*sizeof(char*));
    for(long i=0;i<PARTITION_SIZE;i++){
        input_array[i] = (char*) malloc(101*sizeof(char));
    }
    for (int i=0;i<NUM_FILE_PARTITIONS;i++) {
        char *file_out=(char*) malloc(sizeof(char) * 20);
        sprintf(file_out, "/tmp/out%d.txt", i);
        FILE *fpout = fopen(file_out, "w");

        for(long j=0;j<PARTITION_SIZE;j++) {
            //fgets(input_array[j],100,fp);
            if(fgets(input_array[j],101,fp)==NULL) {
                break;
            }
            fprintf(fpout,"%s",input_array[j]);
        }
/*
        for(long j=0;j<PARTITION_SIZE;j++){
            fprintf(fpout,"%s",input_array[j]);
        }
*/
        fclose(fpout);
    }
    free(input_array);
    fclose(fp);
}
void sortArrays_nothread(char *array[],int array_size,int OUT_FILE_NUM) {
    quickSort(array,0,array_size-1);
    char *file=(char*) malloc(sizeof(char) * 20);
    sprintf(file, "/tmp/out%d.txt", OUT_FILE_NUM);
    FILE *fpout = fopen(file, "w");
    int i;
    for(i=0;i<PARTITION_SIZE;i++){
        fprintf(fpout,"%s",array[i]);
    }
    printf("already sorted out%d.txt\n", OUT_FILE_NUM);
    OUT_FILE_NUM++;
    fclose(fpout);
}

void splitInputFile_nothread() {
    char *file = (char*) malloc(sizeof(char) * 20);
    if(PROG_TYPE == 1) {
        file="/input/data-2GB.in";
    } else {
        file="/input/data-20GB.in";
    }
    FILE *fp=fopen(file, "r");
    int i;
    char **input_array = (char**)malloc(PARTITION_SIZE * 100);
    for(i = 0; i < PARTITION_SIZE; i++) {
        input_array[i] = (char*) malloc(100);
    }
    for (i=0;i<NUM_FILE_PARTITIONS;i++) {
        int j;
        for(j=0;j<PARTITION_SIZE;j++) {
            if(fgets(input_array[j],101,fp)==NULL) {
                break;
            }
            //printf("test\n");
        }
        sortArrays_nothread(input_array,j,i);

    }
    free(input_array);
    fclose(fp);
}



int main(int argc, char *argv[]) {
    if(strcmp(argv[1],"20G")==0){
        NUM_FILE_PARTITIONS=10;
        PARTITION_SIZE=20000000;
        PROG_TYPE=2;
        NUM_THREADS=2;
    }
    else{
        NUM_FILE_PARTITIONS=1;
        PARTITION_SIZE=20000000;
        PROG_TYPE=1;
        NUM_THREADS=1;
    }
    time_t begin,end;
    begin= time(NULL);
    if(PROG_TYPE==2){
        //splitInputFile_nothread();
        splitInputFile();
        sortArrays();
        printf("finished split input file!\n");
        mergeInputFiles();
    }
    else{
        outputFile();
        //splitInputFile_nothread();
        //mergeInputFiles();
    }
    end = time(NULL);
    FILE * fp;

    fp = fopen ("result.txt","a");
    if (PROG_TYPE == 1) {

        fprintf (fp, "Total time taken to sort 2GB data is %.6lf seconds\n",difftime(end, begin));

    } else if(PROG_TYPE == 2){
        fprintf (fp, "Total time taken to sort 20GB data is %.6lf seconds\n",difftime(end, begin));
    }
    fclose (fp);
    clearFiles();

}
