# Chat Application

## Overview

The Chat Application is a peer-to-peer (P2P) chat system implemented in C for embedded Linux systems. It allows multiple instances to communicate over a network, supporting features like establishing connections, sending messages, listing active connections, and terminating connections. The application uses POSIX threads for concurrent handling of clients and servers, and it implements a reverse connection mechanism to ensure all nodes can communicate.

This project was developed as a study on embedded Linux systems, focusing on network programming, multithreading, and inter-process communication.

## Features

- Peer-to-Peer Communication: Connect multiple instances running on different ports or machines.
- Reverse Connections: Automatically establishes reverse connections for full network connectivity.
- Multithreading: Uses POSIX threads to handle multiple clients concurrently.
- Command-Line Interface (CLI): Provides a simple CLI for interaction.
- Connection Management:
  - List active connections.
  - Send messages to specific connections.
  - Terminate connections.
- Error Handling: Robust handling for socket operations, threading, and user input.
- Colorized Output: Uses ANSI color codes for better console readability.

## Prerequisites

To build and run this application, you need:
- Operating System: Linux (preferably embedded Linux, but works on any Linux distribution).
- Compiler: GCC.
- Libraries:
  - POSIX Threads (pthread) for multithreading.
  - Standard C libraries (stdio, stdlib, string, etc.).
  - Network libraries (sys/socket.h, arpa/inet.h, etc.).
- Build Tool: make.
- Network Access: Ensure network access and ports are not blocked by a firewall.

## Installation

1. Clone the Repository (if hosted on Git):
   git clone <repository-url>
   cd chat-application

2. Project Structure:
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

3. Build the Project:
   make

4. Clean the Build (Optional):
   make clean

## Usage

### Running the Application

1. Start the Application:
   Run the application by specifying a port number. Each instance must run on a unique port.
   ./app <port>
   Example:
   - Terminal 1: ./app 4000
   - Terminal 2: ./app 5000
   - Terminal 3: ./app 6000

2. Available Commands:
   - help: Displays the list of available commands.
   - connect <ip> <port>: Connects to a peer at the specified IP and port.
     Example: connect 127.0.0.1 5000
   - send <id> <message>: Sends a message to the connection with the specified ID.
     Example: send 0 Hello World
   - list: Lists all active connections with their IDs, IPs, and ports.
   - terminate <id>: Terminates the connection with the specified ID.
     Example: terminate 0
   - exit: Exits the application.

3. Example Scenario:
   - Start three instances:
     - Terminal 1: ./app 4000
     - Terminal 2: ./app 5000
     - Terminal 3: ./app 6000
   - In Terminal 1, connect to port 5000:
     connect 127.0.0.1 5000
   - List connections in each terminal:
     list
   - Send messages:
     - Terminal 1: send 0 "Hello from 4000"
     - Terminal 2: send 0 "Hello from 5000"
     - Terminal 3: send 0 "Hello from 6000"
   - Terminate a connection (e.g., in Terminal 1):
     terminate 0
   - Exit:
     exit

### Notes
- Ensure ports (e.g., 4000, 5000) are not in use.
- Use 127.0.0.1 for local testing. For different machines, use the target machine's IP.

## Code Structure

### Source Files (src/)
- main.c: Entry point. Initializes server thread and handles user input.
- cli.c: Implements the CLI (process_command, show_help).
- network.c: Handles network operations (socket creation, connections, message sending).
- connection.c: Manages active connections (init_connections, list_connections, terminate_connection).
- utils.c: Utility functions (draw_border, show_title, get_my_ip).

### Header Files (inc/)
- cli.h: CLI function declarations.
- connection.h: Connection structure and globals (connections, connection_count, conn_mutex).
- network.h: Network function declarations and globals (server_socket, server_ports, server_mutex).
- utils.h: Utility function declarations and ANSI color codes.

### Key Mechanisms
- Reverse Connections: Automatically creates reverse connections for full network connectivity.
- Multithreading: Each client connection is handled in a separate thread.
- Mutexes: Uses pthread_mutex_t for synchronized access to shared resources.

## Troubleshooting

### 1. "Bind failed: Address already in use"
   - Cause: Port is already in use.
   - Solution:
     1. Check processes using the port:
        ss -tuln | grep 4000
     2. Kill the process:
        sudo lsof -i :4000
        kill -9 <PID>
     3. Use a different port:
        ./app 4001

### 2. "Connection failed"
   - Cause: Target IP/port not reachable.
   - Solution:
     - Ensure the peer is running.
     - Check network connectivity (ping 127.0.0.1).
     - Verify firewall settings.

### 3. "Maximum connections reached"
   - Cause: Exceeded MAX_CONNECTIONS limit.
   - Solution:
     - Terminate some connections: terminate <id>
     - Increase MAX_CONNECTIONS in connection.h and recompile.

### 4. "Invalid connection ID"
   - Cause: Specified ID is not active.
   - Solution:
     - Use list to check active IDs.
     - Ensure the ID is correct.

## Future Improvements

- Add message persistence (store messages in a file).
- Implement broadcast messaging (send to all peers).
- Improve IP handling (select specific network interface).
- Add GUI support (e.g., using GTK or Qt).
- Add encryption (e.g., SSL/TLS).
- Support IPv6 addresses.

## Contributing

1. Fork the repository.
2. Create a branch:
   git checkout -b feature-name
3. Commit changes:
   git commit -m "Add feature-name"
4. Push to your fork:
   git push origin feature-name
5. Create a pull request.

## License

This project is licensed under the MIT License. See the LICENSE file for details.
