// TPFSConsolever.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include<iostream>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<string>  

/*0.5.0 新增射擊 */
/*0.6.0 碰撞,計分,修正一些東西 */
/*0.7.0 menu,開局時會倒數 */


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
	cout << "版本:0.7.0";
	cursor_locate(52, 6);
	cout << "得分：";
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
	COORD bullet[10];
	Frame enemy[8];

	int score = 0;
	int count = 0;

	Game();
	void menu();

	void de_plane();
	void plane_move(char);
	void print_plane();
	void plane_erase();

	void shoot();
	void de_bullet();
	void create_bullet();
	void bullet_erase();
	void bullet_move();
	void this_bullet_erase(COORD pos);

	void de_enemy();
	void print_enemy();
	void enemy_move();
	void enemy_erase();
	void this_enemy_erase(Frame pos);

	void playing();
	void collision();
	void pause();
	void gameover();
};

Game::Game()
{
	de_enemy();
	de_plane();
	de_bullet();
}

void Game::menu() {

	COORD x = { 10,5 };
	COORD y = { 53,15 };
	create_Frame(x, y, '=', '|');

	cursor_locate(25, 8);
	cout << "Fighter Game";
	cursor_locate(19, 12);
	string str = "Please enter 'w' to start";
	for (int i = 0; i<str.size(); i++)
	{
		Sleep(40);
		cout << str[i];
	}
	char keyin = _getch();
	while (keyin != 'w') {
		keyin = _getch();
	}
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

void Game::de_bullet() {
	for (int i = 0; i < 10; i++) {
		bullet[i].Y = 30;
	}
}

void Game::create_bullet() {
	for (int i = 0; i<10; i++)
	{
		if (bullet[i].Y != 30)
		{
			cursor_locate(bullet[i]);
			cout << "^";
		}
	}
}

void Game::bullet_erase() {
	for (int i = 0; i<10; i++)
		if (bullet[i].Y != 30)
		{
			COORD pos = { bullet[i].X, bullet[i].Y};
			cursor_locate(pos);
			cout << " ";
		}
}

void Game::bullet_move() {
	for (int i = 0; i<10; i++)
	{
		if (bullet[i].Y != 30)
		{
			bullet[i].Y -= 1;
			if (bullet[i].Y == 1)
			{
				COORD pos = { bullet[i].X, bullet[i].Y + 1 };
				this_bullet_erase(pos);
				bullet[i].Y = 30;
			}

		}
	}
}

void Game::this_bullet_erase(COORD pos) {
	cursor_locate(pos);
	cout << " ";
}

void Game::shoot() {
	for (int i = 0; i < 10; i++)
	{
		if (bullet[i].Y == 30) {
			bullet[i].X = position[5].X;
			bullet[i].Y = position[5].Y-1;
			break;
		}
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

void Game::this_enemy_erase(Frame pos) {
	create_Frame(pos, ' ', ' ');
}

void Game::collision() {
	for (int i = 0; i < 8; i++)
		for (int j = 0; j<9; j++)
			if (collision_frame(enemy[i], position[j]))
			{
				create_Frame(enemy[i], '*', '*');
				Sleep(1000);
				gameover();
			}
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 10; j++)
			if (collision_frame(enemy[i], bullet[j]))
			{
				score += 5;
				this_enemy_erase(enemy[i]);
				COORD a = { 1, 1 };
				COORD b = { 45, 3 };
				enemy[i].position[0] = random(a, b);
				enemy[i].position[1].X = enemy[i].position[0].X + 3;
				enemy[i].position[1].Y = enemy[i].position[0].Y + 2;
				this_bullet_erase(bullet[j]);
				bullet[j].Y = 30;
			}
}
void Game::playing() {

	int enemy_quantity=0;
	int bullet_quantity = 0;
	print_plane();
	print_enemy();


	if (0 == count) {
		for (int i = 5; i > 0; i--) {
			cursor_locate(23, 12);
			cout << i;
			Sleep(1000);
			cursor_locate(23, 12);
			cout << "     ";
		}
	}


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
				pause();
			}
		}
		shoot();
		if (0==bullet_quantity ) {
			bullet_erase();
			bullet_move();
			create_bullet();
		}

		bullet_quantity++;
		
		if (5 == bullet_quantity) bullet_quantity = 0;
		
		if (0 == enemy_quantity)
		{
			enemy_erase();
			enemy_move();
			print_enemy();
			
		}
		collision();
		cursor_locate(60, 6);
		cout << score;
	}
}

void Game::gameover() {
	system("cls");
	COORD x = {28,9};
	COORD y = {53,15};
	create_Frame(x, y, '=', '|');
	cursor_locate(37, 11);
	string str1 = "Game Over";
	for (int i = 0; i<str1.size(); i++)
	{
		Sleep(80);
		cout << str1[i];
	}
	Sleep(80);
	cursor_locate(37, 12);
	string str2 = "score:";
	for (int i = 0; i<str2.size(); i++)
	{
		Sleep(80);
		cout << str2[i];
	}
	Sleep(160);
	cout<< score;
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

void Game::pause() {
	cursor_locate(22, 12);
	string str = "-Pause-";
	for (int i = 0; i<str.size(); i++)
	{
		Sleep(80);
		cout << str[i];
	}
	char keyin = _getch();
	while (keyin != 'p')
		keyin = _getch();
	cursor_locate(22, 12);
	cout << "         ";
}


int main()
{
	cursor_hide();
	Game game;
	game.menu();
	system("cls");
	print_playing_frame();
	game.playing();
}



