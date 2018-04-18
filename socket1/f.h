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
using namespace std;
typedef map<string,int> StringMap;


int size=10240;
int nbytes=4;

int getSize(char*buffer,int i,int f){
  string msg="";
  for(;i<f;i++){
    msg+=buffer[i];
  }
  return atoi(msg.c_str());
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


string complete_zero(int size,int nbytes){
  string str_size=to_string(size);
  int tam=str_size.size();
  for(int i=tam;i<nbytes;++i){
    str_size="0"+str_size;
  }
  return str_size;
}



void my_write2(int SocketFD,string msg){
  msg=complete_zero(msg.size(),nbytes) + msg; 
  write(SocketFD,msg.c_str(),msg.size());
}

void my_writeSimple(int SocketFD,string msg){
  write(SocketFD,msg.c_str(),msg.size());
}


/////////////PROTOCOLOS/////////////

int atoi_first_block(char *n_buffer){
  string size;
  for (int i = 0; i < nbytes; i++)
    size = size + n_buffer[i];
  return atoi(size.c_str());
}
//Action P: print list of user on the chat
string write_protocol_P(){
  //cout << "Action P" << endl;
  string action = complete_zero(1, nbytes) +"P";
  return action;
}

//Action L: Login to the chat 
string write_protocol_L(string nickname){
  //cout << "Action L" << endl;
  string action = complete_zero(nickname.size(),nbytes)+"L"+nickname;
  return action;
}
string read_protocol_L(char * n_buffer){
  int nickname_size=atoi_first_block(n_buffer);
  string temp;
  int cadena_size= 5 + nickname_size;
  for (int i = 5; i < cadena_size; i++)
    temp =temp + n_buffer[i];
  string nickname = temp;
  //cout<<"Nickname: "<<nickname<<endl;
  return nickname;
}


//Action C: send a msg to a user on the chat
string write_protocol_C(string nickname, string msg){
  //cout << "Action C" << endl;
  string action = complete_zero(msg.size(),nbytes) +"C" + 
                  complete_zero(nickname.size(),2) +nickname + msg;
  return action;
}

string read_protocol_C(char * n_buffer,string &nick){
  char buffer[size];
  int msg_size=atoi_first_block(n_buffer);
  string temp;
  int cadena_size= 5 + 2; //total Bytes
  for (int i = 5; i < cadena_size; i++)
    temp =temp + n_buffer[i];
  int nickname_size = atoi(temp.c_str());
  temp="";
  cadena_size=7+nickname_size;
  for (int i = 7; i < cadena_size; i++)
        temp += n_buffer[i];
  string nickname = temp;
  //cout<<"Nickname: "<<nickname<<endl;
  nick=nickname;
  temp="";
  int size_temp=cadena_size;
  cadena_size= cadena_size + msg_size;
  for (int i = size_temp; i < cadena_size; i++)
    temp += n_buffer[i];
  string msg = temp;
  //cout<<"mensaje: "<<msg<<endl;
  return msg;
}


//Action R: Send a msg to a client
string write_protocol_R(string msg){
  string action = complete_zero(msg.size(),nbytes)+"R"+msg;
  return action;
}
 
void read_protocol_R(char * n_buffer){
  int msg_size=atoi_first_block(n_buffer);
  //cout<<"buffer:"<<n_buffer<<endl;
  string temp;
  int cadena_size= 5 + msg_size;
  for (int i = 5; i < cadena_size; i++)
    temp =temp + n_buffer[i];
  string msg = temp;
  cout<<msg<<endl;
}

// Action E: End chat or logout from chat 
string write_protocol_E(){
  //cout << "Action E" << endl;
  string action = complete_zero(1, nbytes) +"E";
  return action;
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
  
  //cout<<"size enviado: "<<size<<endl;
  //cout<<"esto estoy enviando:"<<msg<<endl;
  return msg;
}



string read_protocol_F(char * n_buffer,string &nick){
  char buffer[size];
  int msg_size=atoi_first_block(n_buffer);
  string temp;
  int cadena_size= 5 + 2; //total Bytes
  for (int i = 5; i < cadena_size; i++)
    temp =temp + n_buffer[i];
  int nickname_size = atoi(temp.c_str());
  temp="";
  cadena_size=7+nickname_size;
  for (int i = 7; i < cadena_size; i++)
        temp += n_buffer[i];
  string nickname = temp;
  //cout<<"Nickname: "<<nickname<<endl;
  nick=nickname;
  temp="";
  int size_temp=cadena_size;
  cadena_size= cadena_size + msg_size;
  for (int i = size_temp; i < cadena_size; i++)
    temp += n_buffer[i];
  string msg = temp;

  string filesize;
  for (int i = cadena_size; i < cadena_size+4; i++)
    filesize += n_buffer[i];
  int fsize=atoi(filesize.c_str());
  int n_pos=cadena_size+4;

  char* buffer2 = new char[fsize];
  int cont=0;
  for(int i=n_pos;i<n_pos+fsize;i++){
    buffer2[cont]=n_buffer[i];
    cont++;
  }


  ofstream outfile ("holaaaa.png",ofstream::binary);
  outfile.write (buffer2,fsize);
  outfile.close();
  
  return msg;
}


string read_protocol_D(char * n_buffer,string &nick,string newNick){
  char buffer[size];
  int msg_size=atoi_first_block(n_buffer);
  string temp;
  int cadena_size= 5 + 2; //total Bytes
  for (int i = 5; i < cadena_size; i++)
    temp =temp + n_buffer[i];
  int nickname_size = atoi(temp.c_str());
  temp="";
  cadena_size=7+nickname_size;
  for (int i = 7; i < cadena_size; i++)
        temp += n_buffer[i];
  string nickname = temp;
  nick=nickname;
  temp="";
  int size_temp=cadena_size;
  cadena_size= cadena_size + msg_size;
  for (int i = size_temp; i < cadena_size; i++)
    temp += n_buffer[i];
  string msg = temp;

  string msgFinal;
  for(int x=0;x<5;x++)
    msgFinal+=n_buffer[x];

  msgFinal=msgFinal+complete_zero(newNick.size(),2)+newNick;
  
  string filesize;
  for (int i = cadena_size; i < cadena_size+4; i++)
    filesize += n_buffer[i];
  int fsize=atoi(filesize.c_str());
  int n_pos=cadena_size+4;

  for(int i=cadena_size;i<n_pos+fsize;i++){
    msgFinal+=n_buffer[i];  
  }

  cout<<"mesageFINAL:"<<msgFinal<<endl;

  return msgFinal;
}

