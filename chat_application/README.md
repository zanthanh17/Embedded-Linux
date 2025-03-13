# Chat Application

## Overview
The **Chat Application** is a peer-to-peer (P2P) chat system implemented in C for embedded Linux systems. It allows multiple instances to communicate over a network, supporting features like:

- **Peer-to-Peer Communication**: Connect multiple instances running on different ports or machines.
- **Reverse Connections**: Ensures full network connectivity by automatically establishing reverse connections.
- **Multithreading**: Uses POSIX threads (`pthread`) to handle multiple clients concurrently.
- **Command-Line Interface (CLI)**: Provides a simple CLI for user interaction.
- **Connection Management**: List active connections, send messages, and terminate connections.
- **Error Handling**: Robust error handling for socket operations, threading, and user input.
- **Colorized Output**: Uses ANSI color codes to enhance console readability.

This project was developed as part of a study on **embedded Linux systems**, focusing on **network programming, multithreading, and inter-process communication**.

---

## Prerequisites
### Required Software & Libraries
- **Operating System**: Linux (embedded Linux preferred, but works on any Linux distribution).
- **Compiler**: GCC (GNU Compiler Collection).
- **Libraries**:
  - POSIX Threads (`pthread`) for multithreading.
  - Standard C libraries (`stdio`, `stdlib`, `string`, etc.).
  - Network libraries (`sys/socket.h`, `arpa/inet.h`, etc.).
- **Build Tool**: `make` for compiling the project.
- **Network Access**: Ensure the system has network access, and necessary ports are open.

---

## Installation
### Clone the Repository
```bash
git clone <repository-url>
cd chat-application
```

### Project Structure
```plaintext
chat-application/
├── inc/               # Header files
│   ├── cli.h
│   ├── connection.h
│   ├── network.h
│   └── utils.h
├── src/               # Source files
│   ├── cli.c
│   ├── connection.c
│   ├── main.c
│   ├── network.c
│   └── utils.c
├── Makefile           # Build script
├── app                # Compiled executable (generated after building)
└── README.md          # This file
```

### Build the Project
```bash
make
```
This will compile all source files and generate the executable **app**.

### Clean the Build (Optional)
```bash
make clean
```

---

## Usage
### Running the Application
Start an instance by specifying a port number:
```bash
./app <port>
```
Example:
```bash
./app 4000
./app 5000
./app 6000
```

### Available Commands
| Command                | Description |
|------------------------|-------------|
| `help`                | Show available commands |
| `connect <ip> <port>` | Connect to a peer at the specified IP and port |
| `send <id> <message>` | Send a message to the specified connection ID |
| `list`                | List all active connections |
| `terminate <id>`      | Terminate the connection with the specified ID |
| `exit`                | Exit the application gracefully |

### Example Scenario
1. Start three instances:
   ```bash
   ./app 4000
   ./app 5000
   ./app 6000
   ```
2. In Terminal 1 (port 4000), connect to port 5000:
   ```bash
   connect 127.0.0.1 5000
   ```
   - This establishes a connection between 4000 and 5000.
   - A reverse connection (5000 → 4000) is created.
   - Port 6000 is notified and connects to 5000.
3. List connections:
   ```bash
   list
   ```
   - Terminal 1: Shows connections to 5000 and 6000.
   - Terminal 2: Shows connections to 4000 and 6000.
   - Terminal 3: Shows connections to 4000 and 5000.
4. Send messages:
   ```bash
   send 0 "Hello from 4000"
   ```
5. Terminate a connection:
   ```bash
   terminate 0
   ```
6. Exit the application:
   ```bash
   exit
   ```
   Or press `Ctrl+C` for graceful shutdown.

---

## Troubleshooting
### 1. "Bind failed: Address already in use"
**Cause:** The specified port is already in use.
**Solution:**
```bash
ss -tuln | grep 4000  # Check if port is in use
sudo lsof -i :4000    # Find process using the port
kill -9 <PID>         # Kill the process
```
Alternatively, use a different port:
```bash
./app 4001
```

### 2. "Connection failed"
**Cause:** Target IP/port is not reachable.
**Solution:**
- Ensure the peer application is running.
- Check network connectivity:
  ```bash
  ping 127.0.0.1
  ```
- Verify firewall settings.

### 3. "Maximum connections reached"
**Cause:** Active connections exceed `MAX_CONNECTIONS`.
**Solution:**
- Terminate unused connections.
- Increase `MAX_CONNECTIONS` in `connection.h` and recompile.

### 4. "Invalid connection ID"
**Cause:** The specified ID does not exist.
**Solution:**
- Use `list` to check valid IDs.
- Ensure correct ID when using `send` or `terminate`.

---

## Future Improvements
- **Message Persistence**: Store messages for history.
- **Broadcast Messaging**: Send messages to all peers.
- **Better IP Handling**: Improve `get_my_ip` functionality.

---

## License
This project is licensed under the **MIT License**.

