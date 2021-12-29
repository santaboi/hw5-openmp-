#ifndef _QUEUE_LK_H_
#define _QUEUE_LK_H_
#endif
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>


/*First in First out*/
struct queue_node_s
{
    char* mesg;
    struct queue_node_s *next_p;
};
typedef struct queue_node_s queue_node_s ;

//make this a container (with functions)
struct queue_s 
{
    omp_lock_t lock;
    int enqueued;
    int dequeued;
    queue_node_s *front_p ;
    queue_node_s *tail_p ;
};
typedef struct queue_s queue_s;

queue_s *Allocate_queue(void);
void Free_queue(queue_s *q_p);
void Enqueue(queue_s *q_p, char* message);
queue_s * Dequeue(queue_s *q_p);

queue_s * Allocate_queue()
{
    struct queue_s *q_p = (queue_s*)malloc(sizeof(queue_s));
    q_p->enqueued = q_p->dequeued = 0;
    q_p->front_p = q_p->tail_p = NULL;
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

//create data node and add to the linkedlist end
//enque to queue
void Enqueue(queue_s *q_p, char *message)
{
    queue_node_s *n_p = (queue_node_s *)malloc(sizeof(queue_node_s));
    n_p->mesg = message;
    n_p->next_p = NULL;

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
    return ;
}

// remove a node from queue linkedlist end ,  and tokenize them
//pass in pointer to front_pointer and tail_pointer
queue_s* Dequeue(queue_s * q_p)
{
    queue_node_s *temp_p = NULL;
    if ((q_p -> front_p) == NULL)
        return NULL;

    if ((q_p->front_p) == (q_p -> tail_p))
        (q_p->front_p) = (q_p->tail_p) = NULL;
    else
        (q_p->front_p) = (q_p->front_p)->next_p;
    return temp_p;
}
