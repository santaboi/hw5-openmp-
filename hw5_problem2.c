#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "queue_lk.h"
#define MAX_CHAR 25
#define FILE_NUM 6

/************************************************************/
/*  please enter "./h5_problem2 #1 #2" to compile           */
/*  producer_num : #1                                       */
/*  consumer_num : #2                                       */
/*  thread_count = #1 + #2                                  */
/************************************************************/

void enqueue(void);
void dequeue(void);
void read_files(char *);
void produce_consume(char * );


int main(int argc, char *argv[])
{
    int producer_num = 0 , consumer_num = 0 ;
    if (argc != 3)  Usage(argv[0]);
    producer_num = strtol(argv[1], NULL, 10);
    consumer_num = strtol(argv[2], NULL, 10);

    //fork threads
    char path[] = "./text/txt#.txt";
    int index;
    for (index = 1; index < 7; index++){
        path[10] = index + '0';
        //puts(path);
        read_files(path);
    }
    
    
    //read_files();
}

void read_files(char * path) //
{
    FILE *filePointer;
    char *buffer = malloc(MAX_CHAR * sizeof(char));

    filePointer = fopen(path, "r");

    while (fgets(buffer, MAX_CHAR , filePointer))
    {
        //printf("%s\n", buffer);
        /*enqueue each line from buffer*/
        /*split to tokens per line*/
        char *token = strtok(buffer, " ");
        printf(" %s\n", token);
    }

    fclose(filePointer);
}