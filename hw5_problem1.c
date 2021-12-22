#include<stdio.h>
#include<omp.h>
#include<time.h>
void OMP_Count_sort(int a[], int n , int thread_count)
{
    int i, j, count;
    int *temp = malloc(n * sizeof(int));
    //parallel block (thread_count of threads are forked)
    #pragma omp parallel num_threads(thread_count)
    {
        #pragma omp for private(count, j) 
        for (i = 0; i < n; i++)
        {
            count = 0;
            for (j = 0; j < n; j++)
                if (a[j] < a[i])
                    count++;
                else if (a[j] == a[i] && j < i)
                    count++;
            temp[count] = a[i];
        }
    }
    memcpy(a, temp, n * sizeof(int)); //this line is place below the parallel block
    free(temp);
}
void simple_count_sort(int a[], int n){
    int i, j, count;
    int *temp = malloc(n * sizeof(int));
        for (i = 0; i < n; i++)
        {
            count = 0;
            for (j = 0; j < n; j++)
                if (a[j] < a[i])
                    count++;
                else if (a[j] == a[i] && j < i)
                    count++;
            temp[count] = a[i];
        }
    memcpy(a, temp, n * sizeof(int)); // this line is place below the parallel block
    free(temp);
}

int compare(const void *arg1, const void *arg2)
{
    int ret = *(int *)(arg1) - *(int *)(arg2); //cast to int, and then dereference it
    if (ret > 0)
        return 1;
    if (ret < 0)
        return -1;
    return 0;
}

int main(int argc, char **argv)
{
    int thread_count = strtol(argv[1] , NULL , 10); //get thread numbers
    int start_time , end_time ; 
    int n = 100;
    int i ; //for index 
    int * a = malloc(n*sizeof(int)); //array to sort
    srand(time(0));
    for(i = 0 ; i < 100 ; i++){
        a[i] = rand() % (n*100) ;
    }

    start_time = time(NULL);
    OMP_Count_sort(a, n, thread_count);
    end_time = time(NULL);
    printf("(sorted by open_mp) total time : %d"  , start_time - end_time);

    start_time = time(NULL);
    simple_count_sort(a, n, thread_count);
    end_time = time(NULL);
    printf("(sorted by open_mp) total time : %d" ,  start_time - end_time);

    start_time = time(NULL);
    simple_count_sort(a, n, thread_count);
    end_time = time(NULL);
    printf("(sorted by open_mp) total time : %d", start_time - end_time);

    /* //uncomment to see the outcome
    for (i = 0; i < n; i++)
    {
        printf("%d  ", a[i]);
    }
    printf("\n");
    */
}
