#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "queue_lk.h"

#define MAX_CHAR 25
#define FILE_NUM 6
#define NUM_KEYS 6

void read_files(char *path, queue_s *shared_queue)
{
    FILE *filePointer;
    char *buffer = malloc(MAX_CHAR * sizeof(char));

    filePointer = fopen(path, "r");

    while (fgets(buffer, MAX_CHAR, filePointer))
    {
        Enqueue(shared_queue, NULL, buffer); // NULL
        buffer = malloc(MAX_CHAR * sizeof(char));
    }

    fclose(filePointer);
}

void Tokenize(char *mesg, int keycount_array[])
{
    // split to tokens per line
    char *token = strtok(mesg, " ");
    while (token != NULL)
    {
        //"eyes" , "and" , "you" ,"me" ,"yeah" ,"all"
        if (strcmp(token, "eyes") == 0)
            keycount_array[0]++;
        else if (strcmp(token, "and") == 0)
            keycount_array[1]++;
        else if (strcmp(token, "you") == 0)
            keycount_array[2]++;
        else if (strcmp(token, "me") == 0)
            keycount_array[3]++;
        else if (strcmp(token, "yeah") == 0)
            keycount_array[4]++;
        else if (strcmp(token, "all") == 0)
            keycount_array[5]++;
    }
}

int main(int argc, char *argv[])
{
    int producer_num = 0, consumer_num = 0;

    producer_num = strtol(argv[1], NULL, 10);
    consumer_num = strtol(argv[2], NULL, 10);
    int thread_count = producer_num + consumer_num;

    int producer_exe_count = 0;
    int file_nums = FILE_NUM;
    char path[] = "./text/txt#.txt";
    queue_s *shared_queue = Allocate_queue();
    int i = 0;
    #pragma omp parallel num_threads(thread_count) default(none) private(i) shared(file_nums, producer_num, consumer_num, path, producer_exe_count, shared_queue)
    {
        int my_rank = omp_get_thread_num();
        printf("%d",my_rank);
        if (my_rank < producer_num)
        {
            for (i = 1; i < file_nums + 1; i++)
            {
                path[10] = i + '0';
                read_files(path, shared_queue);
            }

            #pragma omp atomic
            producer_exe_count++;
        }
        
        else
        {
            queue_node_s *temp_node = NULL;
            while (producer_exe_count < producer_num)
            {
                temp_node = Dequeue(shared_queue, NULL, NULL);
                if (temp_node != NULL)
                {
                    Tokenize(temp_node->mesg, my_rank);
                    free(temp_node);
                }
            }
            while (shared_queue->front_p != NULL)
            {
                temp_node = Dequeue(shared_queue, NULL, NULL);
                if (temp_node != NULL)
                {
                    //Tokenize(temp_node->mesg, my_rank);
                    free(temp_node);
                }
            }
        }
        
    }
    printf("ssuccess! \n");
}