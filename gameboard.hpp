#ifndef GAMEBOARD_HPP		// Sentinel (Wächter), verhindert das
#define GAMEBOARD_HPP		// Mehrfachladen der Headerdatei! 

#include <string>
#include <ncurses.h>
#include <vector>
#include <memory>
#include <fstream> 
using namespace std;

class Point {
	public:
		int x;
		int y;
};

class Gameboard{
	private:
		WINDOW *wnd;
		Point max;
		Point player;
		char prestau = ' ';
		char irestau = ' ';
		WINDOW *loadGameboard(char **argv);
		vector<Point> goals;
	public:
		Gameboard(int, int,char **argv);
		virtual ~Gameboard();
		virtual WINDOW *getWindowHandle();
		Point getPlayer();
		void movePlayer(Point);
		char getItem(Point pt);
		void moveItem(Point from, Point to);
		void displayGoals();
		bool areGoalsComplete();

};


#endif
