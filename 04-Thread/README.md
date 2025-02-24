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
- **Hint**: The program creates two threads, each printing a message along with its thread ID. The `pthread_join` function ensures that the main program waits for both threads to complete before exiting.

#### `pthread_create`

Used to create a new thread.

**Syntax:**

```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg);
```

**Parameters:**

- `thread`: Pointer to a `pthread_t` variable to store the thread ID.
- `attr`: Thread attributes (can be `NULL` for default settings).
- `start_routine`: The function to be executed by the thread.
- `arg`: Argument passed to the function (can be `NULL`).

#### `pthread_join`

Used to wait for a thread to complete.

**Syntax:**

```c
int pthread_join(pthread_t thread, void **retval);
```

When a thread finishes (either by returning from the function or calling `pthread_exit`), `pthread_join` ensures that the main program collects its result before proceeding.

#### When does a thread terminate?

- When the function inside the thread completes (`return`).
- When `pthread_exit()` is called.
- When the main program exits before calling `pthread_join` (which can cause issues if threads are still running).

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

#### Why is a Mutex Needed in This Program?

In a multi-threaded program, when multiple threads access and modify a shared variable simultaneously, race conditions can occur. A race condition happens when multiple threads attempt to update a shared resource at the same time, leading to unpredictable results.

Using a mutex (mutual exclusion) ensures that only one thread at a time modifies the shared variable, preventing race conditions and ensuring data integrity.

#### What happens if we remove the mutex?

- Without a mutex, multiple threads might read the same value of `counter` and increment it simultaneously, leading to incorrect results.
- The final value of `counter` may be less than the expected value due to lost updates.

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

| Exercise | Directory                 | Compile Command | Run Command | Clean Build Files |
| -------- | ------------------------- | --------------- | ----------- | ----------------- |
| EX1      | `threads_created/`        | `make`          | `./ex1`     | `make clean`      |
| EX2      | `threads_sync_mutex/`     | `make`          | `./ex2`     | `make clean`      |
| EX3      | `threads_condition_vars/` | `make`          | `./ex3`     | `make clean`      |
| EX4      | `threads_even_odd/`       | `make`          | `./ex4`     | `make clean`      |
| EX5      | `threads_rw_resource/`    | `make`          | `./ex5`     | `rm main`         |
| EX6      | `threads_sum_mutex/`      | `make`          | `./ex6`     | `rm main`         |

---

## Summary

Multithreading in Linux helps improve application performance and better utilize system resources. Through these exercises, you will gain an understanding of thread creation, synchronization, and control mechanisms to avoid race conditions. 🚀
