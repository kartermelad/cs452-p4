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
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
  };

// Initialize a new queue with the given capacity
queue_t queue_init(int capacity) {
    queue_t q = malloc(sizeof(struct queue));
    if (!q) {
        return NULL;
    }

    q->buffer = malloc(capacity * sizeof(void *));
    if (!q->buffer) {
        free(q);
        return NULL;
    }

    q->capacity = capacity;
    q->size = 0;
    q->front = 0;
    q->rear = 0;
    q->shutdown = false;

    // Initialize mutex and condition variables
    pthread_mutex_init(&q->lock, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);

    return q;
}

// Destroy the queue and free all resources
void queue_destroy(queue_t q) {
    if (!q) {
        return;
    }

    pthread_mutex_lock(&q->lock);
    q->shutdown = true;
    pthread_cond_broadcast(&q->not_empty);
    pthread_cond_broadcast(&q->not_full);
    pthread_mutex_unlock(&q->lock);

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&q->lock);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);

    free(q->buffer);
    free(q);
}

// Add an element to the queue
void enqueue(queue_t q, void *data) {
    pthread_mutex_lock(&q->lock);

    while (q->size == q->capacity && !q->shutdown) {
        pthread_cond_wait(&q->not_full, &q->lock);
    }

    if (q->shutdown) {
        pthread_mutex_unlock(&q->lock);
        return;
    }

    // Add the element to the rear of the queue
    q->buffer[q->rear] = data;
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;

    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->lock);
}

// Remove and return an element from the queue
void *dequeue(queue_t q) {
    pthread_mutex_lock(&q->lock);

    if (q->size == 0 && !q->shutdown) {
        pthread_mutex_unlock(&q->lock);
        return NULL;
    }

    while (q->size == 0 && !q->shutdown) {
        pthread_cond_wait(&q->not_empty, &q->lock);
    }

    if (q->shutdown && q->size == 0) {
        pthread_mutex_unlock(&q->lock);
        return NULL;
    }

    // Remove the element from the front of the queue
    void *data = q->buffer[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;

    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->lock);

    return data;
}

// Signal that the queue is shutting down
void queue_shutdown(queue_t q) {
    pthread_mutex_lock(&q->lock);
    q->shutdown = true;
    pthread_cond_broadcast(&q->not_empty);
    pthread_cond_broadcast(&q->not_full);
    pthread_mutex_unlock(&q->lock);
}

// Check if the queue is empty
bool is_empty(queue_t q) {
    pthread_mutex_lock(&q->lock);
    bool empty = (q->size == 0);
    pthread_mutex_unlock(&q->lock);
    return empty;
}

// Check if the queue is shutting down
bool is_shutdown(queue_t q) {
    pthread_mutex_lock(&q->lock);
    bool shutdown = q->shutdown;
    pthread_mutex_unlock(&q->lock);
    return shutdown;
}