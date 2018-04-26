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
#include <windows.h>
#include <sstream>
#include <ctime>
#include "SDL.h"

/*
Written by Eric McLean and Tyler Corey
Written in C++ (GNU GCC)
Last updated 3/18/2018
Desc: This program initializes SDL libraries, snipes sprites to create buttons,
	then formats the images to the gui onscreen. The gui then produces another
	window upon clicking a button, and plays a sound effect.

*/

using namespace std;

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
void selectScreen();
void displayText(int);


//the name of the window we create
SDL_Window* gWindow = NULL;
SDL_Window* nWindow = NULL; // the selector window
SDL_Window* aWindow = NULL; // arena window

//the "screen" created by the window
SDL_Surface* gWindowSurface = NULL;
SDL_Surface* nWindowSurface = NULL;
SDL_Surface* aWindowSurface = NULL; //arena surface

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
SDL_Color textColor = {255, 255, 255, 255};
SDL_Surface *message = NULL;
SDL_Texture *SurfaceToTexture( SDL_Surface* surf );
SDL_Texture *messageTexture = NULL;
SDL_Rect textRect;
TTF_Font* font = TTF_OpenFont("arial.ttf", 25);



//selector SDL stuff
const int TOTAL_R_BUTTONS = 10;

SDL_Surface* selectRButton [TOTAL_R_BUTTONS];
SDL_Surface* selectRButtonClick [TOTAL_R_BUTTONS];
SDL_Surface* selectBG = NULL;

SDL_Rect rButtonParam[TOTAL_R_BUTTONS];
SDL_Rect rCButtonParam[TOTAL_R_BUTTONS];

SDL_Rect rButtonPos[TOTAL_R_BUTTONS];

SDL_Event* S[TOTAL_R_BUTTONS];  //mouse handler for the select screen
SDL_Point SPos[TOTAL_R_BUTTONS];


//arena stuff
const int MAX_ROBOTS = 2;

SDL_Rect robPos [2];
SDL_Rect arenaPos;

int rob1 = NULL; //becky
int rob2 = NULL; //steve

SDL_Surface* arena = NULL;
SDL_Surface* robot1 = NULL;
SDL_Surface* robot2 = NULL;


//shapes button
const int TOTAL_SHAPES = 10;
SDL_Surface* shapes [10];
int cycle = 0;

SDL_Rect shapeParam[TOTAL_R_BUTTONS];
SDL_Rect shapePos[TOTAL_R_BUTTONS];


//robot sprite arrays
//this is just referencing the SHAPE
SDL_Surface* sprites[TOTAL_SHAPES];
SDL_Rect spritePos[TOTAL_SHAPES];

std::string getShape;

SDL_Renderer* renderer;

SDL_Texture* rob1T = NULL;

SDL_Texture* arenaT = NULL;
SDL_Rect arenaParam;

void closeSelect();
void loadArena();
int loopShapes();

void createRenderer() {
	renderer = SDL_CreateRenderer(aWindow, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL) {
		cout << SDL_GetError() << endl;

	}

}

void createTextures() {
	rob1T = SDL_CreateTextureFromSurface(renderer, sprites[cycle]);

	if (rob1T == NULL) {
		cout << "error" << endl;
		cout << SDL_GetError() << endl;
	}

	arenaT = SDL_CreateTextureFromSurface(renderer, arena);

	if (arenaT == NULL) {
		cout << "error again" << endl;
		cout << SDL_GetError() << endl;
	}
}


bool initSDL() {
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
	int xMax, yMax, xMin, yMin, xRange, yRange;
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


	//load images for the select screen
	selectRButton[0] = SDL_LoadBMP("r0.bmp");
	if (exitButtonClick == NULL) {
		printf("r 0 could not be loaded ");
		success = false;
	}

	selectRButton[1] = SDL_LoadBMP("r1.bmp");
	if (exitButtonClick == NULL) {
		printf("r 1 could not be loaded ");
		success = false;
	}


	for (int cnt = 0; cnt < TOTAL_R_BUTTONS; cnt++) {
		rButtonParam[cnt].w = 75;
		rButtonParam[cnt].h = 75;

		rButtonPos[cnt].x = 100;
		rButtonPos[cnt].y = 100;

		if (cnt > 0) {
			if (cnt > 0) {
			rButtonPos[cnt].y = rButtonPos[cnt-1].y + 100;

			if (cnt > 4) {
				rButtonPos[cnt].x = rButtonPos[cnt-1].x + 200;
			}

		}
    }
}



	selectRButtonClick[0] = SDL_LoadBMP("r0click.bmp");
	if (exitButtonClick == NULL) {
		printf("r 0 click could not be loaded ");
		success = false;
	}

	selectRButtonClick[1] = SDL_LoadBMP("r1click.bmp");
	if (exitButtonClick == NULL) {
		printf("r 1 click could not be loaded ");
		success = false;
	}


	for (int cnt = 0; cnt < TOTAL_R_BUTTONS; cnt++) {
		rCButtonParam[cnt].w = 75;
		rCButtonParam[cnt].h = 75;

		rButtonPos[cnt].x = 100;
		rButtonPos[cnt].y = 100;

		if (cnt > 0) {
			rButtonPos[cnt].y = rButtonPos[cnt-1].y + 100;

			if (cnt > 4) {
				rButtonPos[cnt].x = rButtonPos[cnt-1].x + 200;
			}

		}



	}


	//load select screen background
	selectBG = SDL_LoadBMP("selectBG.bmp");
	if (selectBG == NULL) {
		printf("Select bg not loaded");
		success = false;
	}

	arena = SDL_LoadBMP("arena.bmp");
	if (arena == NULL) {
		printf("arena Image could not be loaded");
		success = false;
	}

	//set buttons
/* 	robot1 = SDL_LoadBMP("becky.bmp");
	if (robot1 == NULL) {
		printf("Play image could not be loaded");
		success = false;
	}

	robot2 = SDL_LoadBMP("steve.bmp");
	if (robot2 == NULL) {
		printf("play click could not be loaded");
		success = false;
	}
 */

    xMin = 25;
    yMin = 25;
    xMax = 550;
    yMax = 550;

    xRange = xMax - xMin;
    yRange = yMax - yMin;

	//RNG for robots
	//position has to be within arena
	//pos cannot be ==

    int pos = 5;
    srand (time(NULL));
	for (int cnt = 0; cnt < 10; cnt++) {


		pos = xMin + (int)(xRange * rand()) / ((RAND_MAX + 1.0));
		cout << pos << endl;
		spritePos[cnt].x = pos;

		pos = yMin + (int)(yRange * rand()) / ((RAND_MAX + 1.0));
		cout << pos << endl;;
		spritePos[cnt].y = pos;

		spritePos[cnt].w = 16;
		spritePos[cnt].h = 16;

	}

	//load audio
	onClick = Mix_LoadWAV("ring.wav");
	if (onClick == NULL) {
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}


	for (int cnt = 0; cnt < 10; cnt++) {
		switch (cnt) {
			case 0: getShape = "circle";
			break;
			case 1: getShape = "star";
			break;
			case 2: getShape = "triangle";
			break;
			case 3: getShape = "tear";
			break;
			case 4: getShape = "diamond";
			break;
			case 5: getShape = "pacman";
			break;
			case 6: getShape = "mushroom";
			break;
			case 7: getShape = "hour";
			break;
			case 8: getShape = "galaga";
			break;
			case 9: getShape = "pyramid";
			break;
			default: getShape = "fail";
			break;
		}
		getShape = getShape + ".bmp";

		const char *c = getShape.c_str();

		shapes[cnt] = SDL_LoadBMP(c);

		if (shapes[cnt] == NULL) {

			cout << "ERROR" << endl;
			cout << SDL_GetError() << endl;
		}

		if (cnt < TOTAL_R_BUTTONS) {
			shapeParam[cnt].w = 75;
			shapeParam[cnt].h = 75;

			shapePos[cnt].x = 200;
			shapePos[cnt].y = 100;


        }
	}



	for (int cnt = 0; cnt < 10; cnt++) {
		switch (cnt) {
			case 0: getShape = "smol/circle";
			break;
			case 1: getShape = "smol/star";
			break;
			case 2: getShape = "smol/triangle";
			break;
			case 3: getShape = "smol/tear";
			break;
			case 4: getShape = "smol/diamond";
			break;
			case 5: getShape = "smol/pacman";
			break;
			case 6: getShape = "smol/mushroom";
			break;
			case 7: getShape = "smol/hour";
			break;
			case 8: getShape = "smol/galaga";
			break;
			case 9: getShape = "smol/pyramid";
			break;
			default: getShape = "smol/fail";
			break;
		}


		getShape = getShape + ".bmp";
		cout << getShape << endl;

		const char *c = getShape.c_str();

		sprites[cnt] = SDL_LoadBMP(c);

		if (cnt < TOTAL_R_BUTTONS) {
			shapeParam[cnt].w = 75;
			shapeParam[cnt].h = 75;

			shapePos[cnt].x = 200;
			shapePos[cnt].y = 100;


        }
	}




	return success;
}

void closeAll(){
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

bool mouseHandleS(SDL_Event* S) {
/*  Desc: this fucntion handles the position and actions
		of the mouse. The mouse is located, and on click a new button
		is shown to create a "flash" effect. A sound effect is
		also played. */

	bool quitter = false;


	//don't really know why i had to do this, but it's leftover from the gui
	for (int cnt = 0; cnt < TOTAL_R_BUTTONS; cnt++) {

		SPos[cnt].x = rButtonPos[cnt].x;
		SPos[cnt].y = rButtonPos[cnt].y;
	}

	//r1 button
	if( S->type == SDL_MOUSEMOTION || S->type == SDL_MOUSEBUTTONDOWN || S->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;
		//left
		if (x < SPos[0].x)  {
			inside = false;
			//printf("left");
		}

		if (x > SPos[0].x + 75) {
			inside = false;
			//printf("right");
		}

		if (y < SPos[0].y)  {
			inside = false;
			//printf("above");
		}

		if (y > SPos[0].y + 75) {
			inside = false;
			//printf("below");
		}

		if (inside) {
			switch (S->type) {
			case SDL_MOUSEBUTTONDOWN:
				SDL_BlitSurface(selectRButton[0], &rButtonParam[0], nWindowSurface, &rButtonPos[0]);
				displayText(0);
				break;

				 case SDL_MOUSEBUTTONUP:

				break;

                }


            }

		}


	 //r2
	 if( S->type == SDL_MOUSEMOTION || S->type == SDL_MOUSEBUTTONDOWN || S->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;
		//left
		if (x < SPos[1].x)  {
			inside = false;

		}

		if (x > SPos[1].x + BUTTON_WIDTH) {
			inside = false;

		}

		if (y < SPos[1].y)  {
			inside = false;

		}

		if (y > SPos[1].y + BUTTON_HEIGHT) {
			inside = false;

		}

		if (inside) {
			switch (S->type) {
			case SDL_MOUSEBUTTONDOWN:
				SDL_BlitSurface(selectRButtonClick[1], &rCButtonParam[1], nWindowSurface, &rButtonPos[1]);
				displayText(1);
				break;

				case SDL_MOUSEBUTTONUP:
				SDL_BlitSurface(selectRButton[1], &rButtonParam[1], nWindowSurface, &rButtonPos[1]);
				break;

                }


            }

		}

	//shapes 1
	 if( S->type == SDL_MOUSEMOTION || S->type == SDL_MOUSEBUTTONDOWN || S->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;
		//left
		if (x < shapePos[0].x)  {
			inside = false;

		}

		if (x > shapePos[0].x + 75) {
			inside = false;

		}

		if (y < shapePos[0].y)  {
			inside = false;

		}

		if (y > shapePos[0].y + 75) {
			inside = false;

		}

		if (inside) {
			switch (S->type) {
			case SDL_MOUSEBUTTONDOWN:
				rob1 = loopShapes();
				break;

				case SDL_MOUSEBUTTONUP:

				break;

                }


            }

		}

	//new play button
	if( S->type == SDL_MOUSEMOTION || S->type == SDL_MOUSEBUTTONDOWN || S->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;
		//left
		if (x < buttonPos[0].x)  {
			inside = false;

		}

		if (x > buttonPos[0].x + BUTTON_WIDTH) {
			inside = false;

		}

		if (y < buttonPos[0].y)  {
			inside = false;

		}

		if (y > buttonPos[0].y + BUTTON_HEIGHT) {
			inside = false;

		}

		if (inside) {
			switch (S->type) {
			case SDL_MOUSEBUTTONDOWN:
				loadArena();
				break;

				case SDL_MOUSEBUTTONUP:

				break;

                }


            }

		}




		return quitter;


}

int loopShapes() {
	cycle++;


	if (cycle > 9) {
		cycle = 0;
	}

	SDL_BlitSurface(shapes[cycle], &shapeParam[cycle], nWindowSurface, &shapePos[cycle]);
	SDL_UpdateWindowSurface(nWindow);

	return cycle;

}

int main( int argc, char* args[] ) {
    int cnt = 0;
	bool quit = false;
	SDL_Event M;

	//boot SDL and load menu
	initSDL();
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

	closeAll();


	return 0;
}

void launch() {
	SDL_Event S;
	bool quit = false;
	string fileName;


	TTF_Font* font = TTF_OpenFont("arial.ttf", 25);

	if (!font) {

        TTF_GetError();
	}

	closeMenu();

	selectScreen();

	while (!quit) {
		while(SDL_PollEvent( &S ) != 0 )
			{

				quit = mouseHandleS(&S);
				if (quit == true) {

					break;
				}


			}
			//Update the screen
			SDL_UpdateWindowSurface( nWindow );
	}

	//createRenderer
	closeSelect();

	loadArena();




}

void closeMenu() {

	//closes the main menu

	SDL_FreeSurface(titleScreen);
	//SDL_FreeSurface(playButton);
	SDL_FreeSurface(exitButton);
	SDL_FreeSurface(playButtonClick);
	SDL_FreeSurface(exitButtonClick);
	titleScreen = NULL;
	//playButton = NULL;
	exitButton = NULL;
	playButtonClick = NULL;
	exitButtonClick = NULL;

	//free music
	Mix_FreeChunk (onClick);
	onClick = NULL;

	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

}

void closeSelect() {

	SDL_FreeSurface(nWindowSurface);

	for (int cnt = 0; cnt > TOTAL_R_BUTTONS; cnt++) {
		SDL_FreeSurface(selectRButton[cnt]);
		SDL_FreeSurface(selectRButtonClick[cnt]);
		SDL_FreeSurface(selectBG);

	}

	SDL_DestroyWindow( nWindow );
	nWindow = NULL;


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

void selectScreen() {

	nWindow = SDL_CreateWindow("Select Your RBT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	nWindowSurface = SDL_GetWindowSurface(nWindow);
	SDL_BlitSurface(selectBG, NULL, nWindowSurface, NULL);


	for (int cnt = 0; cnt < 2; cnt++) {
		SDL_BlitSurface(selectRButton[cnt], &rButtonParam[cnt], nWindowSurface, &rButtonPos[cnt]);

	}

	SDL_BlitSurface(shapes[0], &shapeParam[0], nWindowSurface, &shapePos[0]);

	buttonPos[0].x = 300;
	buttonPos[0].y = 401;

	SDL_BlitSurface(playButton, &buttonParams[0], gWindowSurface, &buttonPos[0]);

	SDL_UpdateWindowSurface( nWindow );


}

void displayText (int buttonNum) {
	string fileName;

	TTF_Font* font = TTF_OpenFont("smol/arial.ttf", 25);


	if (!font) {
		exit(1);
	}

	if (!TTF_WasInit() && TTF_Init() == -1) {
		exit(1);

	}


	fOpen f;
	fileName = disect(f.getInPath());
	const char *c = fileName.c_str();

	message = TTF_RenderText_Solid(font, c, textColor);

	int x = 300;
	int y = 120;
	SDL_Rect textLocation = { x, y, 0, 0 };



	if (buttonNum > 0) {
		textLocation.y = textLocation.y + (100 * buttonNum);

	}

	SDL_BlitSurface(message, NULL, nWindowSurface, &textLocation);

	SDL_UpdateWindowSurface(nWindow);

	TTF_CloseFont( font );
		if (!font) {

		cout << "null is working" << endl;
	}

	font = NULL;


}

void loadArena ()  {

	aWindow = SDL_CreateWindow("FIGHT!!!!!!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	aWindowSurface = SDL_GetWindowSurface(aWindow);

	createRenderer();

	createTextures();

	arenaPos.x = 25;
	arenaPos.y = 25;
    arenaPos.w = 550;
	arenaPos.h = 550;

    arenaParam.x = 550;
	arenaParam.y = 550;
	//SDL_RenderCopy(renderer, arenaT, NULL, NULL);
	SDL_RenderCopyEx(renderer, arenaT, NULL, &arenaPos, 0, NULL, SDL_FLIP_NONE);

	SDL_RenderCopyEx(renderer, rob1T, NULL, &spritePos[cycle], 0, NULL, SDL_FLIP_NONE);

	SDL_RenderPresent(renderer);

	SDL_Delay( 2000 );


	int cnt = 0;

	//move in the X direction 200px, or until the wall is hit.
	while (cnt < 200) {
		spritePos[cycle].x++;

		if (spritePos[cycle].x >= 550) {
			break;

		}

		SDL_RenderClear(renderer);
		SDL_RenderCopyEx(renderer, arenaT, NULL, &arenaPos, 0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(renderer, rob1T, NULL, &spritePos[cycle], 0, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent(renderer);

		cnt++;

	}

	SDL_Delay( 2000 );
	
	double angle = 0.0; 
	cnt = 0;
	while (cnt < 200) {
		
		SDL_RenderClear(renderer);
		SDL_RenderCopyEx(renderer, arenaT, NULL, &arenaPos, 0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(renderer, rob1T, NULL, &spritePos[cycle], angle + cnt, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent(renderer);

		cnt++;
		
		
		
	}


	
	cnt = 0;
	while (cnt < 200) {
		spritePos[cycle].y++;

		if (spritePos[cycle].y >= 550) {
			break;

		}

		SDL_RenderClear(renderer);
		SDL_RenderCopyEx(renderer, arenaT, NULL, &arenaPos, 0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(renderer, rob1T, NULL, &spritePos[cycle], 0, NULL, SDL_FLIP_NONE);
		SDL_RenderPresent(renderer);

		cnt++;

	}

	SDL_Delay( 2000 );
	//reset

	
	exit(0);
}

void startMenu() {
	int cnt = 0;
	bool quit = false;
	SDL_Event M;


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

	return;
}


