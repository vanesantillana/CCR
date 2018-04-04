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
using  namespace std; 
char buffer[24];

void my_read(int SocketFD){  
  while(1){    
    bzero(buffer,24);
    int n=read(SocketFD,buffer,24);
    printf("servidor dice: [%s]\n",buffer);
  }
}
void my_write(int SocketFD){
  string msg;
  cin>>msg;
  int tam=msg.size();
  msg=to_string(tam)+msg;
  //cout<<msg<<endl;
  //int my_size=atoi(msg.c_str());             //mi tamanio al inicio de la cadena
  //cout<<my_size<<endl;
  write(SocketFD,msg.c_str(),msg.size());

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
    stSockAddr.sin_port = htons(336);
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

    thread t[2];
    t[0]=thread(my_read,SocketFD);
    do{
      t[1]=thread(my_write,SocketFD);
      
      t[1].join();
    }while(msg!="exit");
    t[0].join();
    
    //t[0].join();

    /*n = write(SocketFD,"exit",4);
    bzero(buffer,256);
     n = read(SocketFD,buffer,255);
     if (n < 0) perror("ERROR reading from socket");
     //printf("Here is the message: [%s]\n",buffer);
     n = write(SocketFD,"I got your message",18);
    shutdown(SocketFD, SHUT_RDWR);
 */
    close(SocketFD);
    return 0;
  }
