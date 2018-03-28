  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
#include <bits/stdc++.h>
using  namespace std; 
  int main(void)
  {
    struct sockaddr_in stSockAddr;
    int Res,Res1;
    char buffer[256];
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
    stSockAddr.sin_port = htons(333);
    Res = inet_pton(AF_INET, "192.168.164.62", &stSockAddr.sin_addr);
 
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

    do{
      cin>>msg;
      write(SocketFD,msg.c_str(),msg.size());
      //n = read(SocketFD,buffer,1000);
      read(SocketFD,buffer,256);
      // if (n < 0) perror("ERROR writing to socket");
      
     printf("respuesta del servidor: [%s]\n",buffer);

    }while(msg!="exit");

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
