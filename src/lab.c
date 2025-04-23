#include "lab.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct queue {
    void **buffer;
    int capacity;
    int size;
    int front;
    int rear;
    bool shutdown;
};

queue_t queue_init(int capacity) { // Fixed typo
    queue_t q = malloc(sizeof(struct queue));
    if (!q) return NULL;

    q->buffer = malloc(capacity * sizeof(void *));
    if (!q->buffer) {
        free(q);
        return NULL;
    }

    q->capacity = capacity;
    q->size = 0;
    q->front = 0;
    q->rear = 0;

    return q;
}

void queue_destroy(queue_t q) {
    if (!q) return;

    free(q->buffer);
    free(q);
}

void enqueue(queue_t q, void *data) {
    if (q->size == q->capacity) {
        printf("Queue is full!\n");
        return;
    }

    q->buffer[q->rear] = data;
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;
}

void *dequeue(queue_t q) {
    if (q->size == 0) {
        printf("Queue is empty!\n");
        return NULL;
    }

    void *data = q->buffer[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;

    return data;
}

bool is_empty(queue_t q) {
    return q->size == 0;
}

bool is_shutdown(queue_t q) {
    return q->shutdown;
}

void queue_shutdown(queue_t q) {
    q->shutdown = true;
}