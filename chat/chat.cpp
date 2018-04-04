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

char buffer[256];
int nbytes=4;

void my_read(int SocketFD){  
  while(1){    
    int n=read(SocketFD,buffer,nbytes);
    if (n < 0) perror("error reading size");
    int my_size = atoi(buffer);

    n = read(SocketFD, buffer, my_size);
    if (n < 0) perror("error reading message");
    
    cout<<"Servidor dice: [";
    for (int i = 0; i < my_size; i++)
        cout << buffer[i];
    cout<<"]"<<endl;
  }
}

string complete_zero(int size,int nbytes){
  string str_size=to_string(size);
  int tam=str_size.size();
  for(int i=tam;i<nbytes;++i){
    str_size="0"+str_size;
  }
  return str_size;
}

void my_write(int SocketFD){
  while(1){
    string msg;
    cin>>msg;
    msg=complete_zero(msg.size(),nbytes) + msg; //Agrego mi tamanio a la cabeza del mensaje
    write(SocketFD,msg.c_str(),msg.size());
  }
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
  cout << "Action P" << endl;
  string action = complete_zero(1, nbytes) +"P";
  return action;
}

//Action L: Login to the chat 
string write_protocol_L(string nickname){
  cout << "Action L" << endl;
  string action = complete_zero(nickname.size(),nbytes)+"L"+nickname;
  return action;
}
void read_protocol_L(char * n_buffer){
  int nickname_size=atoi_first_block(n_buffer);
  string temp;
  int cadena_size= 5 + nickname_size;
  for (int i = 5; i < cadena_size; i++)
    temp =temp + n_buffer[i];
  string nickname = temp;
  cout<<"Nickname: "<<nickname<<endl;
}


//Action C: send a msg to a user on the chat
string write_protocol_C(string nickname, string msg){
  cout << "Action C" << endl;
  string action = complete_zero(msg.size(),nbytes) +"C" + 
                  complete_zero(nickname.size(),2) +nickname + msg;
  return action;
}
void read_protocol_C(char * n_buffer){
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
  cout<<"Nickname: "<<nickname<<endl;

  temp="";
  int size_temp=cadena_size;
  cadena_size= cadena_size + msg_size;
  for (int i = size_temp; i < cadena_size; i++)
    temp += buffer[i];
  string msg = temp;
  cout<<"Mensaje: "<<msg<<endl;

}

//Action R: Send a msg to a client
string write_protocol_R(string msg){
  cout << "Action R" << endl;
  string action = complete_zero(msg.size(),nbytes)+"R"+msg;
  return action;
}

void read_protocol_R(char * n_buffer){
  int msg_size=atoi_first_block(n_buffer);
  string temp;
  int cadena_size= 5 + msg_size;
  for (int i = 5; i < cadena_size; i++)
    temp =temp + n_buffer[i];
  string msg = temp;
  cout<<"Mensaje: "<<msg<<endl;
}

// Action E: End chat or logout from chat 
string write_protocol_E(){
  cout << "Action E" << endl;
  string action = complete_zero(1, nbytes) +"E";
  return action;
}
//////////////Cliente///////////////
void cliente()
{
  struct sockaddr_in stSockAddr;
  int Res,Res1;
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
  stSockAddr.sin_port = htons(320);
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

  thread t[2];
  t[0]=thread(my_read,SocketFD);
  t[1]=thread(my_write,SocketFD);
  t[1].join();
  t[0].join();
  close(SocketFD);
}

//////////////Server////////////////
void servidor(){
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
  stSockAddr.sin_port = htons(320);
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
  

  thread t[2];
  for(;;){
    int ConnectFD = accept(SocketFD, NULL, NULL);
    
    if(0 > ConnectFD){
      perror("error accept failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    
    t[1]=thread(my_read,ConnectFD);
    t[0]=thread(my_write,ConnectFD);
    t[0].join();
    t[1].join();
    
    shutdown(ConnectFD, SHUT_RDWR);
    //close(ConnectFD);
     
  }
  close(SocketFD);
}

///////////////MENU///////////////////

int main()
{
    string client,server;
    string b;
    while(1){
      int a;

      cout<<"##########  MENU CHAT  ##########"<<endl;
      cout<< "  1. Server" << endl;
      cout<< "  2. Client" << endl<<endl;
      cout<< "  3. [Action P] Print list of user on the chat" << endl;
      cout<< "  4. [Action L] Login to the chat" << endl;
      cout<< "  5. [Action C] Send a msg to an user on the chat" << endl;
      cout<< "  6. [Action E]End chat or logout from chat" << endl;
      cout<< "#################################" << endl;
      cout<< "Opcion: ";
      cin >> a;
      switch(a){
        case 1:
          servidor();
          break;
        case 2:
          cliente();
          break;
        case 3:{
          client= write_protocol_P();
          cout<<"Enviaste: "<<client<<endl;
          strcpy(buffer, client.c_str()); 
          //cin>>b;
          break;
        }
        case 4:{
          string nick_name="";
          cout << "Nickname: ";
          cin >> nick_name;
          client = write_protocol_L(nick_name);
          cout<<"Enviaste: "<<client<< endl;
          strcpy(buffer, client.c_str());
          break;
        }
        case 5:{
          string destino="";
          string msge="";
          cout << "Mensaje para: ";
          cin >> destino;
          cout << "Mensaje: ";
          cin >> msge;
          client = write_protocol_C(destino,msge);
          cout<<"Enviaste: "<<client<< endl;
          strcpy(buffer, client.c_str()); 
          break;         
        }
        case 6:{
          client= write_protocol_E();
          cout<<"Enviaste: "<<client<<endl;
          strcpy(buffer, client.c_str());
          break;
        }
        default :
         cout << "Opcion invalida" << endl;
         break;
      }

      //El servidor responde al protocolo p con la lista de usuarios
      if (buffer[4] == 'P'){
        string total_users = "Ana,Bob";
        cout <<"Usuarios ONLINE: "<<total_users<< endl;
        server = write_protocol_R(total_users);
        cout <<"Enviaste: " << server << endl;
        //cin>>b;

      }
      else if (buffer[4] == 'L')
        read_protocol_L(buffer);

      else if (buffer[4] == 'C')
        read_protocol_C(buffer);

      else if (buffer[4] == 'E')
        cout << "Adios" << endl;

    }
    return 0;
}
