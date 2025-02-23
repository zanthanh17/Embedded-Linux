# Lesson4 - Thread

## Overview

In this lesson, we will explore multithreading programming in an Embedded Linux environment. You will learn how to create, manage, and synchronize threads using the `pthread` library. This knowledge is crucial for system programming and optimizing application performance.

---

## Objectives

By the end of this lesson, you will be able to:

- Understand the concept of threads in Linux.
- Create and manage threads using `pthread`.
- Synchronize threads with mutexes, condition variables, and read-write locks.
- Handle resource contention issues in multithreading.

---

## Key Topics

- **Introduction to Threads in Linux**
- **Creating and Managing Threads**
- **Thread Synchronization**
- **Using Mutex and Condition Variables**
- **Read-Write Locks in Threads**
- **Practical Applications of Threads**

---

## Commands and Tools

| Command/Library                | Functionality                     |
| ------------------------------ | --------------------------------- |
| `pthread_create`               | Create a new thread               |
| `pthread_join`                 | Wait for a thread to terminate    |
| `pthread_mutex_lock/unlock`    | Synchronize resource access       |
| `pthread_cond_wait/signal`     | Manage conditions between threads |
| `pthread_rwlock_rdlock/wrlock` | Manage read/write access control  |

---

## Exercises

### **Exercise 1: Basic Thread Creation**

- **Description**: Write a program that creates two threads, each printing a message along with its thread ID.
- **Hint**: Use `pthread_self()` to retrieve the current thread ID.

```c
void *print_message(void *arg) {
    printf("Thread ID: %lu\n", pthread_self());
    return NULL;
}
```

### **Exercise 2: Thread Synchronization with Mutex**

- **Description**: Create three threads, each incrementing a `counter` variable 1,000,000 times.
- **Hint**: Use `pthread_mutex_lock/unlock` to ensure data safety.

```c
pthread_mutex_t lock;
int counter = 0;

void *increment(void *arg) {
    for (int i = 0; i < 1000000; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}
```

### **Exercise 3: Synchronizing Producer-Consumer**

- **Description**: Create a producer thread generating random numbers and a consumer thread reading those values.
- **Hint**: Use `pthread_cond_wait/signal` for thread synchronization.

### **Exercise 4: Counting Even and Odd Numbers**

- **Description**: Split a list of numbers into two parts, with one thread counting even numbers and another counting odd numbers.
- **Hint**: No mutex is needed since threads operate on separate data sets.

### **Exercise 5: Read-Write Lock**

- **Description**: Create 5 reader threads and 2 writer threads accessing a shared `data` variable.
- **Hint**: Use `pthread_rwlock_rdlock` to allow multiple readers concurrently.

---

## Instructions to Build and Run Exercises

| Exercise | Directory | Compile Command               | Run Command | Clean Build Files |
| -------- | --------- | ----------------------------- | ----------- | ----------------- |
| BT1      | `BT1/`    | `make`                        | `./main`    | `make clean`      |
| BT2      | `BT2/`    | `make`                        | `./main`    | `make clean`      |
| BT3      | `BT3/`    | `make`                        | `./main`    | `make clean`      |
| BT4      | `BT4/`    | `make`                        | `./main`    | `make clean`      |
| BT5      | `BT5/`    | `gcc -o main main.c -pthread` | `./main`    | `rm main`         |

---

## Summary

Multithreading in Linux helps improve application performance and better utilize system resources. Through these exercises, you will gain an understanding of thread creation, synchronization, and control mechanisms to avoid race conditions. 🚀
