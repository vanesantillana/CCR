#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <thread>
#include "f.h"
using  namespace std; 

void my_read(int SocketFD){  
  while(1){
    int sizef;
    char tipo=getTypeProtocol(SocketFD,sizef);
    if(tipo=='R'){
      read_protocol_R(SocketFD,sizef);
    }
    else if(tipo=='E'){
      cout<<"FIN"<<endl;
      break;
    }
    
  }
}


void my_write(int SocketFD){
  while(1){
    string msg;
    cin>>msg;
    /*if(msg=="6"){
      string wp_P=write_protocol_E();
      my_writeSimple(SocketFD,wp_P);
      //shutdown(SocketFD, SHUT_RDWR);
      break;
    }
    else{*/
    string wp_P=write_protocol_R(msg);
    my_writeSimple(SocketFD,wp_P);
    
      //}
    
  }
}




int main(void)
{
  struct sockaddr_in stSockAddr;
  int Res,Res1;
  //char buffer[256];
  int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  int n;
  string msg;
 
  if (-1 == SocketFD)
    {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }
 
  memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = htons(345);
  Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);
 
  if (0 > Res)
    {
      perror("error: first parameter is not a valid address family");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
  else if (0 == Res)
    {
      perror("char string (second parameter does not contain valid ipaddress");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 
  if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("connect failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

  
  cout<<"usuario: ";
  string nombreUsuario;
  cin>>nombreUsuario;
  string wp_P=write_protocol_L(nombreUsuario);
  my_writeSimple(SocketFD,wp_P);

  
  thread t[2];
  t[0]=thread(my_read,SocketFD);
  t[1]=thread(my_write,SocketFD);
  t[1].join();
  t[0].join();
    
  /*
    shutdown(SocketFD, SHUT_RDWR);
  */
  close(SocketFD);
  return 0;
}
