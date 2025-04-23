# Multithreaded FIFO Queue

## Description

This project implements a multithreaded FIFO queue to solve the bounded buffer problem. The queue has a fixed capacity and supports concurrent access by multiple producer and consumer threads. Producers add items to the queue, while consumers remove items. The queue uses condition variables and mutexes to ensure thread safety and proper synchronization, blocking threads when the queue is full or empty.

## Building

```bash
make
```

## Testing

```bash
make check
```

## Clean

```bash
make clean
```

## Install Dependencies

In order to use git send-mail you need to run the following command:

```bash
make install-deps
```
