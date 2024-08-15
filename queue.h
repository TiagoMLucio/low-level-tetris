#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_SIZE 20

typedef struct
{
    char data[QUEUE_SIZE];
    int front;
    int rear;
    int size;
} Queue;

// Initialize the queue
void initQueue(Queue *q);

void clearQueue(Queue *q);

// Check if the queue is empty
int isQueueEmpty(Queue *q);

// Check if the queue is full
int isQueueFull(Queue *q);

// Enqueue an element
void enqueue(Queue *q, char item);

// Dequeue an element
char dequeue(Queue *q);

#endif // QUEUE_H
