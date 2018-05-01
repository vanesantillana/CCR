#include <ncurses.h>
#include <iostream>       // std::cout
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
using namespace std;
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
void print_win_params(WIN *p_win);
void create_box(WIN *win, bool flag);
void bullet(WIN *p_win)
{
}
int main(int argc, char *argv[])
{       WIN win;
  WIN win2;
  int ch;
  initscr();                      /* Start curses mode            */
  start_color();                  /* Start the color functionality */
  cbreak();                       /* Line buffering disabled, Pass on
				   * everty thing to me           */
  keypad(stdscr, TRUE);           /* I need that nifty F1         */
  noecho();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  /* Initialize the window parameters */
  init_win_params(&win);
  print_win_params(&win);
  /* Initialize the window parameters */
  init_win_params(&win2);
  print_win_params(&win2);
  attron(COLOR_PAIR(1));
  printw("Press F1 to exit");
  refresh();
  attroff(COLOR_PAIR(1));
  create_box(&win, TRUE);
  --win2.startx;
  --win2.starty;
  create_box(&win2, TRUE);
  while((ch = getch()) != KEY_F(1))
    {
      create_box(&win2, TRUE);
      switch(ch)
	{       case KEY_LEFT:
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
    }
  endwin();                       /* End curses mode                */
  return 0;
}
void init_win_params(WIN *p_win)
{
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
void print_win_params(WIN *p_win)
{
}
void create_box(WIN *p_win, bool flag)
{       int i, j;
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

