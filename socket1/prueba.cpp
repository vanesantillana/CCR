#include<bits/stdc++.h>
using namespace std;

typedef map<string,int> StringMap;
StringMap Users;

string printMap(){
  string nash="nashvent";
  Users[nash]=1;
  Users["masj"]=2;
  StringMap::iterator pos;
  string lista="";
  StringMap::iterator it = Users.begin();
  while(it != Users.end()){
      lista=lista+it->first+" ";
      it++;
    }
  
  return lista;
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

int main(){
  ifstream infile ("hack.jpg",ifstream::binary);
  
  infile.seekg (0,infile.end);
  long size = infile.tellg();
  infile.seekg (0);

  char* buffer = new char[size];

  infile.read (buffer,size);
  infile.close();


  
  char* buffer2 = new char[size];
  
  for(int x=0;x<size;x++)
    buffer2[x]=buffer[x];
  
  ofstream outfile ("holaaaa.jpg",ofstream::binary);
  outfile.write (buffer2,size);
  outfile.close();
  return 0;
}
