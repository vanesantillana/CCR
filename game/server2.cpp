//g++ server2.cpp -o serve -std=c++11 -pthread -lncurses

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

  //string mensaje=write_protocol_R("Bienvenido al juego\n-------------------");
  
  WIN win;
  //    WIN win2;
  int sizef; 
  char tipo=getTypeProtocol(ConnectFD,sizef);
  string nick=read_protocol_L(ConnectFD,sizef);
  Users[nick]=ConnectFD;
  string nick2=write_protocol_L(nick);
  sendAllMap(Users,nick2);
  int ch;

  initscr();                      /* Start curses mode            */
  start_color();                  /* Start the color functionality */
  cbreak();                       /* Line buffering disabled, Pass on*/
  keypad(stdscr, TRUE);           /* I need that nifty F1         */
  noecho();
  init_win_params(&win);
  create_box(&win, TRUE);
  //my_writeSimple(ConnectFD,mensaje);

  while(true){ 
    //my_writeSimple(ConnectFD,menu);
    tipo=getTypeProtocol(ConnectFD,sizef);
    if (tipo == 'R'){
      string msj;
      string nv= read_protocol_RGame(ConnectFD,sizef);
      int ch=atoi(nv.c_str());
      movimientoPersonaje(ch,&win);
      string msgForAll=write_protocol_C(nick,nv);
      //nv=write_protocol_R(nick+" dice: "+nv);
      sendAllMap(Users,msgForAll);
    }
    else if(tipo == 'X'){
      int x, y;
      string nickNuevo;
      read_protocol_X(ConnectFD,sizef,nickNuevo,x,y);
      movimientoPersonaje2(x,y,&win);
      string nuevoMensaje=write_protocol_X(nick,x,y);                                                 
      sendAllMap(Users,nuevoMensaje);
    }
    else if (tipo == 'E'){
     //shutdown(ConnectFD, SHUT_RDWR);
     my_writeSimple(ConnectFD,write_protocol_E());
     string key=findInMap(Users,ConnectFD);
     Users.erase(key);
     create_box(&win,FALSE);
    //Indicar a los demas para q borren mi personaje    
    string game_over=write_protocol_O(nick);
      sendAllMap(Users,game_over);
     close(ConnectFD);
     break;
   }
  }
}


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
  stSockAddr.sin_port = htons(puerto);
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
