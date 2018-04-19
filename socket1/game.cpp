#include <ncurses.h>        //g++ -o g.exe g.cpp  -lncurses  -std=c++11  q -lpthread
#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[])
{
  WIN win;
  //    WIN win2;
  jugar(win);
  return 0;
}

