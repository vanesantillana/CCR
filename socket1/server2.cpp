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
typedef map<string,int> StringMap;
StringMap Users;
string printMap(){
  StringMap::iterator pos;
  string lista="LISTA DE USUARIOS\n---------------------------\n";
  StringMap::iterator it = Users.begin();
  while(it != Users.end()){
    lista=lista+it->first+"\n";
    it++;
  }
  
  return lista;
}


void nuevoUsuario( int ConnectFD){

string menu="\nMENU\n 1-[Action P] Print list of user on the chat\n 2. [Action L] Login to the chat\n 3. [Action C] Send a msg to an user on the chat\n 4. [Action E]End chat or logout from chat \n";
 string mensaje=menu;
 
 while(true){ 
   char * buffer=new char[size];
   my_write2(ConnectFD,mensaje);
   my_read2(ConnectFD,buffer);
   cout<<"bufer"<<buffer<<endl;
   
   if (buffer[4] == 'P'){ 
     string nv= printMap();
     mensaje=nv;
   }
   else if (buffer[4] == 'L'){
     string nick=read_protocol_L(buffer);
     Users[nick]=ConnectFD;
     mensaje=menu;
   }  
   else if (buffer[4] == 'C'){
     string nick,msj;
     msj=read_protocol_C(buffer,nick);
     if(Users[nick]){
       my_write2(Users[nick],msj);
     }
     else
       my_write2(ConnectFD,"No existe");
     
     //cout<<"nick:"<<nick<<endl;
     //cout<<"msj:"<<msj<<endl;
   }
   
   else if (buffer[4] == 'E'){
     cout << "Adios" << endl;
     }
   
 }
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

  
  thread t[1000];
  int cont=0;
  for(;;){
    int ConnectFD = accept(SocketFD, NULL, NULL);
    t[cont]=thread(nuevoUsuario,ConnectFD);
    cont++;
  }
  //shutdown(ConnectFD, SHUT_RDWR);
  
  
  //t1[0]=thread(nuevoUsuario,SocketFD);

    //close(ConnectFD);
     
  
  
  close(SocketFD);
  return 0;
}
