#include <SFML/Graphics.hpp>
//#include <SFML/Randomizer.hpp>
#include <time.h>
#include <fstream>
#include <string>
#include <cstdlib>

#include <random>
#include <ctime>

#pragma warning(disable : 4996)

using namespace sf;
using namespace std;


const int M = 20;
const int N = 10;

int counter = 0;
int lifes = 0;

int field[M][N] = { 0 };



bool FirstQuarter, SecondQuarter, ThirdQuarter, FourQuarter;

float losowa;

float tabela_losowa[500];
int ghost_direction = 3;
int ghost_direction_old {0};

int liczymy;
int index = 0;

struct Point
{
	int x, y;
} a, b, a1, b1;   



//a, b - wspolrzedne pacmana
//a1, b1 - wspolrzedne ducha

int mapka[20][21] =
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,7,2,2,2,2,2,2,2,2,9,2,2,2,2,2,2,2,2,9,1,
	1,2,1,2,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,2,1,
	1,2,1,9,2,2,2,2,2,2,9,9,2,2,2,9,1,9,1,2,1,
	1,2,1,2,1,1,1,1,1,1,1,2,1,1,1,2,1,2,1,2,1,
	1,2,1,2,1,1,1,1,1,1,1,2,2,1,1,2,1,2,1,2,1,
	1,2,1,2,1,1,9,2,2,9,1,2,1,1,1,2,1,2,1,2,1,
	1,2,1,2,1,1,2,2,2,2,2,2,9,1,1,2,1,2,1,2,1,
	1,2,1,2,1,1,9,2,1,1,1,1,2,1,1,2,1,2,1,2,1,
	1,2,2,9,1,1,1,2,1,0,0,1,2,1,1,2,1,2,1,2,1,
	1,9,1,2,1,1,1,2,1,0,1,1,2,1,1,2,1,2,1,2,1,
	1,2,1,2,1,1,1,2,1,0,1,1,2,1,1,2,1,2,1,2,1,
	1,2,1,9,2,2,2,2,9,9,2,2,9,1,1,2,1,2,1,2,1,
	1,2,1,2,1,1,1,1,2,1,1,1,2,1,1,2,1,2,1,2,1,
	1,2,1,9,1,1,9,2,2,2,2,2,9,1,1,2,1,2,1,2,1,
	1,2,1,2,1,1,1,1,2,1,1,1,1,1,1,2,1,2,1,2,1,
	1,9,2,9,2,2,2,2,9,2,9,2,2,2,2,2,2,9,2,9,1,
	1,2,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,2,1,
	1,9,2,2,2,2,2,2,2,2,9,2,2,2,2,2,2,2,2,9,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

// 1 - wall, 2 - ball, 3- fruit
bool check()
{
	
		if (a.x < 0 || a.x >= 21 || a.y >= 20) return 0;   //sprawdzanie czy nie wychodzimy poza obrys
		else if ((mapka[a.y][a.x]) == 1) return 0; 
	
	return 1;
};

bool ghostcheck()
{

	if (a1.x < 0 || a1.x >= 21 || a1.y >= 20) return 0;   //sprawdzanie czy nie wychodzimy poza obrys
	else if ((mapka[a1.y][a1.x]) == 1) return 0;

	return 1;
};

int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(672, 700), "XDMan");

	Font font;
	font.loadFromFile("C:/Users/gwern/source/repos/xdman/Fonts/Raleway-Light.ttf");

	Text text;
	Text lives;

	text.setFont(font);
	lives.setFont(font);

	text.setCharacterSize(24);
	lives.setCharacterSize(24);

	text.setFillColor(Color::Black);
	lives.setFillColor(Color::Black);

	text.setPosition(10, 650);
	lives.setPosition(200, 650);

	Texture t1, t2, t3, t4, t5, t6, t7;
	t1.loadFromFile("images/tiles.png");
	t2.loadFromFile("images/background.png");
	
	t4.loadFromFile("C:/Users/gwern/source/repos/xdman/images/background2.png");
	t5.loadFromFile("C:/Users/gwern/source/repos/xdman/images/pacman_set.png");
	t6.loadFromFile("C:/Users/gwern/source/repos/xdman/images/ball.png");
	t7.loadFromFile("C:/Users/gwern/source/repos/xdman/images/ghost.png");

	Sprite s(t1), background(t2), frame(t3), wall(t4), pacman(t5), ball(t6), ghost(t7);
			
	int dx = 0, dy = 0, dx1 = 0, dy1 = 0;
	int direction = 2;

  	float timer = 0, delay = 0.1;
		Clock clock;

    a.x = 1;
	a.y = 1;

 	a1.x = 9;
	a1.y = 9;


	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		
		string str = std::to_string(ghost_direction);
		text.setString("Score : " + str);
		
		string liv = std::to_string(direction); //counter
		
		lives.setString("Life : " + liv);

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Up) { direction = 4; }
				else if (e.key.code == Keyboard::Down) {  direction = 3; }
				else if (e.key.code == Keyboard::Left) {  direction = 2; }
				else if (e.key.code == Keyboard::Right) {  direction = 1; }
		}

		/////////draw//////////

		window.clear(Color::White);

		window.draw(text);
		window.draw(lives);

		ghost.setTextureRect(IntRect(0, 0, 32, 32));
		ghost.setPosition(a1.x * 32, a1.y * 32);
		window.draw(pacman);
	
		///////Tick//////
		if (timer > delay)

			
		{

				ghost_direction_old = ghost_direction;

				index = index + 1;

				losowa = rand() / static_cast<float>(RAND_MAX);

				if ((a.x < a1.x) and (a.y <= a1.y)) {
					FirstQuarter = true; SecondQuarter = false; ThirdQuarter = false;  FourQuarter = false;
				} //1
				if ((a.x >= a1.x) and (a.y < a1.y)) {
					SecondQuarter = true; FirstQuarter = false;  ThirdQuarter = false;  FourQuarter = false;
				}  //2
				if ((a.x <= a1.x) and (a.y >= a1.y)) {
					ThirdQuarter = true; FirstQuarter = false; SecondQuarter = false;   FourQuarter = false;
				}    //3
				if ((a.x > a1.x) and (a.y > a1.y)) {
					FourQuarter = true;  FirstQuarter = false; SecondQuarter = false; ThirdQuarter = false;
				}   //4



				if ((mapka[a1.y][a1.x] == 9) or (mapka[a1.y][a1.x] == 8))
				{
					if (FirstQuarter)
					{
						if (losowa <= 0.5) {

							tabela_losowa[index] = losowa;



							if ((mapka[a1.y - 1][a1.x] != 1) and ((a1.x != 9) and (a1.y != 12))) {
								ghost_direction = 4; //gora
							}
							/*
							else if (mapka[a1.x - 1][a1.y] != 1) {
								ghost_direction = 2; //lewa
							}

							*/
						}
						else {
							tabela_losowa[index] = losowa;

							if (mapka[a1.y][a1.x - 1] != 1) {
								ghost_direction = 2; //lewa
							}
							/*
							else if (mapka[a1.x][a1.y - 1] != 1) {
								ghost_direction = 4; //gora

							}
							*/
						}

						if ((mapka[a1.y - 1][a1.x] == 1) and (mapka[a1.y][a1.x - 1] == 1))
						{
							ghost_direction = 4; //prawa

						}

					}

					if (SecondQuarter)
					{
						if (losowa <= 0.5) {

							tabela_losowa[index] = losowa;

							if (mapka[a1.x][a1.y - 1] != 1) {
								ghost_direction = 4; //gora
							}
							else if (mapka[a1.x + 1][a1.y] != 1) {
								ghost_direction = 1; //prawa
							}
						}
						else {

							tabela_losowa[index] = losowa;

							if (mapka[a1.x + 1][a1.y] != 1) {
								ghost_direction = 1; //prawa
							}
							else if (mapka[a1.x][a1.y - 1] != 1) {
								ghost_direction = 4; //gora

							}
						}

						if ((mapka[a1.x][a1.y - 1] == 1) and (mapka[a1.x + 1][a1.y] == 1))
						{
							ghost_direction = 2; //lewa

						}

					}

					if (ThirdQuarter)
					{
						if (losowa <= 0.5) {

							tabela_losowa[index] = losowa;

							if (mapka[a1.x][a1.y + 1] != 1) {
								ghost_direction = 3; //dol
							}
							else if (mapka[a1.x - 1][a1.y] != 1) {
								ghost_direction = 2; //lewa
							}
						}
						else {

							tabela_losowa[index] = losowa;

							if (mapka[a1.x - 1][a1.y] != 1) {
								ghost_direction = 2; //lewa
							}

							else if (mapka[a1.x][a1.y + 1] != 1) {
								ghost_direction = 3; //dol
							}
						}

						if ((mapka[a1.x][a1.y + 1] == 1) and (mapka[a1.x - 1][a1.y] == 1))
						{
							ghost_direction = 1; //prawa

						}

					}

					if (FourQuarter)
					{
						if (losowa <= 0.5) {

							tabela_losowa[index] = losowa;

							if (mapka[a1.x][a1.y + 1] != 1) {
								ghost_direction = 3; //dol
							}
							else if (mapka[a1.x + 1][a1.y] != 1) {
								ghost_direction = 1; //prawa
							}
						}
						else {

							tabela_losowa[index] = losowa;

							if (mapka[a1.x + 1][a1.y] != 1) {
								ghost_direction = 1; //prawa
							}

							else if (mapka[a1.x][a1.y + 1] != 1) {
								ghost_direction = 3; //dol
							}
						}

						if ((mapka[a1.x][a1.y + 1] == 1) and (mapka[a1.x + 1][a1.y] == 1))
						{
							ghost_direction = 2; //prawa

						}

					}


				}


				liczymy = liczymy + 1;

				

				if (!ghostcheck())
				{
					a1 = b1; dx1 = 0; dy1 = 0;

				}

				if ((mapka[a.y][a.x]) == 2)
				{
					mapka[a.y][a.x] = 0;
					counter = counter + 1;
				}
				if ((mapka[a.y][a.x]) == 9)
				{
					mapka[a.y][a.x] = 8;
					counter = counter + 1;
				}

				if (direction == 1)
				{
					pacman.setTextureRect(IntRect(0, 0, 32, 32)); //prawa
					pacman.setPosition(a.x * 32, a.y * 32);
					dx = 1;
				}
				if (direction == 2)
				{
					pacman.setTextureRect(IntRect(32, 0, 32, 32)); //lewa
					pacman.setPosition(a.x * 32, a.y * 32);
					dx = -1;
				}
				if (direction == 3)
				{
					pacman.setTextureRect(IntRect(64, 0, 32, 32)); //dol
					pacman.setPosition(a.x * 32, a.y * 32);
					dy = 1;
				}
				if (direction == 4)
				{
					pacman.setTextureRect(IntRect(96, 0, 32, 32)); //gora
					pacman.setPosition(a.x * 32, a.y * 32);
					dy = -1;
				}

				if (ghost_direction == 3)
				{
					ghost.setPosition(a1.x * 32, a1.y * 32);
					dy1 = 1;

				}
				if (ghost_direction == 4)
				{

					ghost.setPosition(a1.x * 32, a1.y * 32);
					dy1 = -1;
				}
				if (ghost_direction == 1)
				{

					ghost.setPosition(a1.x * 32, a1.y * 32);
					dx1 = +1;
				}
				if (ghost_direction == 2)
				{

					ghost.setPosition(a1.x * 32, a1.y * 32);
					dx1 = -1;
				}

				//// <- Move -> ///
				{ b = a; a.x += dx; a.y += dy; } // zapisujemy wartoœæ zmiennej Point a w postaci Pointa b, dodajemy do zmiennej x dx
				{ b1 = a1; a1.x += dx1; a1.y += dy1; }

				if (!check())  a = b;   // je¿eli check jest negatywny (równy 0), przywróc star¹ wersjê Pointa a
				dx = 0; dy = 0;
		}
		
			timer = 0;
		 
		

		
		

	//	text.setPosition(100, 650);
		//window.draw(text);
		
		wall.setTextureRect(IntRect(0, 96, 32, 32));
		ball.setTextureRect(IntRect(0, 0, 32, 32));

		for (int i = 0; i < 22; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				if (mapka[i][j] == 1)
				{
					wall.setPosition(j * 32, i * 32);
					window.draw(wall);
				}
				if ((mapka[i][j] == 2) or (mapka[i][j] == 9))
				{
					ball.setPosition(j * 32, i * 32);
					window.draw(ball);
				}

			}

		}
		
		window.draw(ghost);
		window.display();
		
	}
	return 0;
	
}
