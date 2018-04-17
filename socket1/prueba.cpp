#include<bits/stdc++.h>
using namespace std;

typedef map<string,int> StringMap;
StringMap Users;

string printMap(){
  //string nash="nashvent";
  //Users[nash]=1;
  //Users["masj"]=2;
  StringMap::iterator pos;
  string lista="";
  StringMap::iterator it = Users.begin();
  while(it != Users.end()){
      lista=lista+it->first+" ";
      it++;
    }
  
  return lista;
}


int main(){
  
  cout<<printMap()<<endl;
  return 0;
}
