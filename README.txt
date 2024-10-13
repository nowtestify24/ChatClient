


Client Server chatroom program readMe file

Overview
This Client-Server Chatroom program is designed to facilitate real-time communication between multiple clients connected to a single server. 
It employs multi-threading and socket programming to manage concurrent connections and ensure efficient message delivery. 

This program was designed for a Unix environment.

Features
Multi-threading: Utilizes P-threads to handle multiple clients simultaneously.
Command Handling: Supports specific commands (chat, login, logout, exit) in lowercase format.
Custom Protocol: Implements a custom protocol for message and command handling.
Connection Management: Uses select() to manage multiple client connections and handle I/O operations.
Direct Messaging: Supports direct messaging between clients.
Graceful Termination: Allows clients and server to disconnect gracefully using ctrl-c.


Configuration
Client Configuration: The client_config file contains the server name and port number.
Server Configuration: The server_config file contains the port number.

Technical Details
Language: C++
Multi-threading: P-threads for concurrent client handling
Socket Programming: Uses sockets for network communication
Select Function: Manages multiple file descriptors for client connections

chatClient.cpp
----------------------------
P-thread explanation: 

	-p threads are used in this project to read and write in put to our server buffer.


	- By using these threads we are abel to handle multiple clients at once upon the same server application.

	-phthreads are called at the end of our main function and do the majority of the work to the server.



	-Client accepts the following four commands ONLY in lower case format: chat login logout and exit

	-client can only exist when they are not logged in

	-Client can only chat when they are logged in. Same with receiving messages from the server


	-Commands need arguments! valid commands WITHOUT argumetns will end the program

	- Client is informed when an invalid command is used with our server. 

	-Client interprets our own protocol codes and displays messages accordingly


	-Client ends when ctrl-c is entered

	-client reads our codes from the server to print messages

chatServer.cpp
----------------------------

Select explanation:

		-Vectors are used to store connection info, used by select. We theorertically can have many connections but we tested with 4 with potential of say 10
		-The server application uses the select function to handle what file descriptors that contain our client.
	
		- We use this function to determine what has been written to and what is in need of being read by the server. We also use it to handle our incoming connect		ions

		-Server displays messages such as the client IP address and port number upon connection.
		-Server displays actions such as chat sent and items of that nature


		-Direct messsages are handled by the server

		-Server keeps track of which connections are logged in and will only send chats to those
	
	
		-Uses our own protocal to send messages to the client, the client can then interpret these.
	
		-server disconnects itself and client when ctrl-c is entered while it is running

Our own chat protocol Codes:
----------------------------
Below is our protocol codes for various cases that the server gives the client

	Chat Messages
	--------------
	800 -  message sent successfully

	801 - messsage failed to send

	802 - failed to send to user

	
	Login/Logout Messages:
	------------
	600 - Logout suceeded

	601 - Logout Failed

	602 - you are already logged in
	 
	603 - Successful login
	
	604 - logout before exiting

	General:
	--------
	500 - Incorrect Command


