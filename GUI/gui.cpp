#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <SDL_mixer.h>
#include "helloworld.cpp"
#include "fOpen.cpp"
#include "fOpen.h"
#include <SDL_ttf.h>
#include <vector>


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
void closeAll();
void launch();
void closeMenu();
string disect(string);
void createRenderer();


//the name of the window we create
SDL_Window* gWindow = NULL;
SDL_Window* nWindow = NULL;

//the "screen" created by the window
SDL_Surface* gWindowSurface = NULL;
SDL_Surface* nWindowSurface = NULL;

//our images
SDL_Surface* titleScreen = NULL;
SDL_Surface* playButton = NULL;
SDL_Surface* exitButton = NULL;
SDL_Surface* playButtonClick = NULL;
SDL_Surface* exitButtonClick = NULL;
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



//text
TTF_Font* font = NULL; 



SDL_Color textColor = {255, 255, 255, 255};
SDL_Surface *message = NULL;
SDL_Renderer *renderer;
SDL_Texture *SurfaceToTexture( SDL_Surface* surf );
SDL_Texture *messageTexture = NULL;
SDL_Rect textRect;


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
		gWindow = SDL_CreateWindow( "ATRobots: Revised", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );


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

	//turn on text


	if (TTF_Init() == -1) {

		cout << "ttf init error: " << TTF_GetError();
	}
	
	


	return success;
}

bool loadMedia() {
	/* Desc: This fucntion loads in the files needed to
		format the screen and sound effects. The buttons
		are clipped from a sprite sheet. The audio is loaded. */

	bool success = true;

	//title image loaded
	titleScreen = SDL_LoadBMP("mainmenu.bmp");
	if (titleScreen == NULL) {
		printf("Title Image could not be loaded");
		success = false;
	}

	//set buttons
	playButton = SDL_LoadBMP("play.bmp");
	if (playButton == NULL) {
		printf("Play image could not be loaded");
		success = false;
	}

	playButtonClick = SDL_LoadBMP("playClick.bmp");
	if (playButtonClick == NULL) {
		printf("play click could not be loaded");
		success = false;
	}

	exitButton = SDL_LoadBMP("exit.bmp");
	if (exitButton == NULL) {
		printf("exit image could not be loaded");
		success = false;
	}

	exitButtonClick = SDL_LoadBMP("exitClick.bmp");
	if (exitButtonClick == NULL) {
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

void closeAll()
{
	/* Desc: This fucntion frees all the resources
		brought in by the GUI. This also closes the window
		currently in use. */

	//Deallocate surface
	SDL_FreeSurface(titleScreen);
	SDL_FreeSurface(playButton);
	SDL_FreeSurface(exitButton);
	SDL_FreeSurface(playButtonClick);
	SDL_FreeSurface(exitButtonClick);
	titleScreen = NULL;
	playButton = NULL;
	exitButton = NULL;
	playButtonClick = NULL;
	exitButtonClick = NULL;

	//free music
	Mix_FreeChunk (onClick);
	onClick = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	SDL_DestroyWindow( nWindow );
	nWindow = NULL;
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
				Mix_PlayChannel(-1, onClick, 0);
				SDL_BlitSurface(playButtonClick, &buttonParams[0], gWindowSurface, &buttonPos[0]);
				launch();
				break;

				case SDL_MOUSEBUTTONUP:
				SDL_BlitSurface(playButton, &buttonParams[0], gWindowSurface, &buttonPos[0]);
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
				SDL_BlitSurface(exitButtonClick, &buttonParams[1], gWindowSurface, &buttonPos[1]);
				Mix_PlayChannel(-1, onClick, 0);
				break;

				case SDL_MOUSEBUTTONUP:
				SDL_BlitSurface(exitButton, &buttonParams[1], gWindowSurface, &buttonPos[1]);
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
	SDL_BlitSurface(titleScreen, NULL, gWindowSurface, NULL);

	//display buttons
    SDL_BlitSurface(playButton, &buttonParams[cnt], gWindowSurface, &buttonPos[cnt]);
    cnt++;
    SDL_BlitSurface(exitButton, &buttonParams[cnt], gWindowSurface, &buttonPos[cnt]);



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
	//TTF_CloseFont( font );
	closeAll();


	return 0;
}

void launch() {
	SDL_Event M;
	bool quit = false;
	string fileName;
    const char* SDL_GetError();
	
	TTF_Font* font = TTF_OpenFont("arial.ttf", 25);

	if (!font) {

		cout << "ttf opn:" << TTF_GetError();
	}

	closeMenu();
		
	createRenderer();

	fOpen f;

	fileName = disect(f.getInPath());
	const char *c = fileName.c_str();

	cout << fileName << endl;
	cout << c << endl;
	
	if (font == NULL) {
		
		cout << TTF_GetError(); 
	}

	message = TTF_RenderText_Solid(font, c, textColor);
	

	
	messageTexture = SurfaceToTexture(message);

	SDL_QueryTexture(messageTexture, NULL, NULL, NULL, NULL);

	textRect.w = 300;
	textRect.h = 100;

	SDL_RenderClear( renderer );


	SDL_RenderCopy(renderer, messageTexture, NULL, &textRect);
	SDL_RenderPresent(renderer);


	while (!quit) {
		while(SDL_PollEvent( &M ) != 0 )
			{

				quit = mouseHandle(&M);

				if( M.type == SDL_QUIT )
				{
					quit = true;
				}

				if (M.type == SDL_KEYDOWN) {

					if (M.key.keysym.sym == SDLK_q ) {


					}


				}

			}
			//Update the screen
			SDL_UpdateWindowSurface( nWindow );
	}

}

void closeMenu() {

	SDL_FreeSurface(titleScreen);
	SDL_FreeSurface(playButton);
	SDL_FreeSurface(exitButton);
	SDL_FreeSurface(playButtonClick);
	SDL_FreeSurface(exitButtonClick);
	titleScreen = NULL;
	playButton = NULL;
	exitButton = NULL;
	playButtonClick = NULL;
	exitButtonClick = NULL;

	//free music
	Mix_FreeChunk (onClick);
	onClick = NULL;

	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

}


 string disect(string path) {
	string fileName = "";
	int length = path.length();
	int lastBack = 0;
	int dotCnt;

	for (int cnt = 3; cnt < length; cnt++) {

		if (path[cnt] == '.') {
			dotCnt = cnt;

		}

		if (path[cnt] == '\\') {
			lastBack = cnt;
		}

	}


	for (int cnt = lastBack + 1; cnt < dotCnt; cnt++) {
		if (path[cnt] == '.') {
			break;
		}

		fileName += path[cnt];

	}


return fileName;
}

void createRenderer() {
	int x, y;

	nWindow = SDL_CreateWindow( "CHUNGUS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

	renderer = SDL_CreateRenderer( nWindow, -1, SDL_RENDERER_ACCELERATED );

	if (renderer == NULL) {

		cout << "ERROR: " << SDL_GetError() << endl;

	}


	x = SDL_RenderSetLogicalSize( renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (x < 0) {

		cout << "ERROR2: " << SDL_GetError() << endl;

	}


	y = SDL_SetRenderDrawColor( renderer, 34, 150, 200, 255 );

	if (y < 0) {
		cout << "ERROR3: " << SDL_GetError() << endl;

	}





}

SDL_Texture* SurfaceToTexture( SDL_Surface* surf )
{
	SDL_Texture* text;

	text = SDL_CreateTextureFromSurface( renderer, surf );

	if (text == NULL) {

		cout << "ERROR4: " << SDL_GetError() << endl;
	}

	SDL_FreeSurface( surf );

	return text;
}
