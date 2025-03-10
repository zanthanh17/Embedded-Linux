# Lesson 6 - Pipe

## Introduction
This repository contains three C programs demonstrating inter-process communication (IPC) using pipes. Each program illustrates different levels of complexity when using pipes to transfer data between processes.

## Table of Contents
1. [Single Pipe: Parent to Child Communication](#single-pipe-parent-to-child-communication)
2. [Multiple Pipes: Communication Between Three Processes](#multiple-pipes-communication-between-three-processes)
3. [Character Counting Using Pipe](#character-counting-using-pipe)
4. [How to Compile and Run](#how-to-compile-and-run)

---

## 1. Single Pipe: Parent to Child Communication

### **Description**
This program creates a **pipe** and forks a **child process**. The **parent process** sends a string message to the **child process**, which reads the message from the pipe and prints it.

### **How It Works**
1. Create a pipe using `pipe(fd)`, where `fd[0]` is for reading and `fd[1]` is for writing.
2. Use `fork()` to create a child process.
3. The parent process:
   - Closes the read end of the pipe (`fd[0]`).
   - Writes a message into the pipe (`write(fd[1], message, strlen(message) + 1)`).
   - Closes the write end (`fd[1]`).
4. The child process:
   - Closes the write end of the pipe (`fd[1]`).
   - Reads the message from the pipe (`read(fd[0], buffer, sizeof(buffer))`).
   - Prints the received message.
   - Closes the read end (`fd[0]`).

### **Expected Output**
```sh
Child received: Hello from parent!
```

---

## 2. Multiple Pipes: Communication Between Three Processes

### **Description**
This program demonstrates communication between **three processes** using **two pipes**:
- **Parent → Child 1** (Pipe 1)
- **Child 1 → Child 2** (Pipe 2)

### **How It Works**
1. Create two pipes: `pipe1` for communication between **Parent → Child 1**, and `pipe2` for **Child 1 → Child 2**.
2. Fork **Child 1**, then fork **Child 2** from Child 1.
3. The parent process:
   - Sends a message to Child 1 through `pipe1`.
4. Child 1:
   - Reads the message from `pipe1`.
   - Modifies the message (e.g., appends text).
   - Sends the modified message to Child 2 through `pipe2`.
5. Child 2:
   - Reads the message from `pipe2`.
   - Prints the received message.

### **Expected Output**
```sh
Child 2 received: Hello from parent! Modified by Child 1.
```

---

## 3. Character Counting Using Pipe

### **Description**
This program uses a **pipe** to send a string from the **parent process** to the **child process**. The child process counts the number of characters in the received string and prints the result.

### **How It Works**
1. Create a pipe (`pipe(fd)`).
2. Fork a child process (`fork()`).
3. The parent process:
   - Closes the read end (`fd[0]`).
   - Writes a message into the pipe (`write(fd[1], message, strlen(message) + 1)`).
   - Closes the write end (`fd[1]`).
4. The child process:
   - Closes the write end (`fd[1]`).
   - Reads the message (`read(fd[0], buffer, sizeof(buffer))`).
   - Uses `strlen(buffer)` to count characters.
   - Prints the count.
   - Closes the read end (`fd[0]`).

### **Expected Output**
```sh
Received string: "Hello, how are you?"
Number of characters: 19
```

---

## 4. How to Compile and Run

### **Compilation**
Use `gcc` to compile the C programs:
```sh
gcc -o expipe Pa_com_child.c
```
```sh
gcc -o expipe cnt_symbols.c
```
```sh
gcc -o expipe multi_process.c
```

### **Execution**
Run the compiled programs:
```sh
./expipe
```
```sh
./expipe
```
```sh
./expipe
```

---

## Conclusion
This repository provides fundamental examples of **pipes in C** for **inter-process communication (IPC)**. It covers:
✅ Basic parent-child communication with **one pipe**.
✅ **Chained communication** between multiple processes using **two pipes**.
✅ Using pipes to **process and manipulate** data between processes.

These examples are useful for **learning IPC** and **understanding how processes communicate in Unix/Linux systems**. 🚀

---

## 🔗 References
- [Linux pipe() system call](https://man7.org/linux/man-pages/man2/pipe.2.html)
- [Inter-Process Communication in C](https://www.geeksforgeeks.org/inter-process-communication-ipc/)

Happy coding! 😃

