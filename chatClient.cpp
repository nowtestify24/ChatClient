
#include <iostream>
#include <cstring>
#include <fstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string>
#include <pthread.h>

using namespace std;
const unsigned MAXBUFLEN = 256;
int clientfd;

void *process_connection(void *arg) {
	int n;
	char buf[MAXBUFLEN];
	pthread_detach(pthread_self());
	while (1) 
	{
		n = read(clientfd, buf, MAXBUFLEN);
		if (n <= 0) {
			if (n == 0) 
			{
				cout << "server closed" << endl;
			} else 
			{
				cout << "something wrong" << endl<<endl;
			}
			close(clientfd);
			// we directly exit the whole process.
			exit(1);
		}
		buf[n] = '\0';
		if(strcmp(buf,"exit")==0)
		{
			cout<<"Exit command Received! Goodbye!"<<endl<<endl;
			close(clientfd);
			break;
			
		}
		if(strcmp(buf,"800")==0)
		{
			cout<<"Message sent Sucessfully!"<<endl<<endl;
			continue;	
		}
											//all of these chains of if statemetns here are our protocal we have made
		if(strcmp(buf,"801")==0)
		{
			cout<<"Message failed to send!"<<endl<<endl;
			continue;
		}

		if(strcmp(buf,"802")==0)
		{
			cout<<"Message failed to sent to User"<<endl<<endl;
			continue;
		}

		if(strcmp(buf,"600")==0)
		{

			cout<<"Logged out Successfully!"<<endl<<endl;
			continue;
		}

		if(strcmp(buf,"601")==0)
		{
			cout<<"Failed to logout."<<endl<<endl;
			continue;
		}
		if(strcmp(buf,"602")==0)
		{
			cout<<" You are already logged in"<<endl<<endl;
			continue;
		}

		if(strcmp(buf,"603")==0)
		{
			cout<<"Successful login!"<<endl<<endl;
			continue;
		}
		if(strcmp(buf,"604")==0)
		{
			cout<<"You must logout in order to exit!"<<endl<<endl;
		continue;	
	}

		if(strcmp(buf,"500")==0)
		{

			cout<<"Only four valid commands:"<<endl;
			cout<<"Login, Logout, Chat or Exit"<<endl;		//Inform user of correct commands only
			cout<<"Try again!"<<endl<<endl;
		}
		else
		{
		cout << buf << endl;
		cout<<endl;
		}

	}
	exit(0);
}

//const unsigned serv_port = 5100;

int main(int argc, char **argv) {
	int rv, flag;
	// ssize_t n;
	struct addrinfo myAddr, *res, *ressave;
	pthread_t tid;

	if (argc != 2) {
		cout << "chat_client config_file" << endl;
		exit(1);
	}

	ifstream myFile;

	myFile.open(argv[1]);

	if(!myFile)
	{
		cout<<"Could not open File!";
		exit(0);
	}

	char firstOne[20];
	char firstReal[20];

	char secondOne[20];
	char secondReal[20];

	myFile.getline(firstOne,20,' ');
	myFile.getline(firstReal,20);
	cout<<"servHost is: "<<firstReal<<endl;

	myFile.getline(secondOne,20,' ');
	myFile.getline(secondReal,20);


	char realArgument1[20];
	char realArgument2[20];

	

	if(strcmp(firstOne,"servhost:")==0)
	{
		strcpy(realArgument1,firstReal);
		strcpy(realArgument2,secondReal);
	}

	else
	{
		strcpy(realArgument2,secondReal);
		strcpy(realArgument1,firstReal);		//we do this to make sure if config file is wrong then we still bind correctly
	}

	myFile.close();

	bzero(&myAddr, sizeof(struct addrinfo));
	myAddr.ai_family = AF_UNSPEC;
	myAddr.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(realArgument1, realArgument2, &myAddr, &res)) != 0) {
		cout << "getaddrinfo wrong: " << gai_strerror(rv) << endl;
		exit(1);
	}

	ressave = res;
	flag = 0;
	do {
		clientfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (clientfd < 0) 
			continue;
		if (connect(clientfd, res->ai_addr, res->ai_addrlen) == 0) {
			flag = 1;
			break;
		}
		close(clientfd);
	} while ((res = res->ai_next) != NULL);
	freeaddrinfo(ressave);

	if (flag == 0) {
	//	fprintf(stderr, "cannot connect\n");
		exit(1);
	}

	pthread_create(&tid, NULL, &process_connection, NULL);			//thread used to handle input/output of the client

	string myInput;
	while (getline(cin, myInput)) 
	{

			write(clientfd, myInput.c_str(), myInput.length());

	}
	return 0;
}
