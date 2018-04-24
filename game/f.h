#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <thread>
#include <ncurses.h>  

using namespace std;
typedef map<string,int> StringMap;

int size=10240;
int nbytes=4;

string charToString(char*buffer,int sizef){
  string resp;
  for (int i = 0; i < sizef; i++){
    resp+=buffer[i];
  }
  return resp;
}


string findInMap(StringMap mapa,int val){
  StringMap::iterator it = mapa.begin();
  string resp="";
  while(it != mapa.end()){
    if(it->second==val){
      resp=it->first;
      break;
    }
    it++;
    
  }
  return resp;
}


int atoi_first_block(char *n_buffer){
  string size;
  for (int i = 0; i < nbytes; i++)
    size = size + n_buffer[i];
  return atoi(size.c_str());
}

string complete_zero(int size,int nbytes){
  string str_size=to_string(size);
  int tam=str_size.size();
  for(int i=tam;i<nbytes;++i){
    str_size="0"+str_size;
  }
  return str_size;
}


int getSize(char*buffer,int i,int f){
  string msg="";
  for(;i<f;i++){
    msg+=buffer[i];
  }
  return atoi(msg.c_str());
}

char getTypeProtocol(int SocketFD,int &size){
  char buffer[5];
  int n=read(SocketFD,buffer,4);
  //cout<<"buffer recibido: "<<buffer<<endl;
  if (n < 0) perror("error reading size");
  size = atoi_first_block(buffer);
  n=read(SocketFD,buffer,1);
  //cout<<"Protocolo:"<<buffer[0]<<endl;
  return buffer[0];
}

void my_read2(int SocketFD,char* &buffer){
  int n=read(SocketFD,buffer,nbytes);
  if (n < 0) perror("error reading size");
  int my_size = atoi(buffer);
  n = read(SocketFD, buffer, my_size);
  if (n < 0) perror("error reading message");  
  cout<<"Msg recibido\n------------\n";
  for (int i = 0; i < my_size; i++)
    cout << buffer[i];
  cout<<endl;
}

void my_write2(int SocketFD,string msg){
  msg=complete_zero(msg.size(),nbytes) + msg; 
  write(SocketFD,msg.c_str(),msg.size());
}

void my_writeSimple(int SocketFD,string msg){
  
  write(SocketFD,msg.c_str(),msg.size());
}


/////////////PROTOCOLOS/////////////


//Action R: Send a msg to a client
string write_protocol_R(string msg){
  string action = complete_zero(msg.size(),nbytes)+"R"+msg;
  return action;
}
 
void read_protocol_R(int SocketFD,int sizef){
  //cout<<"size:"<<sizef<<endl;
  char buffer[sizef+1];
  int n = read(SocketFD, buffer, sizef);
  if (n < 0) perror("error reading message");
  cout<<charToString(buffer,sizef)<<endl;
  
}



//Action P: print list of user on the chat
string write_protocol_P(){
  string action = complete_zero(1, nbytes) +"P";
  return action;
}

//Action L: Login to the chat 
string write_protocol_L(string nickname){
  string action = complete_zero(nickname.size(),nbytes)+"L"+nickname;
  return action;
}

string read_protocol_L(int SocketFD,int sizef){
  char buffer[sizef];
  int n = read(SocketFD, buffer, sizef);
  string nick=charToString(buffer,sizef);
  //cout<<"Nickname: "<<nickname<<endl;
  return nick;
}


//Action C: send a msg to a user on the chat
string write_protocol_C(string nickname, string msg){
  //cout << "Action C" << endl;
  string action = complete_zero(msg.size(),nbytes) +"C" + 
                  complete_zero(nickname.size(),2) +nickname + msg;
  return action;
}

string read_protocol_C(int SocketFD,int sizef,string &receptor){
  char buffer2[3];
  int n = read(SocketFD, buffer2,2);
  int sizeNick=atoi(buffer2);
  char bufferNick[sizeNick+1];
  n=read(SocketFD,bufferNick,sizeNick);
  receptor=bufferNick;
  char bufferMsg[sizef+1];
  n=read(SocketFD,bufferMsg,sizef);
  return string(bufferMsg);
}



// Action E: End chat or logout from chat 
string write_protocol_E(){
  //cout << "Action E" << endl;
  string action = complete_zero(1, nbytes) +"E";
  return action;
}




string write_protocol_F(string nick,string filename){
  string msg=complete_zero(filename.size(),nbytes) +"F" + 
    complete_zero(nick.size(),2) +nick+ filename;

  //Leer archivo
  //ifstream infile ("hack.jpg",ifstream::binary);
  cout<<"file name"<<filename<<endl;
  ifstream infile (filename,ifstream::binary);
  infile.seekg (0,infile.end);
  long size = infile.tellg();
  infile.seekg (0);
  msg=msg+complete_zero(size,4);
  char* buffer = new char[size];
  infile.read (buffer,size);
  infile.close();

  for(int x=0;x<size;x++){
    msg=msg+buffer[x];
  }

  //cout<<"mesaje original:"<<msg<<endl;
  //cout<<"size enviado: "<<size<<endl;
  //cout<<"esto estoy enviando:"<<msg<<endl;
  return msg;
}



string read_protocol_F(int SocketFD,int sizef){
  char buffer2[2];
  int n = read(SocketFD, buffer2,2);
  int sizeNick=atoi(buffer2);
  char bufferNick[sizeNick+1];
  n=read(SocketFD,bufferNick,sizeNick);
  //  receptor=bufferNick;
  char bufferMsg[sizef];
  n=read(SocketFD,bufferMsg,sizef);
  
  string nombreArchivo=charToString(bufferMsg,sizef);
  
  char buffer3[4];
  n=read(SocketFD,buffer3,4);
  int sizeFile=atoi(buffer3);
  char bufferFile[sizeFile];
  n=read(SocketFD,bufferFile,sizeFile);
  cout<<"Archivo "<<nombreArchivo<<" de "<<bufferNick<<endl;
  ofstream outfile (nombreArchivo,ofstream::binary);
  outfile.write (bufferFile,sizeFile);
  outfile.close();
  
  //return msg;
}


string read_protocol_D(int SocketFD,int sizef,string &receptor,string newNick){
  char buffer2[3];
  int n = read(SocketFD, buffer2,2);
  int sizeNick=atoi(buffer2);

  
  char bufferNick[sizeNick];
  n=read(SocketFD,bufferNick,sizeNick);
  receptor=bufferNick;
  char bufferMsg[sizef+1];
  n=read(SocketFD,bufferMsg,sizef);
  string nombreArchivo=charToString(bufferMsg,sizef);
  
  
  char buffer3[5];
  n=read(SocketFD,buffer3,4);
  int sizeFile=atoi(buffer3);
  char bufferFile[sizeFile+1];
  n=read(SocketFD,bufferFile,sizeFile);
  string archivoReal=charToString(bufferFile,sizeFile);
  string msgFinal=complete_zero(sizef,4)+'F'+
    complete_zero(newNick.size(),2)+newNick
    +nombreArchivo+complete_zero(sizeFile,4)+archivoReal;
  return msgFinal;
}

string read_protocol_RGame(int SocketFD,int sizef){
  char buffer[sizef+1];
  int n = read(SocketFD, buffer, sizef);
  if (n < 0) perror("error reading message");
  return charToString(buffer,sizef);
  
}


void sendAllMap(StringMap mapa,string msj){
  StringMap::iterator it = mapa.begin();
  while(it != mapa.end()){ 
    my_writeSimple(it->second,msj);
    it++;
  }
}
//////////////////////////////////////////
//JUEGOOO/////////////////////////////////
//////////////////////////////////////////

typedef struct _win_border_struct {
  chtype  ls, rs, ts, bs,
    tl, tr, bl, br;
}WIN_BORDER;
typedef struct _WIN_struct {
  int startx, starty;
  int height, width;
  WIN_BORDER border;
}WIN;


typedef map<string,WIN> WinMap;
void init_win_params(WIN *p_win);
void print_win_params(WIN *p_win){}
void create_box(WIN *win, bool flag);
//void bullet(WIN *p_win){}





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

void movimientoPersonaje(int ch,WIN *win){
  switch(ch){

    case KEY_LEFT:
	      create_box(win, FALSE);
	      --win->startx;
	      create_box(win, TRUE);
	      break;
      case KEY_RIGHT:
        create_box(win, FALSE);
        ++win->startx;
        create_box(win, TRUE);
        break;
      case KEY_UP:
        create_box(win, FALSE);
        --win->starty;
        create_box(win, TRUE);
        break;
      case KEY_DOWN:
        create_box(win, FALSE);
        ++win->starty;
        create_box(win, TRUE);
        break;
    /*  case KEY_LEFT:
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
        break;*/
  }
}