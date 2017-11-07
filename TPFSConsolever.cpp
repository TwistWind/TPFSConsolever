// TPFSConsolever.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include<iostream>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<string>  



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

void create_Frame(Frame frame, char row, char col)
{
	COORD a = frame.position[0];
	COORD b = frame.position[1];
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
	cout << "版本:0.4.0";
	cursor_locate(52, 6);
	cout << "得分：";
	cursor_locate(52, 7);
	cout << "稱號：";
	cursor_locate(52, 10);
	cout << "操作方式：";
	cursor_locate(52, 12);
	cout << "  a,s,d,w 控制移動。";
	cursor_locate(52, 14);
	cout << "  P:暫停遊戲。";
	cursor_locate(52, 16);
}

int random(int max, int min) {
	srand(time(NULL));
	int result = (rand() % (max - min)) + max;
	return result;
}

COORD random(COORD max, COORD min)
{
	int x = random(max.X, min.X);
	int y = random(max.Y, min.Y);
	COORD result = { x, y };
	return result;
}

bool collision_frame(Frame frame, COORD node) {
	if (node.X >= frame.position[0].X) {
		if (node.X <= frame.position[1].X)
			if (node.Y >= frame.position[0].Y)
				if (node.Y <= frame.position[1].Y)
					return true;

	}
	return false;
}

/*Game code*/

class Game {
public:
	COORD position[10];
	Frame enemy[8];

	Game();
	void de_plane();
	void plane_move(char);
	void print_plane();
	void plane_erase();

	void de_enemy();
	void print_enemy();
	void enemy_move();
	void enemy_erase();

	void playing();
	void collision();
	void game_over();
};

Game::Game()
{
	de_enemy();
	de_plane();
}

void Game::de_plane() {

	COORD centren = { 39, 22 };
	position[0].X = position[5].X = position[7].X = position[9].X = centren.X;
	position[1].X = centren.X - 2;
	position[2].X = position[6].X = centren.X - 1;
	position[3].X = position[8].X = centren.X + 1;
	position[4].X = centren.X + 2;

	for (int i = 0; i <= 4; i++)position[i].Y = centren.Y;
	for (int i = 6; i <= 8; i++)position[i].Y = centren.Y + 1;

	position[5].Y = centren.Y - 1;
	position[9].Y = centren.Y - 2;
}

void Game::print_plane() {
	for (int i = 0; i<9; i++)
	{
		cursor_locate(position[i]);
		switch (i) {
		case 0:cout << "0"; break;
		case 1:cout << "_"; break;
		case 2:cout << "/"; break;
		case 3:cout << "\\"; break;
		case 4:cout << "_"; break;
		case 5:cout << "|"; break;
		case 6:cout << "/"; break;
		case 7:cout << "|"; break;
		case 8:cout << "\\"; break;
		}
	}
}

void Game::plane_move(char keyin) {

	if (keyin == 'a')if (position[1].X != 1)	for (int i = 0; i <= 9; i++)position[i].X -= 2;
	if (keyin == 's')if (position[7].Y != 23)	for (int i = 0; i <= 9; i++)position[i].Y += 1;
	if (keyin == 'd')if (position[4].X != 47)	for (int i = 0; i <= 9; i++)position[i].X += 2;
	if (keyin == 'w')if (position[5].Y != 3)	for (int i = 0; i <= 9; i++)position[i].Y -= 1;
}

void Game::plane_erase() {
	for (int i = 0; i<9; i++)
	{
		cursor_locate(position[i]);
		cout << " ";
	}
}

void Game::de_enemy() {
	COORD a = { 1, 1 };
	COORD b = { 45, 15 };
	for (int i = 0; i<8; i++)
	{
		enemy[i].position[0] = random(a, b);
		enemy[i].position[1].X = enemy[i].position[0].X + 3;
		enemy[i].position[1].Y = enemy[i].position[0].Y + 2;
	}
}

void Game::print_enemy() {
	for (int i = 0; i<8; i++)	create_Frame(enemy[i].position[0], enemy[i].position[1], '-', '|');
}

void Game::enemy_move(){
	for (int i = 0; i<8; i++)
	{
		for (int j = 0; j<2; j++)
			enemy[i].position[j].Y++;

		if (24 == enemy[i].position[1].Y)
		{
			COORD a = { 1, 1 };
			COORD b = { 45, 3 };
			enemy[i].position[0] = random(a, b);
			enemy[i].position[1].X = enemy[i].position[0].X + 3;
			enemy[i].position[1].Y = enemy[i].position[0].Y + 2;
		}
	}
}

void Game::enemy_erase() {
	for (int i = 0; i<8; i++)	create_Frame(enemy[i].position[0], enemy[i].position[1], ' ', ' ');

}
void Game::collision() {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j<9; j++)
			if (collision_frame(enemy[i], position[j]))
			{
				create_Frame(enemy[i], '*', '*');
				Sleep(1000);
				break;
			}
	
}
void Game::playing() {

	int enemy_quantity=0;
	print_plane();
	print_enemy();

	while (1)
	{
		Sleep(100);
		if (_kbhit())
		{
			char keyin = _getch();
			if ('a' == keyin || 's' == keyin || 'd' == keyin || 'w' == keyin)
			{
				plane_erase();
				plane_move(keyin);
				print_plane();
				
			}
			else if ('p' == keyin)
			{				
				game_over();
				break;
			}
		}
		if (0 == enemy_quantity)
		{
			enemy_erase();
			enemy_move();
			print_enemy();
			
		}
		collision();
	}
}

void Game::game_over() {
	system("cls");
	COORD x = {28,9};
	COORD y = {53,15};
	create_Frame(x, y, '=', '|');
	cursor_locate(37, 11);
	string str = "-Pause-";
	for (int i = 0; i<str.size(); i++)
	{
		Sleep(80);
		cout << str[i];
	}
	cursor_locate(34, 13);
	cout << "Continue?(y/n):";
as:
	char keyin= _getch();
	if (keyin == 'n')
		exit(0);
	else if (keyin == 'y')
	{
		system("cls");
		Game game;
		system("cls");
		print_playing_frame();
		game.playing();
	}
	else 
		goto as;
}


int main()
{
	cursor_hide();
	Game game;
	print_playing_frame();
	game.playing();
}



