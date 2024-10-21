# Client-Server Chatroom Program

## Overview
This Client-Server Chatroom program is designed to facilitate real-time communication between multiple clients connected to a single server. It employs multi-threading and socket programming to manage concurrent connections and ensure efficient message delivery. This program was designed for a Unix environment.

## Features
- **Multi-threading**: Utilizes P-threads to handle multiple clients simultaneously.
- **Command Handling**: Supports specific commands (`chat`, `login`, `logout`, `exit`) in lowercase format.
- **Custom Protocol**: Implements a custom protocol for message and command handling.
- **Connection Management**: Uses `select()` to manage multiple client connections and handle I/O operations.
- **Direct Messaging**: Supports direct messaging between clients.
- **Graceful Termination**: Allows clients and server to disconnect gracefully using `ctrl-c`.

## Configuration
- **Client Configuration**: The `client_config` file contains the server name and port number.
- **Server Configuration**: The `server_config` file contains the port number.

## Technical Details
- **Language**: C++
- **Multi-threading**: P-threads for concurrent client handling
- **Socket Programming**: Uses sockets for network communication
- **Select Function**: Manages multiple file descriptors for client connections

## chatClient.cpp

### P-thread explanation:
- P-threads are used in this project to read and write input to our server buffer.
- By using these threads, we are able to handle multiple clients at once on the same server application.
- P-threads are called at the end of our main function and do the majority of the work for the server.
- The client accepts the following four commands ONLY in lowercase format: `chat`, `login`, `logout`, and `exit`.
- The client can only exist when they are not logged in.
- The client can only chat when they are logged in, and the same goes for receiving messages from the server.
- Commands need arguments! Valid commands WITHOUT arguments will end the program.
- The client is informed when an invalid command is used with our server.
- The client interprets our own protocol codes and displays messages accordingly.
- The client ends when `ctrl-c` is entered.
- The client reads our codes from the server to print messages.

## chatServer.cpp

### Select explanation:
- Vectors are used to store connection info, used by `select()`. We can theoretically have many connections, but we tested with 4, with the potential of up to 10.
- The server application uses the `select()` function to handle which file descriptors contain our client.
- We use this function to determine what has been written to and what needs to be read by the server. We also use it to handle our incoming connections.
- The server displays messages such as the client IP address and port number upon connection.
- The server displays actions such as chats sent and items of that nature.
- Direct messages are handled by the server.
- The server keeps track of which connections are logged in and will only send chats to those.
- Uses our own protocol to send messages to the client, which the client can then interpret.
- The server disconnects itself and the client when `ctrl-c` is entered while it is running.

## Our Own Chat Protocol Codes

### Chat Messages
- `800` - Message sent successfully
- `801` - Message failed to send
- `802` - Failed to send to user

### Login/Logout Messages
- `600` - Logout succeeded
- `601` - Logout failed
- `602` - You are already logged in
- `603` - Successful login
- `604` - Logout before exiting

### General
- `500` - Incorrect command
