// TPFSConsolever.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include<iostream>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<string>  

/**
1.輸出影像printcol、printrow。
2.鼠標定位cursor_locate、隱藏cursor_hide。
3.創造邊框create_Frame,;
4.

**/



using namespace std;

/****/
const int n = 55, m = 35;
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
/****/

/**Structure**/
typedef struct Frame
{
	COORD position[2];
	int flag;
}Frame;


/**All the sub_function **/

void cursor_locate(COORD pos)
{
	SetConsoleCursorPosition(hout, pos);

};
void cursor_locate(int x, int y)
{
	COORD pos = { x,y };
	cursor_locate(pos);
};
// 隐藏游標
void cursor_hide()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1,0 };
	SetConsoleCursorInfo(hout, &cursor_info);
}

void print_row(int y,int x1,int x2,char item) {
	cursor_locate(x1, y);
	for (int i = 0; i <= (x2 - x1); i++) cout << item;
}

void print_row(COORD a, COORD b, char item)
{
	if (a.Y == b.Y)
		print_row(a.Y, a.X, b.X, item);
}


void print_col(int x, int y1, int y2, char item) {

	for (int i=y1;i != y2 + 1;i++)
	{
		cursor_locate(x, i);
		cout << item;
	}
}

void print_col(COORD a, COORD b, char item) {
	if (a.X == b.X)
		print_col(a.X, a.Y, b.Y, item);
}
void create_Frame(COORD a, COORD b, char rowitem, char colitem) {
	print_row(a.Y, a.X + 1, b.X - 1, rowitem);
	print_row(b.Y, a.X + 1, b.X - 1, rowitem);
	print_col(a.X, a.Y + 1, b.Y - 1, colitem);
	print_col(b.X, a.Y + 1, b.Y - 1, colitem);
}

void create_Frame(int x1, int y1, int x2, int y2, char row, char col)
{
	COORD a = { x1, y1 };
	COORD b = { x2, y2 };
	create_Frame(a, b, row, col);
}

void print_playing_frame()
{
	create_Frame(0, 0, 48, 24, '=', '|');//	draw map frame;
	create_Frame(49, 0, 79, 4, '-', '|');//		draw output frame
	create_Frame(49, 4, 79, 9, '-', '|');//		draw score frame
	create_Frame(49, 9, 79, 20, '-', '|');//	draw operate frame
	create_Frame(49, 20, 79, 24, '-', '|');//	draw other message frame
	cursor_locate(52, 2);
	cout << "製作者:FCH";
	cursor_locate(52, 3);
	cout << "版本:0.1.0";
	cursor_locate(52, 6);
	cout << "得分：";
	cursor_locate(52, 7);
	cout << "稱號：";
	cursor_locate(52, 10);
	cout << "操作方式：";
	cursor_locate(52, 12);
	cout << "  a,s,d,w 控制移動。";
	cursor_locate(52, 14);
	cout << "  暫停遊戲。";
	cursor_locate(52, 16);
	cout << "  退出遊戲。";
	cursor_locate(52, 18);
}


int main()
{
	print_playing_frame();
	system("PAUSE");
    return 0;
}



