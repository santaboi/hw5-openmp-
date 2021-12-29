#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "queue_lk.h"


#define MAX_CHAR 25
#define FILE_NUM 6
#define NUM_KEYS 6


/************************************************************/
/*  please enter "./h5_problem2 #1 #2" to compile           */
/*  producer_num : #1                                       */
/*  consumer_num : #2                                       */
/*  thread_count = #1 + #2                                  */
/************************************************************/

//void get_keywords(char **golbal_keywords);
void read_files(char *path, queue_s *shared_queue);
void Tokenize(char *mesg, int keycount_array[]);
void produce_consume(int producer_num, int consumer_num, int keycount_array[]);

int main(int argc, char *argv[])
{
    int producer_num = 0 , consumer_num = 0 ;
    
    producer_num = strtol(argv[1], NULL, 10);
    consumer_num = strtol(argv[2], NULL, 10);
    printf("%d , %d" , producer_num , consumer_num);
    
    //get keywords
    char* golbal_keywords[6] = {"eyes" , "and" , "you" ,"me" ,"yeah" ,"all"};
    int keycount_array[6] = {0}; 
    //get_keywords(golbal_keywords);

    produce_consume(producer_num, consumer_num, keycount_array);

    int i = 0 ;
    for (i = 0; i < NUM_KEYS; i++){
        printf("%s : %d", golbal_keywords[i], keycount_array[i]);
    }
    
    




    /*
    //create a shared queue
    queue_s * shared_queue = Allocate_queue();
    //fork threads
    char path[] = "./text/txt#.txt";
    int index;
    for (index = 1; index < 7; index++){
        path[10] = index + '0';
        //puts(path);
        read_files(path, shared_queue);
    }
    */
    
    return 0 ;
}
/*
void get_keywords(char *golbal_keywords)
{
    //get # of keywords
    FILE *filePointer;
    filePointer = fopen("./keywords.txt", "r");
    int key_index = 0 ;
    char arr[NUM_KEYS][10];
    golbal_keywords = arr; 
    char *buffer = malloc(10 * sizeof(char));
    
    for (key_index = 0; key_index < NUM_KEYS; key_index++){
        fgets(buffer, 10, filePointer);
        strcpy(golbal_keywords[key_index], buffer);
        buffer = malloc(10 * sizeof(char));
    }
    fclose(filePointer);
}
*/

void read_files(char *path, queue_s *shared_queue) 
{
    FILE *filePointer;
    char *buffer = malloc(MAX_CHAR * sizeof(char));

    filePointer = fopen(path, "r");

    while (fgets(buffer, MAX_CHAR , filePointer))
    {
        //printf("%s\n", buffer);
        /*enqueue each line from buffer*/
        Enqueue(shared_queue, NULL , buffer); //NULL
        buffer = malloc(MAX_CHAR * sizeof(char));
    }

    fclose(filePointer);
}

void Tokenize(char* mesg , int keycount_array[]){
    // split to tokens per line
    if (mesg == NULL) return ;
    char *token = strtok(mesg, " ");
    printf("%s" , token);
    /*
    while(token != NULL){
        //"eyes" , "and" , "you" ,"me" ,"yeah" ,"all"
        if (strcmp(token, "eyes") == 0) keycount_array[0]++;
        else if (strcmp(token, "and") == 0) keycount_array[1]++;
        else if (strcmp(token, "you") == 0) keycount_array[2]++;
        else if (strcmp(token, "me") == 0) keycount_array[3]++;
        else if (strcmp(token, "yeah") == 0) keycount_array[4]++;
        else if (strcmp(token, "all") == 0) keycount_array[5]++;
    }
    */
}

void produce_consume(int producer_num, int consumer_num, int keycount_array[])
{
    int thread_count = producer_num + consumer_num;
    //queue_node_s* queue_head = NULL ;
    //queue_node_s* queue_tail = NULL ; 
    int producer_exe_count = 0 ;
    int file_nums = FILE_NUM ; 
    char path[] = "./text/txt#.txt";
    queue_s *shared_queue = Allocate_queue();
    int i = 0 ;

#pragma omp parallel num_threads(thread_count) default(none) private(i) shared(file_nums, producer_num, consumer_num, path, producer_exe_count, shared_queue, keycount_array)
    {
        int my_rank = omp_get_thread_num() ;
        if (my_rank < producer_num){
            for (i = 1; i < file_nums + 1; i++)
            {
                path[10] = i + '0';
                read_files(path, shared_queue);
            }
            #pragma omp atomic
            producer_exe_count++;
        }
        else{
            queue_node_s* temp_node ;
            temp_node = Dequeue(shared_queue, NULL, NULL);
            Tokenize(temp_node->mesg, keycount_array);
            /*
            while (producer_exe_count < producer_num)
            {
                temp_node = Dequeue(shared_queue, NULL , NULL);
                if (temp_node != NULL)
                {
                    Tokenize(temp_node->mesg, keycount_array);
                    //free(temp_node);
                }
            }
            
            while (shared_queue->front_p != NULL)
            {
                temp_node = Dequeue(shared_queue , NULL , NULL);
                if (temp_node != NULL)
                {
                    Tokenize(temp_node->mesg, keycount_array);
                    free(temp_node);
                }
            }
            */
            
        }
        
        
    }
}

