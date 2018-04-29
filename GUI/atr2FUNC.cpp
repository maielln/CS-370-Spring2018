#include <iostream>
#include <ctime>
#include <string>
#include <math.h>
#include <chrono>
#include <thread>
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

#define pi 3.1415926535897932384626433

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

long delay_per_sec;
bool registered,graphix,sound_on;
string reg_name;
uint16_t reg_num;            //ORIGIONALLY TYPE :word!!!!!!!!!!!
double sint[256],cost[256];

string addrear(string b,int l);
double distance(int x1, int y1, int x2, int y2);
int rol(int n,int k);
int ror(int n,int k);
int sal(int n,int k);
int sar(int n,int k);
double abs(double x);
void textxy(int x,int y, string s);
void coltextxy(int x,int y,string s,uint8_t c);
char hexnum(uint8_t num);
string hexb(uint8_t num);
string hex(uint16_t num );
double valuer(string i);
long value(string i);
string cstrr(double i);
string cstr(long i);
string zero_pad(long n,long l);
string zero_pads(string s ,long l);
string addfront(string b,int l);
string ucase(string s);
string lcase(string s);
string space(uint8_t i);
string repchar(char c,uint8_t i);
string ltrim(string s1);
string rtrim(string s1);
string btrim(string s1);
string lstr(string s1,int  l);
string rstr(string s1,int l);
void FlushKey();
void calibrate_timing();
void time_delay(int n); //{n=milliseconds}
void check_registration();
void viewport(int x1,int y1,int x2,int y2);
void main_viewport();
void make_tables();
//FINISH FUNCTIONS BELOW HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!
int robot_color(int n);
void box(int x1,int y1,int x2,int y2);
void hole(int x1,int y1,int x2,int y2);
void chirp();
void click();
int hex2int(string s);
int str2int(string s);
string bin(int n);
string decimal(int num,int length );

//***GUI pointers/variables ***///

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
SDL_Renderer *renderer;
SDL_Texture *SurfaceToTexture( SDL_Surface* surf );
SDL_Texture *messageTexture = NULL;
SDL_Rect textRect;
TTF_Font* font = TTF_OpenFont("arial.ttf", 25);


//selector SDL stuff
const int TOTAL_R_BUTTONS = 6;

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

const int rob1 = 0; //becky
const int rob2 = 1; //steve

SDL_Surface* arena = NULL;
SDL_Surface* robot1 = NULL;
SDL_Surface* robot2 = NULL;


//shapes button 
const int TOTAL_SHAPES = 10; 
SDL_Surface* shapes[10] = NULL;
SDL_Surface* shapesB[10] = NULL;
int cycle = 0; 

SDL_Rect shapeParam[TOTAL_R_BUTTONS];
SDL_Rect shapePos[TOTAL_R_BUTTONS]; 

//GUI definitions
bool initSDL(); //turn on SDL *important*
bool loadMedia(); //load images *IMPORTANT*
bool mouseHandle(SDL_Event* M); 


//NOTE: probably need to add "loadrobotimg" function
//***GUI FUNCTIONS START HERE **** //
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


		pos = xMin + (int)(xRange * rand()) / ((RAND_MAX + 1.0));
		cout << pos << endl;
		robPos[cnt].x = pos;

		pos = yMin + (int)(yRange * rand()) / ((RAND_MAX + 1.0));
		cout << pos << endl;;
		robPos[cnt].y = pos;

	}

	//load audio
	onClick = Mix_LoadWAV("ring.wav");
	if (onClick == NULL) {
		printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}

	string getShape; 
	
	for (int cnt = 0; cnt < 10; cnt++) {
		switch (cnt) {
			case 0: getShape = "circle"; 
			case 1: getShape = "star"; 
			case 2: getShape = ""; 
			case 3: getShape = ""; 
			case 4: getShape = ""; 
			case 5: getShape = ""; 
			case 6: getShape = ""; 
			case 7: getShape = ""; 
			case 8: getShape = ""; 
			case 9: getShape = ""; 
			default: getShape = "fail"; 
		}
		
		shapes[cnt] = SDL_LoadBMP(getShape + ".bmp"); 
		
		if (cnt < TOTAL_R_BUTTONS) {
			shapeParam[cnt].w = 16;
			shapeParam[cnt].h = 16;

			shapePos[cnt].x = 130;
			shapePos[cnt].y = 100;

			if (cnt > 0) {
				rButtonPos[cnt].y = rButtonPos[cnt-1].y + 100;

				if (cnt > 4) {
					rButtonPos[cnt].x = rButtonPos[cnt-1].x + 200;
				}

			}	
		}
		
		
		
	}

	return success;
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
				selectMenu(); 
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

bool mouseHandleS(SDL_Event* S, int cycle) {
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

		if (x > SPos[0].x + BUTTON_WIDTH) {
			inside = false;
			//printf("right");
		}

		if (y < SPos[0].y)  {
			inside = false;
			//printf("above");
		}

		if (y > SPos[0].y + BUTTON_HEIGHT) {
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

	
	for (int cnt = 0; cnt < TOTAL_R_BUTTONS; cnt++) {
		//may need another position array?????
		SPos[cnt].x = shapePos[cnt].x;
		SPos[cnt].y = shapePos[cnt].y;
	}
	
	//shapes1
	if( S->type == SDL_MOUSEMOTION || S->type == SDL_MOUSEBUTTONDOWN || S->type == SDL_MOUSEBUTTONUP ) {
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;
		//left
		if (x < SPos[0].x)  {
			inside = false;
			//printf("left");
		}

		if (x > SPos[0].x + BUTTON_WIDTH) {
			inside = false;
			//printf("right");
		}

		if (y < SPos[0].y)  {
			inside = false;
			//printf("above");
		}

		if (y > SPos[0].y + BUTTON_HEIGHT) {
			inside = false;
			//printf("below");
		}

		if (inside) {
			switch (S->type) {
			case SDL_MOUSEBUTTONDOWN:
				cycleShapes(cycle); 
				break;

				 case SDL_MOUSEBUTTONUP:

				break;

                }


            }
		
	}
		
	//shapes2 
	if( S->type == SDL_MOUSEMOTION || S->type == SDL_MOUSEBUTTONDOWN || S->type == SDL_MOUSEBUTTONUP ) {
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		bool inside = true;
		//left
		if (x < SPos[0].x)  {
			inside = false;
			//printf("left");
		}

		if (x > SPos[0].x + BUTTON_WIDTH) {
			inside = false;
			//printf("right");
		}

		if (y < SPos[0].y)  {
			inside = false;
			//printf("above");
		}

		if (y > SPos[0].y + BUTTON_HEIGHT) {
			inside = false;
			//printf("below");
		}

		if (inside) {
			switch (S->type) {
			case SDL_MOUSEBUTTONDOWN:
				cycleShapes(cycle); 
				break;

				 case SDL_MOUSEBUTTONUP:

				break;

                }


            }
		
	}
		
	
		return quitter;


}
	
void startMenu() {
	//Brings up the start menu and 
	//handles the mouse and clicking 
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

void selectMenu() {
	SDL_Event S;
	bool quit = false;
	string fileName;
	
	closeStartMenu(); 

	nWindow = SDL_CreateWindow("Select Your Robot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	nWindowSurface = SDL_GetWindowSurface(nWindow);
	SDL_BlitSurface(selectBG, NULL, nWindowSurface, NULL);


	for (int cnt = 0; cnt < 2; cnt++) {
		SDL_BlitSurface(selectRButton[cnt], &rButtonParam[cnt], nWindowSurface, &rButtonPos[cnt]);

	}
	
	for (int cnt = 0; cnt < 2; cnt++) {
		SDL_BlitSurface(shapes[cycle], &shapeParam[cnt], nWindowSurface, &shapePos[cnt]);
		
	} 

	SDL_UpdateWindowSurface( nWindow );

	
	while (!quit) {
			while(SDL_PollEvent( &S ) != 0 )
				{

					quit = mouseHandleS(&S, cycle);
					if (quit == true) {

						break;
					}


				}
				//Update the screen
				SDL_UpdateWindowSurface( nWindow );
		}
	
		return; 
	
}

void cycleShapes() {
	cycle++; 
	
	if (cycle > 9) {
		cycle = 0; 
		
	}
	
	SDL_BlitSurface(shapes[cycle], &shapeParam[cycle], nWindowSurface, &shapePos[cycle]); 
		
}

void closeStartMenu() {
	//closes main menu 
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

void closeSelectMenu() {
	
SDL_FreeSurface(nWindowSurface);

	for (int cnt = 0; cnt > TOTAL_R_BUTTONS; cnt++) {
		SDL_FreeSurface(selectRButton[cnt]);
		SDL_FreeSurface(selectRButtonClick[cnt]);
		SDL_FreeSurface(selectBG);

	}

	SDL_DestroyWindow( nWindow );
	nWindow = NULL;
}

void arena () {
	aWindow = SDL_CreateWindow("FIGHT!!!!!!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	aWindowSurface = SDL_GetWindowSurface(aWindow);
	
	SDL_BlitSurface(arena, NULL, aWindowSurface, &arenaPos);
	SDL_BlitSurface(robot1, NULL, aWindowSurface, &robPos[0]);
	SDL_BlitSurface(robot2, NULL, aWindowSurface,  &robPos[1]);
	SDL_UpdateWindowSurface(aWindow);
	
	//the assets for the arena are loaded in 
	//just visuals, where do we get our info?
	
	
}


void closeArena() {
	
	SDL_FreeSurface(robot1);
	SDL_FreeSurface(robot2); 
	SDL_FreeSurface(arena); 

	SDL_DestroyWindow( aWindow );
	aWindow = NULL;
}

//takes the file path from fOpen and produces the robot 
//name as a string 
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

//we'll probably use this instead of textXY, but
//either can be used 
void displayText (int buttonNum) {
	string fileName;

	TTF_Font* font = TTF_OpenFont("arial.ttf", 25);


	if (!font) {

        cout << "no font  " << SDL_GetError();
		exit(1);
	}

	if (!TTF_WasInit() && TTF_Init() == -1) {
		cout << "ya done fucked up " << TTF_GetError();
		exit(1);

	}


	fOpen f;
	fileName = disect(f.getInPath());
	const char *c = fileName.c_str();

	message = TTF_RenderText_Solid(font, c, textColor);

	int x = 180;
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


void textxy(int x,int y, string s, SDL_Surface windowSurface, SDL_Window window)  //GUI PEOPLE NEED TO DO THIS (prints string to screen at cord. x & y)
{
	//turn on font
	TTF_Font* font = TTF_OpenFont("arial.ttf", 25);

	if (!font) {

        TTF_GetError();
	}

	const char *c = s; 
	//set location for text
	SDL_Rect textLocation = { x, y, 0, 0 };
	
	//put text on screen (screen must be specified)
	SDL_BlitSurface(message, NULL, window, &textLocation);

	//update window 
	SDL_UpdateWindowSurface(nWindow);
	
	
}

//WHY do we need this? - EM
void coltextxy(int x,int y,string s,uint8_t c)//GUI PEOPLE NEED TO DO THIS(changes color of the text and displays string)
{
/*    setcolor(c);
    textxy(x,y,s);
*/
}

char hexnum(uint8_t num)
{
    switch(num)
    {
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return 'A';
        case 11: return 'B';
        case 12: return 'C';
        case 13: return 'D';
        case 14: return 'E';
        case 15: return 'F';
        default: return 'X';
    }
}

string hexb(uint8_t num)
{
    return ((hexnum(num >> 4) + hexnum(num & 15))+"");
}

string hex(uint16_t num )
{
    return ((hexb(num >> 8) + hexb(num & 255))+"");
}

double valuer(string i)
{
    double s;
    int n;
 //   val(i,s,n);
    if (n>0)
    {
        s = 0;
    }
    return s;
}

 long value(string i)
 {
    long s;
    int n;
  //  val(i,s,n);
    if (n>0)
    {
        s = 0;
    }
    return s;
 }

 string zero_pads(string s ,long l)
 {
    string s1;
    s1=s;
    while (s1.length()<l)
    {
        s1 = '0'+s1;
    }

    return s1;
 }

 string addfront(string b,int l)
 {
    while (b.length()< l)
    {
        b = ' '+b;
    }
    return b;
 }

string ucase(string s)
{
    for(int i = 0; i < s.length(); i++)
    {
        if(s[i] >= 97 && s[i] <= 122)
        {
            s[i] = s[i] - 32;
        }
    }
    return s;
}

string lcase(string s)
{
    int i;
    if (s.length()>=1)
    {
        for (i = 0; i< s.length();i++)
        {
            if ((s[i]>=65) && (s[i]<=90))
            {
                s[i] = s[i]+32;
            }
        }

    }
    return s;
}

string space(uint8_t i)
{
    string s;
    int k ;
    s = "";
    if (i>0)
    {
        for (k = 1; k <= i; k++)
        {
            s = s+' ';
        }
    }
    return s;
}

string repchar(char c,uint8_t i)
{
    string s;
    int k;

    s = "";
    if (i>0)
    {
        for (k = 1; k <= i; k++)
        {
            s = s + c;
        }
    }
    return s;
}


string addrear(string b,int l)
{
    while ((int)(b.length())< l)
    {
        b = b+' ';
    }
    return b;
}

string zero_pad(long n,long l)
{
    string s;

    s = n + "";
    while (s.length()<l)
    {
        s ='0'+s;
    }

    return s;
}

double distance(int x1, int y1, int x2, int y2)
{
    return pow((pow(x2-x1,2) + pow(y2-y1,2)),.5);
}

int rol(int n,int k)
{
/*  asm                     //THE NEXT FEW FUNCTIONS NEED TO KNOW HOW TO WORK WITH ASSEMBLY!!!!!
    cld
    mov   cx,     k
    rep   rol     n,      1
*/
 return n;
}

int ror(int n,int k)
{
/*    asm
    cld
    mov   cx,     k
    rep   ror     n,      1
    end;
*/
    return n;
}

int sal(int n,int k)
{
/*    asm
    cld
    mov   cx,     k
    cmp   cx,     0
    jle   @2
    @1:
    sal   n,      1
    loop  @1
    @2:
*/
    return n;
}

int sar(int n,int k)
{
/*    asm
    cld
    mov   cx,     k
    cmp   cx,     0
    jle   @2
    @1:
    sar   n,      1
    loop  @1
    @2:
*/
    return n;
}

void FlushKey()                 //{ Clears any key strokes in the key-  }
                                //{ board buffer so a couple of key     }
{                               //{ presses don't race you through program. }
/*    Regs : Registers;
    Regs.AH := $01;  { AH=1: Check for keystroke }
    Intr($16,regs); { Interupt $16: Keyboard services}
    IF (regs.Flags and $0040) = 0 then { if chars in buffer }
        REPEAT
        Regs.AH := 0;
        Intr($16,Regs);
        Regs.AH := $01;
        Intr($16,Regs);
    Until (regs.flags and $0040) <> 0;
    */
}

void calibrate_timing()
{
    long i,k;

    delay_per_sec = 0;
/*    k=mem[0:$46C];
    repeat until (k!=mem)[0:$46C];
    k=mem[0:$46C];
    repeat delay(1);
    delay_per_sec++;
    until k<>mem[0:$46C];
    delay_per_sec = round(delay_per_sec*18.2);
*/
}

void time_delay(int n)
{
     long i,l;
     if (delay_per_sec==0)
     {
         calibrate_timing();
     }
     l = round(n/1000*delay_per_sec);

     for (i = 1; i<= l; i++)
     {
         sleep_for(nanoseconds(10));
     }
}

void check_registration()
{
    uint8_t w;
    int i;
    //f:text;
    string s;

    registered = false;
    if (/*exist('ATR2.REG')*/true)
    {
 /*       assign(f,'ATR2.REG');
        reset(f);
        readln(f,reg_name);
        readln(f,reg_num);
        close(f);
        w = 0;
        s = btrim(ucase(reg_name));
*/        for (i=0; i < s.length();i++)
        {
            w+=(int)(s[i]);
        }

        w = w ^ 23205;
        if (w==reg_num)
        {
            registered = true;
        }
    }
}

void viewport(int x1,int y1,int x2,int y2)      //GUI CODE!!!!!!!!!!!!!!
{
 /*   if (!graphix)
    {
        return;
    }
    setviewport(x1,y1,x2,y2,true);
*/
}

void main_viewport()    //MORE CODE FOR GUI!!!!!!!!!
{
    //viewport(5,5,474,474); //{470x470}
}

void make_tables()
{
    int i,j,k;
    for (i = 0; i<= 255;i++)
    {
        sint[i] = sin(i/128*pi);
        cost[i] = cos(i/128*pi);
    }
}

int robot_color(int n)
{
    int k;
    k = 7;
    switch (n % 14)
    {
        case 0:k = 10;
            break;
        case 1:k = 12;
            break;
        case 2:k = 9;
            break;
        case 3:k = 11;
            break;
        case 4:k = 13;
            break;
        case 5:k = 14;
            break;
        case 6:k = 7;
            break;
        case 7:k = 6;
            break;
        case 8:k = 2;
            break;
        case 9:k = 4;
            break;
        case 10:k = 1;
            break;
        case 11:k = 3;
            break;
        case 12:k = 5;
            break;
        case 13:k = 15;
            break;
        default: k = 15;
            break;
    }
    return k;
}

double abs(double x)
{
    if(x<0)
    {
        x*=-1;
    }
    return x;
}

double find_angle(double xx,double yy,double tx,double ty)
{
    int v,z;
    double q;
    q=0;
    v=(int)(abs(round(tx-xx)));
    if (v==0)
    {
        //{v=0.001;} commented out in original
        if ((tx==xx) && (ty>yy))
        {
            q = pi;
        }
        if ((tx==xx) && (ty<yy))
        {
            q = 0;
        }
    }
    else
    {
        z=abs(round(ty-yy));
        q=abs(atan(z/v));
        if ((tx>xx) && (ty>yy))
        {
            q=pi/2+q;
        }
        if ((tx>xx) && (ty<yy))
        {
            q = pi/2-q;
        }
        if ((tx<xx) && (ty<yy))
        {
            q=pi+pi/2+q;
        }
        if ((tx<xx) && (ty>yy))
        {
            q=pi+pi/2-q;
        }
        if ((tx=xx) && (ty>yy))
        {
            q=pi/2;
        }
        if ((tx=xx) && (ty<yy))
        {
            q=0;
        }
        if ((tx<xx) && (ty=yy))
        {
            q=pi+pi/2;
        }
        if ((tx>xx) && (ty=yy))
        {
            q=pi/2;
        }
    }
    return q;
}

void box(int x1,int y1,int x2,int y2)    //GUI PEOPLE (Draws a box at coordinates)
{


}

void hole(int x1,int y1,int x2,int y2)    //GUI PEOPLE (Don't know exactly what this does )
{
/*    i:integer;
    begin
    if not graphix then exit;
    if x2<x1 then begin i:=x1; x1:=x2; x2:=i; end;
    if y2<y1 then begin i:=y1; y1:=y2; y2:=i; end;
    setfillstyle(1,0);
    setcolor(0);
    bar(x1,y1,x2,y2);
    setcolor(8);
    line(x1,y1,x2-1,y1);
    line(x1,y1,x1,y2-1);
    setcolor(15);
    line(x1+1,y2,x2,y2);
    line(x2,y1+1,x2,y2);
    putpixel(x1,y2,7);
    putpixel(x2,y1,7);
    */
}

void chirp()        //GUI PEOPLE!!!! (sound)
{

}

void click()        //GUI PEOPLE!!!! (shooting/explosion sound)
{

}

int hex2int(string s)
{
    uint16_t w;
    int i,j;

    i = -1; w = 0;
    while (i<s.length())
    {
        i++;
        switch ((int)(s[i]))
        {
            case 48: w = (w << 4) | 0;
                break;
            case 49: w = (w << 4) | 1;
                break;
            case 50: w = (w << 4) | 2;
                break;
            case 51: w = (w << 4) | 3;
                break;
            case 52: w = (w << 4) | 4;
                break;
            case 53: w = (w << 4) | 5;
                break;
            case 54: w = (w << 4) | 6;
                break;
            case 55: w = (w << 4) | 7;
                break;
            case 56: w = (w << 4) | 8;
                break;
            case 57: w = (w << 4) | 9;
                break;
            case 65: w = (w << 4) | 10;
                break;
            case 66: w = (w << 4) | 11;
                break;
            case 67: w = (w << 4) | 12;
                break;
            case 68: w = (w << 4) | 13;
                break;
            case 69: w = (w << 4) | 14;
                break;
            case 70: w = (w << 4) | 15;
                break;
            default: i = s.length();
        }
    }

    return w;
}

int str2int(string s)
{
    long i,j,k;
    bool neg;
    neg = false;
    s = btrim(ucase(s));
    if (s == "")
    {
        k = 0;
    }
    else
    {
        if (s[1]='-')
        {
            neg = true;
            s = rstr(s,s.length()-1);
        }
        k = 0;
        if (lstr(s,2)=="0X")
        {
            k = hex2int(rstr(s,s.length()-2));
        }
        else if (rstr(s,1)=="H")
        {
            k = hex2int(lstr(s,s.length()-1));
        }
        else
        {
            k = value(s);
        }
        if (neg)
        {
            k = 0-k;
        }
    }
    return k;
}

int find_anglei(double xx,double yy,double tx,double ty)
{
    int i;
    i=round(find_angle(xx,yy,tx,ty)/pi*128+256);
    while (i<0)
    {
        i += 256;
    }
    i = i & 255;
    return i;
}

string bin(int n)
{
    int i;
    string bin_string;

    bin_string = "";
    for (i = 0; i <= 15; i++)
    {
        if ((n % 2) == 0)
        {
            bin_string =  '0' + bin_string;
        }
        else
        {
            bin_string =  '1' + bin_string;
        }

        n = n/2;
    }
    return bin_string;
}

string decimal(int num,int length )
{
    string dec_string;
    int i;
    dec_string = "";
    for (i = 1; i <=length; i++)
    {
        dec_string = (char)((num % 10)+48) + dec_string;
        num = num / 10;
    }
    return dec_string;
}

int main(void)
{
    return 0;
}
