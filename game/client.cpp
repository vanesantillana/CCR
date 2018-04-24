#include <ncurses.h>        //g++ -o g.exe g.cpp  -lncurses  -std=c++1
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
  }
}


void my_write(int SocketFD){
  WIN win;
  //    WIN win2;


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
   
  while(1){
    int ch;
    while((ch = getch()) != KEY_F(1)){
      //cout<<"tecla"<<endl;
      string wp_P=write_protocol_R(to_string(ch));
      my_writeSimple(SocketFD,wp_P);
      switch(ch){
      case KEY_LEFT:
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
      //cout<<win.startx<<endl;
      //cout<<win.starty<<endl;
    }
    
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
  //Res = inet_pton(AF_INET, "192.168.197.57", &stSockAddr.sin_addr);
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
