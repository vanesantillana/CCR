#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <bits/stdc++.h>
#include <thread>
#include "f.h"
using  namespace std; 

void nuevoUsuario( int ConnectFD){
    string menu="\nMENU\n 1-[Action P] Print list of user on the chat\n 2. [Action L] Login to the chat\n 5. [Action C] Send a msg to an user on the chat\n 6. [Action E]End chat or logout from chat \n";
    my_write2(ConnectFD,menu);
    my_read2(ConnectFD);
    while(1);
}

//g++ client.cpp -o cli -std=c++11 -pthread

int main(void){
  struct sockaddr_in stSockAddr;
  int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  
  int n;
  string msg="";
  
  if(-1 == SocketFD)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }
  
  memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
  
  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = htons(338);
  stSockAddr.sin_addr.s_addr = INADDR_ANY;
  
  if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("error bind failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
  
  if(-1 == listen(SocketFD, 10))
    {
      perror("error listen failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

  
  map<char,int> users;
  
  thread t[10];
  int cont=0;
  for(;;){
    int ConnectFD = accept(SocketFD, NULL, NULL);
    t[cont]=thread(nuevoUsuario,ConnectFD);
  }
  //shutdown(ConnectFD, SHUT_RDWR);
  for(int tr=0;tr<cont;tr++){
    t[tr].join();
  }
  
  //t1[0]=thread(nuevoUsuario,SocketFD);

    //close(ConnectFD);
     
  
  
  close(SocketFD);
  return 0;
}
