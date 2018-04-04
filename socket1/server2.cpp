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
//g++ client.cpp -o cli -std=c++11 -pthread
using  namespace std; 
char buffer[24];
void writePrueba(int ConnectFD){
  while(1){    
    string msg2;
    cin>>msg2;
    write(ConnectFD,msg2.c_str(),msg2.size()); 
  }
}

void readPrueba(int ConnectFD){
  while(1){
    bzero(buffer,24);
    int n = read(ConnectFD,buffer,24);
    printf("cliente dice: [%s]\n",buffer);
  }
  
}
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
  stSockAddr.sin_port = htons(336);
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
  

  thread t[2];
  for(;;){
    int ConnectFD = accept(SocketFD, NULL, NULL);
    
    if(0 > ConnectFD){
      perror("error accept failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    
    t[1]=thread(readPrueba,ConnectFD);
    t[0]=thread(writePrueba,ConnectFD);
    t[0].join();
    
    t[1].join();
    
    //t[1]=thread(readPrueba,ConnectFD);  
    //t[1].join();
    
    
    shutdown(ConnectFD, SHUT_RDWR);
    //close(ConnectFD);
     
  }
  
  close(SocketFD);
  return 0;
}
