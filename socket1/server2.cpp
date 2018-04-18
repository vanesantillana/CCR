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
StringMap Users;
thread t[1000];


string printMap(){
  StringMap::iterator pos;
  string lista="+++++++++++++++++++++++++++\nLISTA DE USUARIOS\n";
  StringMap::iterator it = Users.begin();
  int cont=1;
  while(it != Users.end()){
    lista=lista+to_string(cont)+"- ";
    lista=lista+it->first+"\n";
    it++;
    cont++;
  }
  lista=lista+"+++++++++++++++++++++++++++";
  return lista;
}


void nuevoUsuario( int ConnectFD){
  
  string menu="\n----------------------------\nMENU\n 1. [Action P] Print list of user on the chat\n 2. [Action L] Login to the chat\n 3. [Action C] Send a msg to an user on the chat\n 4. [Action F] Send File \n 6. [Action E] End chat or logout from chat \n----------------------------";
  string mensaje;
  menu=write_protocol_R(menu);
  
  while(true){ 
   my_writeSimple(ConnectFD,menu);
   int sizef;
   char tipo=getTypeProtocol(ConnectFD,sizef);
  
   if (tipo == 'P'){ 
     string nv= write_protocol_R(printMap());
     my_writeSimple(ConnectFD,nv);
   }
   else if (tipo == 'L'){ 
     string nick=read_protocol_L(ConnectFD,sizef);
     Users[nick]=ConnectFD;
     string nv= write_protocol_R(printMap());
     my_writeSimple(ConnectFD,nv);
     
   }
   else if (tipo == 'C'){
     string nick,msj;
     msj=read_protocol_C(ConnectFD,sizef,nick);

     if(Users[nick]){
       string newNick=findInMap(Users,ConnectFD);
       msj=newNick+" dice: "+msj;
       string nv= write_protocol_R(msj);
       my_writeSimple(Users[nick],nv);
     }
     else{
       string nv= write_protocol_R("No existe el usuario");
       my_writeSimple(ConnectFD,nv);
     }
      
   }
   else if (tipo == 'E'){
     //shutdown(ConnectFD, SHUT_RDWR);
     my_writeSimple(ConnectFD,write_protocol_E());
     string key=findInMap(Users,ConnectFD);
     Users.erase(key);
     close(ConnectFD);
     break;
   }
   else if (tipo == 'F'){
     string nick,msj;
     string newNick=findInMap(Users,ConnectFD);
     msj=read_protocol_D(ConnectFD,sizef,nick,newNick);
     //cout<<"msj:"<<msj<<endl;
     if(Users[nick]){
       //msj=newNick+" dice: "+msj;
       my_writeSimple(Users[nick],msj);
     }
     else{
       string nv= write_protocol_R("No existe el usuario");
       my_writeSimple(ConnectFD,nv);
     }
    

     //cout<<"lego aqui"<<endl;
     //msj=read_protocol_D(buffer,nick,newNick);

     // my_write2(Users[nick],msj);
   }
   
   /*
   else if (buffer[4] == 'F'){
     string nick,msj;
     //cout<<"lego aqui"<<endl;
     string newNick=findInMap(Users,ConnectFD);
     msj=read_protocol_D(buffer,nick,newNick);
     my_write2(Users[nick],msj);
  
   }
   
   
   else if (buffer[4] == 'E'){
     //shutdown(ConnectFD, SHUT_RDWR);
     my_write2(ConnectFD,"E");
     string key=findInMap(Users,ConnectFD);
     Users.erase(key);
     close(ConnectFD);
     break;
     }*/
   
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
  stSockAddr.sin_port = htons(344);
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
     
  
  
  //close(SocketFD);
  return 0;
}
