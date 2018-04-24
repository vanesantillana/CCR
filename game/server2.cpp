#include <ncurses.h>        //g++ -o g.exe g.cpp  -lncurses  -std=c++11  q -lpthread
#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

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
  //  string menu="\n----------------------------\nMENU\n 1. [Action P] Print list of user on the chat\n 2. [Action L] Login to the chat\n 3. [Action C] Send a msg to an user on the chat\n 4. [Action F] Send File \n 6. [Action E] End chat or logout from chat \n----------------------------";

  string mensaje=write_protocol_R("Bienvenido al juego\n-------------------");
  
  WIN win;
  //    WIN win2;
 

  int sizef; 
  char tipo=getTypeProtocol(ConnectFD,sizef);
  string nick=read_protocol_L(ConnectFD,sizef);
  Users[nick]=ConnectFD;

  int ch;

  initscr();                      /* Start curses mode            */
  start_color();                  /* Start the color functionality */
  cbreak();                       /* Line buffering disabled, Pass on*/
  keypad(stdscr, TRUE);           /* I need that nifty F1         */
  noecho();
     
  init_win_params(&win);
  //    print_win_params(&win);
  //    attron(COLOR_PAIR(1));
  //    attroff(COLOR_PAIR(1));
  create_box(&win, TRUE);
  my_writeSimple(ConnectFD,mensaje);
  while(true){ 
    //my_writeSimple(ConnectFD,menu);
    tipo=getTypeProtocol(ConnectFD,sizef);
    if (tipo == 'R'){
      string msj;
      string nv= read_protocol_RGame(ConnectFD,sizef);
      //nv=write_protocol_R(nick+" dice: "+nv);
      //sendAllMap(Users,nv);
      //para ver al inicio
      int ch=atoi(nv.c_str());
      switch(ch)
	{case KEY_LEFT:
	    create_box(&win, FALSE);
	    --win.startx;
	    create_box(&win, TRUE);
	    break;
	case KEY_RIGHT:
	  create_box(&win, FALSE);
	  ++win.startx;
	  create_box(&win, TRUE);
	  break;
	case KEY_UP:
	  create_box(&win, FALSE);
	  --win.starty;
	  create_box(&win, TRUE);
	  break;
	case KEY_DOWN:
	  create_box(&win, FALSE);
	  ++win.starty;
	  create_box(&win, TRUE);
	  break;
	}
      
    }
    /*   else if (tipo == 'E'){
    //shutdown(ConnectFD, SHUT_RDWR);
    my_writeSimple(ConnectFD,write_protocol_E());
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
  stSockAddr.sin_port = htons(345);
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
