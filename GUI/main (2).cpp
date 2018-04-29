#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <SDL_mixer.h>
#include <windows.h>
#include <sstream>
#include <ctime>
#include "stats.h"
#include <SDL_ttf.h>

//http://headerphile.blogspot.com/2014/07/sdl2-part-10-text-rendering.html
using namespace std;

const int MAX_ROBOTS = 2;


//sets screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//set button dimensions and number of buttons
const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 100;
const int TOTAL_BUTTONS = 2;


SDL_Rect robPos [2];
SDL_Rect arenaPos;


const int rob1 = 0; //becky
const int rob2 = 1; //steve

/* const int ROBHEIGHT;
const int ROBWIDTH;

const int ARENA_SIZE;
const int SIDEBAR; */

//basic functions that start, load, and run SDL
bool init();
bool loadMedia();
void close();
void createTextures();
SDL_Texture* SurfaceToTexture( SDL_Surface* surf );

//the name of the window we create
SDL_Window* gWindow = NULL;
SDL_Window* textWindow = NULL;

//the "screen" created by the window
SDL_Surface* gWindowSurface = NULL;

//our images
SDL_Surface* arena = NULL;
SDL_Surface* robot1 = NULL;
SDL_Surface* robot2 = NULL;


//font /  text
TTF_Font* Sans =  TTF_OpenFont("Sans.ttf", 24);
TTF_Font* font; //sketchy.ttf

SDL_Color White = {255, 255, 255};
SDL_Color bgColor = { 0, 0, 0, 255 };
SDL_Surface* actualText = TTF_RenderText_Solid(font, "Omaewah", White);

SDL_Texture* solidTexture;

SDL_Renderer* renderer;
SDL_Rect textRect;
SDL_Rect solidRect;




bool init() {
	/* Desc: This fucntion initializes the SDL
		Libraries used in this program, as well as creating
		the window and surface we will use. Our audio is
		also initalized. */

	bool success = true;
	//initalize audio and vid
	if( SDL_Init( SDL_INIT_EVERYTHING) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "Choose Your Robot",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gWindowSurface = SDL_GetWindowSurface( gWindow );
		}



	}
					//freq, format, channels, sample size
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {

		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	if (TTF_Init() == -1) {

		success = false;
	}

	//font = TTF_OpenFont( fontName.c_str(), 90 );


	return success;
}

bool loadMedia() {

    textRect.x = 200;
    textRect.y = 100;
    textRect.w = 800;
    textRect.h = 300;

    int xMax, yMax, xMin, yMin, xRange, yRange;

	 /*Desc: This fucntion loads in the files needed to
		format the screen and sound effects. The buttons
		are clipped from a sprite sheet. The audio is loaded.
        */

	bool success = true;

	//title image loaded
	arena = SDL_LoadBMP("arena.bmp");
	if (arena == NULL) {
		printf("arena Image could not be loaded");
		success = false;
	}

	//set buttons
	robot1 = SDL_LoadBMP("becky.bmp");
	if (robot1 == NULL) {
		printf("Play image could not be loaded");
		success = false;
	}

	robot2 = SDL_LoadBMP("steve.bmp");
	if (robot2 == NULL) {
		printf("play click could not be loaded");
		success = false;
	}


	arenaPos.x = 50;
	arenaPos.y = 50;

    xMin = 50;
    yMin = 50;
    xMax = 600;
    yMax = 550;

    xRange = xMax - xMin;
    yRange = yMax - yMin;

	//RNG for robots
	//position has to be within arena
	//pos cannot be ==

    int pos = 5;
    srand (time(NULL));
	for (int cnt = 0; cnt < 2; cnt++) {


		pos = xMin + int(xRange * rand()) / ((RAND_MAX + 1.0));
		cout << pos << endl;
		robPos[cnt].x = pos;

		pos = yMin + int(yRange * rand()) / ((RAND_MAX + 1.0));
		cout << pos << endl;;
		robPos[cnt].y = pos;



	}




	return success;
}

void close() {
	/* Desc: This fucntion frees all the resources
		brought in by the GUI. This also closes the window
		currently in use. */

	//Deallocate surface
	SDL_FreeSurface(arena);
	SDL_FreeSurface(robot1);
	SDL_FreeSurface(robot2);
	arena = NULL;
	robot1 = NULL;
	robot2 = NULL;
	//free music


	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	SDL_Quit();
}

	int main (int argc, char* args[] ){
	SDL_Event M;

    init();
    loadMedia();

	SDL_BlitSurface(arena, NULL, gWindowSurface, &arenaPos);
	SDL_BlitSurface(robot1, NULL, gWindowSurface, &robPos[0]);
	SDL_BlitSurface(robot2, NULL, gWindowSurface,  &robPos[1]);
	SDL_UpdateWindowSurface(gWindow);

	int cnt = 0;
	int cnta = 0;
	robPos[1].x = 500;
	Stats stats;

 	while (cnta < 10) {
		while (cnt < 200) {
			robPos[1].x++;
			//cout <<  robPos[1].x << endl;

			if (robPos[1].x >= 589) {
				cout << "break" << endl;
				cout <<  robPos[1].x << endl;
				break;
			}

			SDL_BlitSurface(robot2, NULL, gWindowSurface,  &robPos[1]);
			SDL_UpdateWindowSurface(gWindow);
			cnt++;
		}

		while (cnt > -500) {
			robPos[1].x--;
			//cout <<  robPos[1].x << endl;

			if (robPos[1].x <= 51) {
				cout << "break" << endl;
				cout <<  robPos[1].x << endl;
				break;
			}

			SDL_BlitSurface(robot2, NULL, gWindowSurface,  &robPos[1]);
			SDL_UpdateWindowSurface(gWindow);
			cnt--;
		}

		cnta++;

	}

	//reset


	cnt = 0;
	cnta = 0;
    cout << "y pos is " << robPos[1].y << endl;
    while (cnta < 5) {
        while (cnt > -200) {
			robPos[1].y--;
			//cout <<  robPos[1].x << endl;

			if (robPos[1].y <= 51) {
				cout << "break" << endl;
				cout <<  robPos[1].y << endl;
				break;
			}

			SDL_BlitSurface(robot2, NULL, gWindowSurface,  &robPos[1]);
			SDL_UpdateWindowSurface(gWindow);
			cnt--;
		}

		while (cnt < 1000) {
			robPos[1].y++;
			//cout <<  robPos[1].x << endl;

			if (robPos[1].y >= 539) {
				cout << "break" << endl;
				cout <<  robPos[1].y << endl;
				break;
			}
			SDL_BlitSurface(robot2, NULL, gWindowSurface,  &robPos[1]);
			SDL_UpdateWindowSurface(gWindow);
			cnt++;
		}

		cnta++;

    }





    //run match


    // show stats
    int a[MAX_ROBOTS];
    stats.init();
	stats.shotCnt(3, 1);  // steve
	stats.shotCnt(1, 0); //becky

    stats.killCnt(1);


	for (int cnt = 0; cnt < MAX_ROBOTS; cnt++) {
		a[cnt] = rand() % 5;
	}

    for (int cnt = 0; cnt < MAX_ROBOTS; cnt++) {
		stats.winLossChk(a[cnt], cnt);
	}

	//display header
	stats.displayHead();
	//display each line
	for (int cnt = 0; cnt < MAX_ROBOTS; cnt++) {
		stats.displayLine(cnt);
	}



	SDL_Delay( 6000 );


    close();
    return 0;
}

