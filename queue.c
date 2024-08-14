#include "queue.h"

void initQueue(Queue *q)
{
    q->front = 0;
    q->rear = 0;
    q->size = 0;
}

void clearQueue(Queue *q)
{
    initQueue(q);
}

int isQueueEmpty(Queue *q)
{
    return q->size == 0;
}

int isQueueFull(Queue *q)
{
    return q->size == QUEUE_SIZE;
}

void enqueue(Queue *q, char item)
{
    if (isQueueFull(q))
        return;

    q->data[q->rear] = item;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->size++;
}

char dequeue(Queue *q)
{
    if (isQueueEmpty(q))
        return '\0';

    char item = q->data[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->size--;
    return item;
}
