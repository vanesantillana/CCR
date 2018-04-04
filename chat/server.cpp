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
int mbytes=4;

void my_read(int SocketFD){  
  while(1){    
    int n=read(SocketFD,buffer,4);
    if (n < 0) perror("error reading size");
    int my_size = atoi(buffer);

    n = read(SocketFD, buffer, my_size);
    if (n < 0) perror("error reading message");
    
    cout<<"cliente dice: [";
    for (int i = 0; i < my_size; i++)
        cout << buffer[i];
    cout<<"]"<<endl;
  }
}

string complete_zero(string size){
  int tam=size.size();
  for(int i=tam;i<4;++i){
    size="0"+size;
  }
  return size;
}
void my_write(int ConnectFD){
  while(1){    
    string msg;
    cin>>msg;
    string tam=to_string(msg.size());
    msg=complete_zero(tam) + msg;
    write(ConnectFD,msg.c_str(),msg.size());

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
  stSockAddr.sin_port = htons(320);
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
    
    t[1]=thread(my_read,ConnectFD);
    t[0]=thread(my_write,ConnectFD);
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
