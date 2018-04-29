#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <SDL_mixer.h>
#include "helloworld.cpp"

/*
Written by Eric McLean and Tyler Corey
Written in C++ (GNU GCC)
Last updated 3/18/2018
Desc: This program initializes SDL libraries, snipes sprites to create buttons,
	then formats the images to the gui onscreen. The gui then produces another
	window upon clicking a button, and plays a sound effect.
*/

//sets screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//set button dimensions and number of buttons
const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 100;
const int TOTAL_BUTTONS = 2;

//basic functions that start, load, and run SDL
bool init();
bool loadMedia();
void close();

//the name of the window we create
SDL_Window* gWindow = NULL;

//the "screen" created by the window
SDL_Surface* gWindowSurface = NULL;

//our images
SDL_Surface* selectScreen = NULL;
SDL_Surface* fightButton = NULL;
SDL_Surface* backButton = NULL;
SDL_Surface* fightButtonClick = NULL;
SDL_Surface* backButtonClick = NULL;
//on click default = 0, click = 1

//button arrays
SDL_Rect buttonParams [TOTAL_BUTTONS];
SDL_Rect buttonPos [TOTAL_BUTTONS];
// 0: play
// 1: exit

//mouse handler
SDL_Point MPos[TOTAL_BUTTONS];
// 0: play
// 1: exit

//sound effect
Mix_Chunk *onClick = NULL;

bool init() {
	/* Desc: This fucntion initializes the SDL
		Libraries used in this program, as well as creating
		the window and surface we will use. Our audio is
		also initalized. */

	bool success = true;
	//initalize audio and vid
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "Choose Your Robot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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



	return success;
}

bool loadMedia() {
	/* Desc: This fucntion loads in the files needed to
		format the screen and sound effects. The buttons
		are clipped from a sprite sheet. The audio is loaded. */

	bool success = true;

	//title image loaded
	selectScreen = SDL_LoadBMP("mainmenu.bmp");
	if (selectScreen == NULL) {
		printf("Title Image could not be loaded");
		success = false;
	}

	//set buttons
	fightButton = SDL_LoadBMP("play.bmp");
	if (fightButton == NULL) {
		printf("Play image could not be loaded");
		success = false;
	}

	fightButtonClick = SDL_LoadBMP("playClick.bmp");
	if (fightButtonClick == NULL) {
		printf("play click could not be loaded");
		success = false;
	}

	backButton = SDL_LoadBMP("exit.bmp");
	if (backButton == NULL) {
		printf("exit image could not be loaded");
		success = false;
	}

	backButtonClick = SDL_LoadBMP("exitClick.bmp");
	if (backButtonClick == NULL) {
		printf("exit image click could not be loaded");
		success = false;
	}

	//set param and pos for each button
	//0: play
	//1: exit
	//2: play click
	//3: exit click
	for (int cnt = 0; cnt < TOTAL_BUTTONS; cnt++) {
		buttonParams[cnt].w = BUTTON_WIDTH;
		buttonParams[cnt].h = BUTTON_HEIGHT;

		buttonPos[cnt].x = 300;
		buttonPos[cnt].y = 300;

		if (cnt > 0) {
		buttonPos[cnt].y = buttonPos[cnt-1].y + 100;
		}
	}


	//load audio
	onClick = Mix_LoadWAV("ring.wav");
	if (onClick == NULL) {
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	return success;
}

void close()
{
	/* Desc: This fucntion frees all the resources
		brought in by the GUI. This also closes the window
		currently in use. */

	//Deallocate surface
	SDL_FreeSurface(selectScreen);
	SDL_FreeSurface(fightButton);
	SDL_FreeSurface(backButton);
	SDL_FreeSurface(fightButtonClick);
	SDL_FreeSurface(backButtonClick);
	selectScreen = NULL;
	fightButton = NULL;
	backButton = NULL;
	fightButtonClick = NULL;
	backButtonClick = NULL;

	//free music
	Mix_FreeChunk (onClick);
	onClick = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	Mix_Quit();
	SDL_Quit();
}

bool mouseHandle (SDL_Event* M) {
	/* Desc: this fucntion handles the position and actions
		of the mouse. The mouse is located, and on click a new button
		is shown to create a "flash" effect. A sound effect is
		also played. */

	bool quitter = false;
	MPos[0].x = buttonPos[0].x;
	MPos[0].y = buttonPos[0].y;
	MPos[1].x = buttonPos[1].x;
	MPos[1].y = buttonPos[1].y;

	//play button
	if( M->type == SDL_MOUSEMOTION || M->type == SDL_MOUSEBUTTONDOWN || M->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );


		bool inside = true;
		//left
		if (x < MPos[0].x)  {
			inside = false;
			//printf("left");
		}

		if (x > MPos[0].x + BUTTON_WIDTH) {
			inside = false;
			//printf("right");
		}

		if (y < MPos[0].y)  {
			inside = false;
			//printf("above");
		}

		if (y > MPos[0].y + BUTTON_HEIGHT) {
			inside = false;
			//printf("below");
		}

		if (inside) {
			switch (M->type) {
			case SDL_MOUSEBUTTONDOWN:
				SDL_BlitSurface(fightButtonClick, &buttonParams[0], gWindowSurface, &buttonPos[0]);
				Mix_PlayChannel(-1, onClick, 0);
				
				world();
				break;

				case SDL_MOUSEBUTTONUP:
				SDL_BlitSurface(fightButton, &buttonParams[0], gWindowSurface, &buttonPos[0]);
				break;

                }


            }

		}

	//exit button
	if( M->type == SDL_MOUSEMOTION || M->type == SDL_MOUSEBUTTONDOWN || M->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );


		bool inside = true;
		//left
		if (x < MPos[1].x)  {
			inside = false;
		}

		if (x > MPos[1].x + BUTTON_WIDTH) {
			inside = false;
		}

		if (y < MPos[1].y)  {
			inside = false;
		}

		if (y > MPos[1].y + BUTTON_HEIGHT) {
			inside = false;
		}

		if (inside) {
			switch (M->type) {
			case SDL_MOUSEBUTTONDOWN:
				SDL_BlitSurface(backButtonClick, &buttonParams[1], gWindowSurface, &buttonPos[1]);
				Mix_PlayChannel(-1, onClick, 0);
				break;

				case SDL_MOUSEBUTTONUP:
				SDL_BlitSurface(backButton, &buttonParams[1], gWindowSurface, &buttonPos[1]);
				quitter = true;
				break;
                }


            }

		}

		return quitter;

	}


int main( int argc, char* args[] ) {
    int cnt = 0;
	bool quit = false;
	SDL_Event M;

	//boot SDL and load menu
	init();
	loadMedia();


	//load title screen
	SDL_BlitSurface(selectScreen, NULL, gWindowSurface, NULL);

	//display buttons
    SDL_BlitSurface(fightButton, &buttonParams[cnt], gWindowSurface, &buttonPos[cnt]);
    cnt++;
    SDL_BlitSurface(backButton, &buttonParams[cnt], gWindowSurface, &buttonPos[cnt]);


	while (!quit) {
		while(SDL_PollEvent( &M ) != 0 )
			{

				quit = mouseHandle(&M);

				if( M.type == SDL_QUIT )
				{
					quit = true;
				}

			}
			//Update the screen
			SDL_UpdateWindowSurface( gWindow );
	}
	close();


	return 0;
}

