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
thread t[1000];
void my_writeR(int SocketFD,string msg){
  msg=write_protocol_R(msg); //Agrego mi tamanio a la cabeza del mensaje
  write(SocketFD,msg.c_str(),msg.size());
}


string printMap(){
  StringMap::iterator pos;
  string lista="\n+++++++++++++++++++++++++++\nLISTA DE USUARIOS\n";
  StringMap::iterator it = Users.begin();
  int cont=1;
  while(it != Users.end()){
    lista=lista+to_string(cont)+"- ";
    lista=lista+it->first+"\n";
    it++;
    cont++;
  }
  lista=lista+"+++++++++++++++++++++++++++\n";
  return lista;
}


void nuevoUsuario( int ConnectFD){

string menu="----------------------------\nMENU\n 1. [Action P] Print list of user on the chat\n 2. [Action L] Login to the chat\n 3. [Action C] Send a msg to an user on the chat\n 4. [Action E]End chat or logout from chat \n---------------------------\n";
 string mensaje;
 
 while(true){ 
   char * buffer=new char[size];
   my_writeR(ConnectFD,menu);
   my_read2(ConnectFD,buffer);
   
   if (buffer[4] == 'P'){ 
     string nv= printMap();
     my_writeR(ConnectFD,nv);
   }
   else if (buffer[4] == 'L'){
     string nick=read_protocol_L(buffer);
     Users[nick]=ConnectFD;
     //mensaje=menu;
   }  
   else if (buffer[4] == 'C'){
     string nick,msj;
     msj=read_protocol_C(buffer,nick);
     if(Users[nick]){
       msj=nick+" dice: "+msj;
       my_writeR(Users[nick],msj);
     }
     else
       my_writeR(ConnectFD,"Usuario No existe");
    
   }
   
   else if (buffer[4] == 'E'){
     //shutdown(ConnectFD, SHUT_RDWR);
     my_write2(ConnectFD,"E");
     close(ConnectFD);
     break;
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

  
  
  int cont=0;
  for(;;){
    int ConnectFD = accept(SocketFD, NULL, NULL);
    t[cont]=thread(nuevoUsuario,ConnectFD);
    cont++;
  }

  for(int h=0;h<cont;h++)
    t[h].join();
  
  //shutdown(ConnectFD, SHUT_RDWR);
  

    //close(ConnectFD);
     
  
  
  close(SocketFD);
  return 0;
}
