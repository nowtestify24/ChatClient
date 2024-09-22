all: chat_server chat_client

chat_server: chatServer.o
	g++ -std=c++11 -o chat_server chatServer.o

chatServer.o: chatServer.cpp
	g++ -std=c++11 -c chatServer.cpp

chat_client: chatClient.o
	g++ -pthread -o chat_client chatClient.o

duanClient.o: chatClient.cpp
	g++ -pthread -c chatClient.cpp
clean:
	rm *.o chat_server chat_client
