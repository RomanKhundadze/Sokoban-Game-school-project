#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>

#include "gameboard.hpp"

using namespace std;


Gameboard::Gameboard( int pos1, int pos2, char **argv){
	wnd = nullptr;
	initscr();

	wnd = loadGameboard(argv);
	cbreak();
	noecho();
	clear();

	refresh();


	keypad(wnd, true);
	curs_set(0);
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);		//
	init_pair(2, COLOR_CYAN, COLOR_BLACK);		//player
	init_pair(3, COLOR_MAGENTA, COLOR_BLACK);	//goal
	init_pair(4, COLOR_WHITE, COLOR_BLACK);		//Box
	wbkgd(wnd, COLOR_PAIR(1));

	attron(A_BOLD);
	//box(wnd, 0, 0);
	attroff(A_BOLD);

}

Gameboard::~Gameboard() {
}

WINDOW *Gameboard::getWindowHandle() {
	return wnd;
}

WINDOW *Gameboard::loadGameboard(char **argv){
	WINDOW *w;

	vector<string> vec;

	// open stream for reading..
	fstream f(argv[1], ios::in);
	if (f.good()) 
	{
		while (!f.eof()) 
		{
			string str;
			getline(f, str);
			if (string::npos == str.find(';')) 
			{
				vec.push_back(str);
				for(string xachse : vec)
				{
					int xcheck = xachse.length();
					if(xcheck > max.x)
					{
						max.x = xachse.length();
					}
				}
			}			
		}
		max.y = vec.size();

		w = newwin(max.y, max.x, 0 ,0);
		
		int adx = 0;
		int ady = 0;
		for(string xachse : vec)
		{
			for(char print : xachse)
			{
				if(print == '@'){
					player.y = ady;
					player.x = adx;
					mvwaddch(w, ady, adx, print|COLOR_PAIR(2));
					
				}
				else if(print == '.'){
					goals.push_back({adx,ady});
					mvwaddch(w, ady, adx, print|COLOR_PAIR(3));
					
				}
				else if(print == '$'){
					mvwaddch(w, ady, adx, print|COLOR_PAIR(4));
					
				}
				else{
					mvwaddch(w, ady, adx, print);
				}
				adx++;
				
			}
			adx = 0;
			ady++;
		}
		f.close();

		return w;
	} else {
		cout << "Map Existiert nicht!" << endl;
		endwin();
		return 0;
	}
}

Point Gameboard::getPlayer(){
	return player;
}
void Gameboard::movePlayer(Point move){
	char ch = mvwinch(wnd, move.y, move.x) & A_CHARTEXT;
	if('#' == ch || '$' == ch){

	}else{
		mvwaddch(wnd, move.y, move.x,'@'|COLOR_PAIR(2));
		mvwaddch(wnd, player.y, player.x, prestau);
		player = move;
		prestau = ' ';
	}
	if(ch == '.' ){
		prestau = '.';
	}
}
char Gameboard::getItem(Point pt){
	char ch = mvwinch(wnd, pt.y, pt.x) & A_CHARTEXT;
	return ch;
}
void Gameboard::moveItem(Point from, Point to){
	char ch = mvwinch(wnd, to.y, to.x) & A_CHARTEXT;
	
	if('#' != ch && '$' != ch){
		mvwaddch(wnd, to.y, to.x, '$'|COLOR_PAIR(4));
		mvwaddch(wnd, from.y, from.x, irestau);
	}

}
void Gameboard::displayGoals(){
	for (Point goal : goals){
		char ch = mvwinch(wnd, goal.y, goal.x) & A_CHARTEXT;
		if('@' != ch && '$' != ch){
			mvwaddch(wnd, goal.y, goal.x, '.'|COLOR_PAIR(3));
		}
	}
}

bool Gameboard::areGoalsComplete(){
	int zahler = 0;
	for (Point check : goals){
		char ch = mvwinch(wnd, check.y, check.x) & A_CHARTEXT;
		if(ch == '$'){
			zahler++;
		}
		else{
			return false;
		}
	}
	if (zahler == goals.size()){
		return true;
	}
}
