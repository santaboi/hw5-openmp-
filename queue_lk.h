#ifndef _QUEUE_LK_H_
#define _QUEUE_LK_H_
#endif
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct queue_s queue_s;
typedef struct queue_node_s queue_node_s ;

/*First in First out*/
struct queue_node_s
{
    int src;
    char* mesg;
    struct queue_node_s *next_p;
};

//make this a container (with functions)
struct queue_s 
{
    omp_lock_t lock;
    int enqueued;
    int dequeued;
    queue_node_s *front_p ;
    queue_node_s *tail_p ;
};

queue_s *Allocate_queue(void);
void Free_queue(queue_s *q_p);
void Print_queue(queue_s *q_p);
void Enqueue(queue_s *q_p, int source, char* message);
queue_node_s *Dequeue(queue_s *q_p, int *src_p, char *mesg_p);
int Search(queue_s *q_p, char mesg, int *src_p);


queue_s * Allocate_queue()
{
    struct queue_s *q_p = (queue_s*)malloc(sizeof(queue_s));
    q_p->enqueued = q_p->dequeued = NULL;
    q_p->front_p = q_p->tail_p = NULL;
    omp_init_lock(&q_p->lock); // DIFF
    return q_p;
}

void Free_queue(queue_s *q_p)
{
    queue_node_s *curr_p, *temp_p;
    for (curr_p = q_p->front_p; curr_p != NULL; temp_p = curr_p, curr_p = curr_p->next_p, free(temp_p))
        ;
    q_p->enqueued = q_p->dequeued = NULL;
    q_p->front_p = q_p->tail_p = NULL;
}

void Print_queue(queue_s *q_p)
{
    queue_node_s *curr_p;
    printf("queue = \n");
    for (curr_p = q_p->front_p; curr_p != NULL; curr_p = curr_p->next_p)
        printf("   src = %d, mesg = %d\n", curr_p->src, curr_p->mesg);
    printf("enqueued = %d, dequeued = %d\n", q_p->enqueued, q_p->dequeued);
    printf("\n");
}

//create data node and add to the linkedlist end
//enque to queue
void Enqueue(queue_s *q_p, int source, char *message)
{
    queue_node_s *n_p = (queue_node_s *)malloc(sizeof(queue_node_s));
    n_p->src = source;
    n_p->mesg = message;
    n_p->next_p = NULL;

    #pragma omp critical
    {
        if (q_p->tail_p == NULL) // list is empty
        {
            q_p->front_p = n_p;
            q_p->tail_p = n_p;
        }
        else 
        {
            q_p->tail_p->next_p = n_p;
            q_p->tail_p = n_p;
        }
        q_p->enqueued++;
    }
}

// remove a node from queue linkedlist end ,  and tokenize them
queue_node_s* Dequeue(queue_s *q_p, int *src_p, char *mesg_p)
{
    queue_node_s *temp_p;
    if (q_p->front_p == NULL)
        return;
    *src_p = q_p->front_p->src;
    *mesg_p = q_p->front_p->mesg;
    temp_p = q_p->front_p;
    #pragma omp critical
    {
        if (q_p->front_p == q_p->tail_p)
            q_p->front_p = q_p->tail_p = NULL;
        else
            q_p->front_p = temp_p->next_p;
        q_p->dequeued++;
    }
    temp_p = q_p->front_p;
    return temp_p;
}

int Search(queue_s *q_p, int mesg, int *src_p)
{
    queue_node_s *curr_p;
    for (curr_p = q_p->front_p; curr_p != NULL; curr_p = curr_p->next_p)
    {
        if (curr_p->mesg == mesg)
        {
            *src_p = curr_p->src;
            return;
        }
    }
    return;
}
