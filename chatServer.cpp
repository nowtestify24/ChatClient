



#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <vector>			//Necessary c++ libraries
#include <algorithm>
#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

void sig_chld(int signo)			//child process signal handler
{
	pid_t pid;
	int stat;
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) 
		printf("child %d terminated.\n", pid);
	return ;
}


struct chatArgument
{
        int argumentCount;
        bool validCommand;
        char * tokens [7];                              //struct used to hold the chat arguments. 

};




chatArgument getChatArgument(char * x);


struct userInfo
{
	int socketNum;					//struct used to store logged in user info such as logged in, username and socket num
	char username[30];
	bool loggedIn;

};

vector<int> sock_vector;			//two vectors, one to store users connected to server and another of pure socket
vector<userInfo> user_vector;

fd_set allset;

int endProgram();

int sockfd;
void sigCancel(int signo)
{						
	auto itr=sock_vector.begin();	//signal handler fucntion that will close all connected clients when ctrl-c is pressed for the server.
	auto itr2=user_vector.begin();

	while(itr!=sock_vector.end())
	{
                        int num, fd;
                        fd = *itr;
                        if (FD_ISSET(fd, &allset)) 
			{

				   close(fd);
                                        FD_CLR(fd, &allset);
                                        cout<<"Server: Client Disconnected!"<<endl;
                                        itr = sock_vector.erase(itr);
                                        itr2=user_vector.erase(itr2);
                                        continue;

			}
		++itr;
		++itr2;
	}
	close(sockfd);
	exit(0);
}




int main(int argc, char * argv[])
{
	int rec_sock;
	socklen_t len;
	// vector<int>::iterator itr;
	struct sockaddr_in addr, recaddr;
	struct sigaction abc;
	char buf[100];
	fd_set rset;
	int maxfd;

	abc.sa_handler = sig_chld;
	sigemptyset(&abc.sa_mask);				//signal handlers of child processes and ctrl-c
	abc.sa_flags = 0;

	sigaction(SIGCHLD, &abc, NULL);


	struct sigaction blue;
	blue.sa_handler=sigCancel;
	sigemptyset(&blue.sa_mask);
	blue.sa_flags=0;
	sigaction(SIGINT,&blue,NULL);





	if (argc < 2) {
		printf("Usage: a.out config_file.\n");
		exit(0);
	}
	
	char storage[20];
	
		ifstream myFile;

		

		myFile.open(argv[1]);

		if(!myFile)
		{
			printf("Config file not found.\n");
			exit(0);
		}

		myFile.getline(storage,20,' ');

		char portNum[20];
		myFile.getline(portNum,20);

		myFile.close();
		cout<<"Port Number from file is: "<<portNum<<endl;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror(": Can't get socket");
		exit(1);
	}

	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_family = AF_INET;				//set up our information based on the input



	addr.sin_port = htons((short)atoi(portNum));



	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror(": bind");
		exit(1);
	}


	len = sizeof(addr);
	if (getsockname(sockfd, (struct sockaddr *)&addr, &len) < 0) {
		perror(": can't get name");
		_exit(1);
	}

	printf("ip = %s, port = %d\n", inet_ntoa(addr.sin_addr), htons(addr.sin_port));

	if (listen(sockfd, 5) < 0) 		//listen for connection
	{
		perror(": bind");
		exit(1);
	}


	FD_ZERO(&allset);		//reset our list of file descriptors
	FD_SET(sockfd, &allset);
	maxfd = sockfd;

	sock_vector.clear();

	while (1) {
		rset = allset;
		select(maxfd+1, &rset, NULL, NULL, NULL);
		if (FD_ISSET(sockfd, &rset)) {
			/* somebody tries to connect */
			if ((rec_sock = accept(sockfd, (struct sockaddr *)(&recaddr), &len)) < 0) {
				if (errno == EINTR)
					continue;
				else {
					perror(":accept error");
					exit(1);
				}
			}

			/*
			if (rec_sock < 0) {
				perror(": accept");
				exit(1);
			}
			*/

			/* print the remote socket information */

			printf("Client info: remote machine = %s, port = %d.\n",
					inet_ntoa(recaddr.sin_addr), ntohs(recaddr.sin_port)); 

			sock_vector.push_back(rec_sock);

			userInfo myInfo;
			myInfo.socketNum=rec_sock;
			strcpy(myInfo.username,"");
			myInfo.loggedIn=false;
			user_vector.push_back(myInfo);


			FD_SET(rec_sock, &allset);
			if (rec_sock > maxfd) maxfd = rec_sock;
		}
		char copy[100];
		// vector<int>::iterator itr = sock_vector.begin();
		auto itr = sock_vector.begin(); 			//iterators for our storage vectors
		auto itr2 = user_vector.begin();
		while (itr != sock_vector.end()) {
			int num, fd;
			fd = *itr;
			if (FD_ISSET(fd, &rset)) {
				num = read(fd, buf, 100);
				buf[num]='\0';
				
				//cout<<"Buffer is: "<<buf<<endl;
				strcpy(copy,buf);
				//cout<<"Copy is: "<<copy<<endl;

				char * firstToken = strtok(copy," ");
			


				if (num == 0) 
				{						//use this code for exit case. Check to see if user is logged out,
										//send message if it isn't.
					/* client exits */
					close(fd);
					FD_CLR(fd, &allset);
					cout<<"Server: Client Disconnected!"<<endl;
					itr = sock_vector.erase(itr);	
					itr2=user_vector.erase(itr2);
					continue;
				}
		else 
			{

				if(strcmp(firstToken,"chat")==0)
				{

					//loop here to see if user is logged in or not. 
						bool chatGood=false;
					auto checkChat =user_vector.begin();
					int tracker=0;
					while(checkChat !=user_vector.end())
					{
						if(fd==user_vector[tracker].socketNum)
						{
							if(user_vector[tracker].loggedIn==true)
							{
								chatGood=true;
								break;
							}
						}
						
						++checkChat;
						tracker+=1;
					}	
					
					if(chatGood==true)
					{
				
						bool dm=false;
						for(int i=0;i<num;i++)
						{
							if(buf[i]=='@')
							{
								cout<<"direct message found!"<<endl;
								dm =true;
							}
						}
					if(dm==true)
					{
						cout<<"Server: Handling direct message.."<<endl;		//case where we send DMS to logged in users
			

						string user;
						  auto why=user_vector.begin();
                                                while(why!=user_vector.end())
                                                {
                                                        userInfo myInfo=*why;
                                                        if(myInfo.socketNum==fd)
                                                        {                               //append the username to our chat string we are sending
                                                                user=myInfo.username;
                                                                user=user+ " ";
                                                                user=user+">> ";
                                                        }

                                                        ++why;
                                                }

									
						string at ="@";
						string bufHandler(buf);
						int green=bufHandler.find(at.c_str(),0);
						string temp =bufHandler.substr(green);
					cout<<"temp is: "<<temp<<endl;
						at.clear();
						at=" ";
						string red=temp;
						 char mesa[200];
						strcpy(mesa,red.c_str());	
						 char* token = strtok(mesa, " ");
						 string grey(token);
						grey=grey.substr(1,grey.size()-1);
							
						int blue =temp.find(at.c_str(),0);			//tokenize the string
						string temp2=temp.substr(blue);
						//cout<<"The message is: "<<temp2<<endl;
						//cout<<"The user is: "<<grey<<endl;
						string final;
						temp2=user+temp2;
						auto yellow=user_vector.begin();
						int tracker=0;
						bool foundUser=false;
						while(yellow !=user_vector.end())
						{
							userInfo slot=*yellow;
							
							if(strcmp(user_vector[tracker].username,grey.c_str())==0)	//if the user is in the server send the message
							{
								string mes="800";
								write(user_vector[tracker].socketNum,temp2.c_str(),temp2.size());	//send the Direct message
								write(fd,mes.c_str(),mes.size());		//write success message to client
								cout<<"Server: Message Sent Successfully"<<endl;
								foundUser=true;	
								break;
							}
							tracker+=1;	
							++yellow;
						}
							if(foundUser==false)
							{
								cout<<"Server: failed to find user. Message not sent"<<endl;
								string mes="802";		//send error message back to client
								write(fd,mes.c_str(),mes.size());
							}
					}
					else
					{			

						string user;
						auto why=user_vector.begin();
						while(why!=user_vector.end())
						{
							userInfo myInfo=*why;
							if(myInfo.socketNum==fd)
							{				//append the username to our chat string we are sending
								user=myInfo.username;
								user=user+ " ";
								user=user+">> ";
							}		
				
							++why;
						}


						string blue;
						auto x =user_vector.begin();
						while(x!=user_vector.end())
						{
						userInfo temp=*x;
						//cout<<"We have socket num: "<<temp.socketNum<<endl;
						string boo(buf);
						boo=boo.substr(5);
						string send="800";
						send+=" "+boo;
						boo=user+boo;
						blue=send;
						  if(temp.socketNum!=fd && temp.loggedIn==true)			//if command is chat, write to everyone but yourself. 
						  {
							write(temp.socketNum,boo.c_str(),boo.size());
					       	  }
					++x;
						}	
					
						
						blue="800";					
						write(fd,blue.c_str(),blue.size());		//send success message back to the client
					}	

				}
					
					
					else
					{
						cout<<"Server: User must login to send a chat"<<endl;
						string temp="801";		//tell user must be logged in to send a chat
						write(fd,temp.c_str(),temp.size());
					}

				 }

					else if(strcmp(firstToken,"login")==0)
					{

						string temp(buf);
						
							temp=temp.substr(6);
							
							string temp2=temp;
							//cout<<"Your user name is: "<<temp2<<endl;
							cout<<"Server: Login Command given"<<endl;
							auto userItr = user_vector.begin();
							int x=0;
							while(userItr !=user_vector.end())		//loop through our vector
							{					
								userInfo temp=*userItr;
								if(user_vector[x].socketNum==fd)		//find the current connection that wants to login
								{
										strcpy(temp.username,temp2.c_str());
										if(user_vector[x].loggedIn==false)		//if the connection isn't logged in
										{
	
											user_vector[x].loggedIn=true;		//set the login to be true
											char message[100];
											strcpy(message,"603");
											int size=strlen(message);
											strcpy(user_vector[x].username,temp.username);	//copy the username
											cout<<"Server: Login Successful!"<<endl;
											write(fd,message,size);
											break;
										}
										else		//we are already logged in. Let the single user know this
										{
											char message[100];
											strcpy(message,"602");
											cout<<"Server: Login Failed!"<<endl;
											int size=strlen(message);
											write(fd,message,size);

										}
				
										break;
								}
								userItr++;
								x+=1;
							}


					}
				
					else if(strcmp(firstToken,"logout")==0)
					{
						auto x=user_vector.begin();
						int tracker=0;

						while(x!=user_vector.end())
						{
							if(user_vector[tracker].socketNum==fd)
							{
								if(user_vector[tracker].loggedIn==true)
								{						//if user is logged in then log them out
									user_vector[tracker].loggedIn=false;
									string response="600";
									cout<<"Server: User logged Out status: "<<user_vector[tracker].loggedIn<<endl;
									write(fd,response.c_str(),response.size());
									break;
								}
								else
								{
									string response="601";		//if user is already logged out then inform the user
									write(fd,response.c_str(),response.size());
									break;
								}								
							}

							tracker+=1;	
							x++;
						}
					}


					else if(strcmp(firstToken,"exit")==0)
					{
					
						auto lego=user_vector.begin();			//handles exit case
						while(lego !=user_vector.end())
						{
							userInfo myinfo=*lego;
							cout<<"Server: Login status:" <<myinfo.loggedIn<<endl;
							if(myinfo.socketNum==fd)
							{
								if(myinfo.loggedIn==true)
								{
		
								
								char sendBack[50];
								strcpy(sendBack,"604");
								int size=strlen(sendBack);
								sendBack[size]='\0';
								write(fd,sendBack,size);
								break;
								}
								else
								{
									char sendBack[50];
									strcpy(sendBack,"exit");
									int size=strlen(sendBack);
									write(fd,sendBack,size);
									break;
								}
										
							}

							++lego;
						}

					}	

					else					//write to yourself not valid command. Send back list of valid commands
					{
						string ero="500";
						write(fd,ero.c_str(),num);
					}



				}
			}
			++itr;
			++itr2;
		}

		maxfd = sockfd;
		if (!sock_vector.empty()) {
			maxfd = max(maxfd, *max_element(sock_vector.begin(),
						sock_vector.end()));
		}
	}

return 0;
}



