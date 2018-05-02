//g++ client.cpp -o cli -std=c++11 -pthread -lncurses

#include "f.h"
using  namespace std; 

WinMap U;
string yo;
WIN *vidas;

void my_read(int SocketFD){  
  while(1){
    int sizef;
    char tipo=getTypeProtocol(SocketFD,sizef);
    if(tipo=='R'){
      read_protocol_R(SocketFD,sizef);
    }
    if(tipo=='L'){
      string nick=read_protocol_L(SocketFD,sizef);
      if(yo!=nick){
        U[nick]=new WIN;
        initscr();                      /* Start curses mode            */
        start_color();                  /* Start the color functionality */
        cbreak();                       /* Line buffering disabled, Pass on*/
        keypad(stdscr, TRUE);           /* I need that nifty F1         */
        noecho();
        init_win_params(U[nick]);
        create_box(U[nick], TRUE);
        //create_legend(vidas);
      }
      
    }
    
    if(tipo=='C'){
      string nick;
      string msg=read_protocol_C(SocketFD,sizef,nick);
      if(U[nick]==nullptr){
        U[nick]=new WIN;
        initscr();                      /* Start curses mode            */
        start_color();                  /* Start the color functionality */
        cbreak();                       /* Line buffering disabled, Pass on*/
        keypad(stdscr, TRUE);           /* I need that nifty F1         */
        noecho();
        init_win_params(U[nick]);
        create_box(U[nick], TRUE);
      }
      int ch=atoi(msg.c_str());
      movimientoPersonaje(ch,U[nick]);
      //cout<<"ch: "<<ch<<endl;
      
    }
    if(tipo=='X'){
      string nick;
      int x,y;
      read_protocol_X(SocketFD,sizef,nick,x,y);
      if(U[nick]==nullptr){
        U[nick]=new WIN;
        initscr();                      /* Start curses mode            */
        start_color();                  /* Start the color functionality */
        cbreak();                       /* Line buffering disabled, Pass on*/
        keypad(stdscr, TRUE);           /* I need that nifty F1         */
        noecho();
        init_win_params(U[nick]);
        U[nick]->startx=x;
        U[nick]->starty=y;
        create_box(U[nick], TRUE);
      }
      else{
        movimientoPersonaje2(x,y,U[nick]);  
      }
      
        //cout<<"ch: "<<ch<<endl;      
    }
    else if(tipo=='O'){//Un jugador perdio
      string perdedor=read_protocol_RGame(SocketFD,sizef);
      create_box(U[perdedor],FALSE);
    }
    else if(tipo=='E'){
      cout<<"GAME OVER"<<endl;
      break;
    }

  }
}


void my_write(int SocketFD){
  U[yo]=new WIN;
  //WIN win;
  int ch;
  initscr();                      /* Start curses mode            */
  start_color();                  /* Start the color functionality */
  cbreak();                       /* Line buffering disabled, Pass on*/
  keypad(stdscr, TRUE);           /* I need that nifty F1         */
  noecho();     
  init_win_params(U[yo]);
  //    print_win_params(&win);
  //    attron(COLOR_PAIR(1));
  //    attroff(COLOR_PAIR(1));
  create_box(U[yo], TRUE);
   
  while(1){
    int ch;
    while((ch = getch()) != KEY_F(1)){
      //string wp_P=write_protocol_R(to_string(ch));
      if(ch==' '){
        ch=1111;
        string wp_P=write_protocol_R(to_string(ch));
        my_writeSimple(SocketFD,wp_P);
      }
      else if(ch==27){
        string wp_P=write_protocol_E();
        my_writeSimple(SocketFD,wp_P);
        //shutdown(SocketFD, SHUT_RDWR);
         break;
      }
      else{
        int x=U[yo]->startx;
      int y=U[yo]->starty;
      movimientoPersonaje3(ch,x,y);
      string wp_X=write_protocol_X(yo,x,y);
      my_writeSimple(SocketFD,wp_X);
      //cout<<win.startx<<endl;
      //cout<<win.starty<<endl;
      
      }
      
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
  stSockAddr.sin_port = htons(puerto);
  //Res = inet_pton(AF_INET, "192.168.197.95", &stSockAddr.sin_addr);
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
  yo=nombreUsuario;
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
