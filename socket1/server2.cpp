  /* Server code in C */
 
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
using  namespace std;

void my_write(int SocketFD,string msg){
  int tam=msg.size();
  msg= to_string(tam)+msg;
  //cout<<msg<<endl;
  //int my_size=atoi(msg.c_str());             //mi tamanio al inicio de la cadena
  //cout<<my_size<<endl;
  write(SocketFD,msg.c_str(),msg.size());

} 
  
  int main(void)
  {
    struct sockaddr_in stSockAddr;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    char buffer[1000];
    int n;
    string msg="";
 
    if(-1 == SocketFD)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(122);
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
 
    for(;;)
    {
      int ConnectFD = accept(SocketFD, NULL, NULL);

      if(0 > ConnectFD)
      {
        perror("error accept failed");
        close(SocketFD);
        exit(EXIT_FAILURE);
      }

     
      string msg2;
     /* perform read write operations ... */
      //write(ConnectFD,"Bienvenido al chat",18);
      do{
        /*
        cout<<"entre"<<endl;
        read(ConnectFD,buffer,1000);
        cin>>msg;
        cout<<msg<<endl;
        write(ConnectFD,msg.c_str(),msg.size());
*/      
        cin>>msg2;
        my_write(ConnectFD,msg2.c_str());
        //write(ConnectFD,msg2.c_str(),msg2.size());

        //
        bzero(buffer,1000);
        //cout<<"bucle"<<endl;
       n = read(ConnectFD,buffer,1000);
       if (n < 0) perror("ERROR reading from socket");
       //cout<<"cliente dice: "<<buffer<<endl;
       printf("cliente dice: [%s]\n",buffer);
       //n = write(ConnectFD,"server",6);
       //if (n < 0) perror("ERROR writing to socket");
       //read(SocketFD,buffer,256);

      }while(msg!="exit");

        cout<<"fin"<<endl;

      shutdown(ConnectFD, SHUT_RDWR);
 
      //close(ConnectFD);
    }
 
    close(SocketFD);
    return 0;
  }
