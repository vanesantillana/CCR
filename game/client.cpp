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

typedef struct _win_border_struct {
  chtype  ls, rs, ts, bs,
    tl, tr, bl, br;
}WIN_BORDER;
typedef struct _WIN_struct {
  int startx, starty;
  int height, width;
  WIN_BORDER border;
}WIN;
void init_win_params(WIN *p_win);
void print_win_params(WIN *p_win){}
void create_box(WIN *win, bool flag);
//void bullet(WIN *p_win){}
void jugar(WIN win){
  
}




void init_win_params(WIN *p_win){
  p_win->height = 8;
  p_win->width = 16;
  p_win->starty = (LINES - p_win->height)/2;
  p_win->startx = (COLS - p_win->width)/2;
  p_win->border.ls = '|';
  p_win->border.rs = '|';
  p_win->border.ts = '-';
  p_win->border.bs = '-';
  p_win->border.tl = '+';
  p_win->border.tr = '+';
  p_win->border.bl = '+';
  p_win->border.br = '+';
}
void create_box(WIN *p_win, bool flag){
  int i, j;
  int x, y, w, h;
  x = p_win->startx;
  y = p_win->starty;
  w = p_win->width;
  h = p_win->height;
  if(flag == TRUE)
    {
      move( y+0,x ); addstr("  ##        ##  ");
      move( y+1,x ); addstr("    #      #    ");
      move( y+2,x ); addstr("  ############  ");
      move( y+3,x ); addstr(" ###  ####  ### ");
      move( y+4,x ); addstr("################");
      move( y+5,x ); addstr("# ############ #");
      move( y+6,x ); addstr("# #          # #");
      move( y+7,x ); addstr("   ##      ##   ");
    }
  else
    for(j = y; j <= y + h; ++j)
      for(i = x; i <= x + w; ++i)
	mvaddch(j, i, ' ');
  refresh();
}


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
  jugar(win);

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
  Res = inet_pton(AF_INET, "192.168.197.57", &stSockAddr.sin_addr);
 
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
