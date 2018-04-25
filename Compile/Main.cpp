/*
Program ported from ATR2.pas and ATR2FUNC.pas
Written in c++ (gcc compiler)
Written in Spring of 2018
Contributors: Mark Yetter, Niccole Maiello, William Csont, Eric, Tyler
*/



#define _USE_MATH_DEFINES

#include <iostream>
#include <ctime>
#include <string>
#include <cmath>

#include <math.h>
#include <chrono>
#include <thread>
//#include <SDL.h>
//#include <SDL_image.h>
//#include <stdio.h>
//#include <SDL_mixer.h>
//#include "helloworld.cpp"
#include <fOpen.h>
//#include <SDL_ttf.h>
#include <vector>
#include <windows.h>
#include <sstream>
#include <ctime>
#define pi 3.1415926535897932384626433

using namespace std;

//------------------------------------------------ATR2FUNC HEADER---------------------------------------------------
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
double abso(double x);
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
string ltrim(string s1);
string rtrim(string s1);
string btrim(string s1);
string lstr(string s1, int l);
string rstr(string s1,int l);


//***GUI pointers/variables ***///GUI HAS BEEN COMMENTED OUT FOR DEBUGGING BECAUSE FUCK SDL
/*
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
int cycle = 0;

SDL_Rect shapeParam[TOTAL_R_BUTTONS];
SDL_Rect shapePos[TOTAL_R_BUTTONS];

//GUI definitions
bool initSDL(); //turn on SDL *important*
bool loadMedia(); //load images *IMPORTANT*
bool mouseHandle(SDL_Event* M);
*/

//------------------------------------------------------END ATR2FUNC HEADER-------------------------------------------------


void damage(int n,int d,bool physical);
void robot_error(int n,int i,string ov);
void do_robot(int n);
void prog_error(int n, string ss);
void execute_instruction(int n);
void init_missile(double xx, double yy, double xxv, double yyv,int dir,int s,int blast,bool ob);
string mnemonic(int n,int m);
string operand(int n,int m);
bool graph_check(int n);
bool invalid_microcode(int n,int ip);
int max_shown();
void update_debug_window();
void update_debug_system();
void update_debug_registers();
void update_debug_flags();
void update_debug_memory();
void update_debug_code();
void update_debug_window();
bool gameover();
int get_val(int n,int c,int o);
int get_from_ram(int n,int i,int j);
int find_label(int n,int l,int m);
void put_val(int n,int c,int o,int v);
int pop(int n);
void push(int n,int v);
void jump(int n,int o, bool inc_ip);
void call_int(int n,int int_num, int time_used);
void reset_software(int n);
void com_transmit(int n,int chan,int data);
int com_receive(int n);
int in_port(int n,int p);
int scan(int n);
void out_port(int n,int p,int v, int* time_used);

//ALL COUTS AND WRITES NEED TO BE DISCUSSED AND REPLACED WITH GUI STUFF


string progname       ="AT-Robots";
string version        ="2.11";
string cnotice1       ="Copyright 1997 ''99, Ed T. Toton III";
string cnotice2       ="All Rights Reserved.";
string cnotice3       ="Copyright 2014, William 'Amos' Confer";
string main_filename  ="ATR2";
string robot_ext      =".AT2";
string locked_ext     =".ATL";
string config_ext     =".ATS";
string compile_ext    =".CMP";
string report_ext     =".REP";

const bool _T = true;
const bool _F = false;
const int minint = -32768;
const int maxint = 32768;

 //DEBUGGING AND COMPILER
 const bool show_code = _F;
 const bool compile_by_line = _F;
 const int max_var_len = 16;
 const bool debugging_compiler = _F;

//ROBOTS
const int max_robots = 31;
const int max_code = 1023;
const int max_op = 3;
const int stack_size = 256;
const int stack_base = 768;
const int max_ram = 1023;
const int max_vars = 256;
const int max_labels = 256;
const int acceleration = 4;
const int turn_rate = 8;
const int max_vel = 4;
const int max_missiles = 1023;
const int missile_spd = 32;
const int hit_range = 14;
const int blast_radius = 25;
const int crash_range = 8;
const int max_sonar = 250;
const int com_queue = 512;
const int max_queue = 255;
const int max_config_points = 12;
const int max_mines = 63;
const int mine_blast = 35;

//SIMULATOR AND GRAPHICS - GUI people fix this if you need bc idk what these are
double screen_scale = 0.46;
double screen_x = 5;
double screen_y = 5;
int robot_scale = 6;
int default_delay = 20;
int default_slice = 05;
double mine_circle = trunc(mine_blast*screen_scale)+1;
double blast_circle = trunc(blast_radius*screen_scale)+1;
double mis_radius = trunc(hit_range/2)+1;
int max_robot_lines = 8;
// Gray50 : FillPatternType = ($AA, $55, $AA, $55, $AA, $55, $AA, $55); GUI can deal with this, I can't

struct op_rec
{
        int op[max_op];
};

struct prog_type
{
    op_rec code[max_code];
};

struct config_rec //CONFIG FOR EACH BOT
{
        int scanner,weapon,armor,engine,heatsinks,shield,mines;
};

struct mine_rec //MINES
{
        double x,y;
        int detect,yield;
        bool detonate;
};

struct robot_rec //ROBOTS
{
        bool is_locked;
        int mem_watch;
        double x,y,lx,ly,xv,yv,speed,shotstrength,damageadj,speedadj,meter;
        int hd,thd,lhd,spd,tspd,armor,larmor,heat,lheat,ip,plen,scanarc,accuracy,shift,err,delay_left,robot_time_limit, max_time,time_left,lshift,arc_count,sonar_count,scanrange,last_damage,last_hit,transponder,shutdown,channel,lendarc,endarc,lstartarc,startarc,mines;
        int tx,ltx,ty,lty;
        long wins,trials,kills,deaths,startkills,shots_fired,match_shots,hits,damage_total,cycles_lived,error_count;
        config_rec config;
        string name;
        string fn;
        bool shields_up,lshields,overburn,keepshift,cooling,won;
        prog_type code;
        int ram[max_ram];
        mine_rec mine[max_mines];
        string errorlog;
};

string parsetype[max_op];


struct robot_ptr
{
    robot_rec act;
    robot_rec * ptr = &act;
};


struct missile_rec
{
        double x,y,lx,ly,xv,yv,mult,mspd;
        int source, a, hd, rad, lrad, max_rad;
};

robot_rec robot[max_robots];


//ROBOT VARIABLES
int num_robots;
robot_ptr robots[max_robots + 2];
missile_rec missile[max_missiles];

//COMPILER VARIABLES
string f; //I believe this is the contents of the file
int numvars,numlabels,maxcode,lock_pos,lock_dat;
string varname[max_vars];
int varloc[max_vars];    //varloc:array[1..max_vars] of integer;
string labelname[max_vars];  //labelname:array[1..max_vars] of string[max_var_len];
int labelnum[max_labels];    //labelnum:array[1..max_labels] of integer;
bool show_source, compile_only;
string lock_code;

//SIMULATOR/GRAPHICS VARIABLES, I removed everything in {} assuming they were comments, look at them in the source code
bool bout_over;
int step_mode;
int temp_mode;
int step_count;
bool step_loop;
bool old_shields,insane_missiles,debug_info,windoze,no_gfx,logging_errors,timing,show_arcs;
int game_delay,time_slice,insanity,update_timer,max_gx,max_gy,stats_mode;
long game_limit,game_cycle;
long matches,played,executed;

//GENERAL SETTINGS
bool quit,report,show_cnotice;
int kill_count,report_type;


//-----------------------------------------------------ATR2FUNC FUNCTIONS------------------------------------------------
//***GUI FUNCTIONS START HERE **** //
//bool initSDL()
//{
	/* Desc: This fucntion initializes the SDL
		Libraries used in this program, as well as creating
		the window and surface we will use. Our audio is
		also initalized. *//*

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




	return success;*/
//}

//bool loadMedia() {
	/* Desc: This fucntion loads in the files needed to
		format the screen and sound effects. The buttons
		are clipped from a sprite sheet. The audio is loaded. *//*
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

	return success;*/
//}

//bool mouseHandle (/*SDL_Event* M*/) {
	/* Desc: this fucntion handles the position and actions
		of the mouse. The mouse is located, and on click a new button
		is shown to create a "flash" effect. A sound effect is
		also played. *//*

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

		return quitter;*/

//}

//bool mouseHandleS(/*SDL_Event* S, int cycle*/) {
/*  Desc: this fucntion handles the position and actions
		of the mouse. The mouse is located, and on click a new button
		is shown to create a "flash" effect. A sound effect is
		also played. *//*

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

*/
//}

//void startMenu() {
/*
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


*/
//}

//void selectMenu() {
    /*
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

		return;*/

//}

//void cycleShapes() {
/*
	cycle++;

	if (cycle > 9) {
		cycle = 0;

	}

	SDL_BlitSurface(shapes[cycle], &shapeParam[cycle], nWindowSurface, &shapePos[cycle]);*/

//}

//void closeStartMenu() {
/*
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
	gWindow = NULL;*/
//}

//void closeSelectMenu() {
/*

SDL_FreeSurface(nWindowSurface);

	for (int cnt = 0; cnt > TOTAL_R_BUTTONS; cnt++) {
		SDL_FreeSurface(selectRButton[cnt]);
		SDL_FreeSurface(selectRButtonClick[cnt]);
		SDL_FreeSurface(selectBG);

	}

	SDL_DestroyWindow( nWindow );
	nWindow = NULL;*/
//}

//void arena () {
    /*
	aWindow = SDL_CreateWindow("FIGHT!!!!!!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	aWindowSurface = SDL_GetWindowSurface(aWindow);

	SDL_BlitSurface(arena, NULL, aWindowSurface, &arenaPos);
	SDL_BlitSurface(robot1, NULL, aWindowSurface, &robPos[0]);
	SDL_BlitSurface(robot2, NULL, aWindowSurface,  &robPos[1]);
	SDL_UpdateWindowSurface(aWindow);

	//the assets for the arena are loaded in
	//just visuals, where do we get our info?

*/
//}


//void closeArena() {
    /*

	SDL_FreeSurface(robot1);
	SDL_FreeSurface(robot2);
	SDL_FreeSurface(arena);

	SDL_DestroyWindow( aWindow );
	aWindow = NULL;*/
//}

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
//***GUI FUNCTIONS END HERE **** //




string ltrim(string s1)
{
    int i;
    while ((s1.length()>0) && (s1[0]==' ') || ((int)(s1[0])==8) || ((int)(s1[0])==9))
    {
        s1 = s1.substr(1,s1.length()-1);
    }

    return s1;
}


string rtrim(string s1)
{
    int i;

    while ((s1.length()>0) && ((s1[s1.length()-1] == ' ') || ((int)(s1[s1.length()-1]) == 8)
         || (((int)(s1[s1.length()-1]) ==9))))
    {
        s1 = s1.substr(0,s1.length()-2);
    }
    return s1;
}

string btrim(string s1)
{
    return ltrim(rtrim(s1));
}

string lstr(string s1, int l)
{
    if (s1.length()<=l)
    {
        return s1;
    }
    else
    {
        return s1.substr(0,l);
    }
}

string rstr(string s1,int l)
{
    if (s1.length()<=l)
    {
        return s1;
    }
    else
    {
        return s1.substr(s1.length()-2+1,l);
    }
}

void textxy(int x,int y, string s)  //GUI PEOPLE NEED TO DO THIS (prints string to screen at cord. x & y)
{
/*    setfillstyle(1,0);
    bar(x,y,x+length(s)*8,y+7);
    outtextxy(x,y,s);
*/
}


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
 n = ~(~0U << 16) & ((n << 1) + (n & (1 << 15) >> 1)); //need to look back at this (copied from confer)
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
    n = ~(~0U << 16) & (((unsigned) n >> 1) + ((n & 1 << 0) << 15));
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

void flushkey()                 //{ Clears any key strokes in the key-  }
{                               //{ board buffer so a couple of key     }//{ presses don't race you through program. }
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
         //sleep_for(nanoseconds(10)); IM ASSUMING NANOSECONDS IS SOMETHING WITH TIME THAT DOESNT WORK
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

double abso(double x)
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
    v=(int)(abso(round(tx-xx)));
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
        z=abso(round(ty-yy));
        q=abso(atan(z/v));
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
//----------------------------------------------------END ATR2FUNC FUNCTIONS----------------------------------------------------------


//may not be needed depending on how we approach
string operand(int n, int m) //function that takes in two integers (n & m) returns string
{
   string s = to_string(n);
/*       (*
       Microcode:
          0 = instruction, number, constant
          1 = variable, memory access
          2 = :label
          3 = !label (unresolved)
          4 = !label (resolved)
         8h mask = inderect addressing (enclosed in [])
       *)*/
   switch(m & 7){ //this is a switch case for what was found in line 219
    case 1:
      s = "@"+s;
      break;
    case 2:
      s = ":"+s;
      break;
    case 3:
      s = "$"+s;
      break;
    case 4:
      s = "!"+s;
      break;
    default:
      s = to_string(n);
      break;
   }
   if ((m & 8)>0)
    s ="["+s+"]";
   return s; //returns what s is, not sure we will need this function
};

string mnemonic(int n, int m)
{
    string s = to_string(n); //another cstr call
    if (m == 0)
    {
        switch (n)
        { //this is going to be important when it comes to reading what the robots are coded to do
        case 0:
            s = "NOP";
            break;
        case 1:
            s = "ADD";
            break;
        case 2:
            s = "SUB";
            break;
        case 3:
            s = "OR";
            break;
        case 4:
            s = "AND";
            break;
        case 5:
            s = "XOR";
            break;
        case 6:
            s = "NOT";
            break;
        case 7:
            s = "MPY";
            break;
        case 8:
            s = "DIV";
            break;
        case 9:
            s = "MOD";
            break;
        case 10:
            s = "RET";
            break;
        case 11:
            s = "CALL";
            break;
        case 12:
            s = "JMP";
            break;
        case 13:
            s = "JLS";
            break;
        case 14:
            s = "JGR";
            break;
        case 15:
            s = "JNE";
            break;
        case 16:
            s = "JE";
            break;
        case 17:
            s = "SWAP";
            break;
        case 18:
            s = "DO";
            break;
        case 19:
            s = "LOOP";
            break;
        case 20:
            s = "CMP";
            break;
        case 21:
            s = "TEST";
            break;
        case 22:
            s = "MOV";
            break;
        case 23:
            s = "LOC";
            break;
        case 24:
            s = "GET";
            break;
        case 25:
            s = "PUT";
            break;
        case 26:
            s = "INT";
            break;
        case 27:
            s = "IPO";
            break;
        case 28:
            s = "OPO";
            break;
        case 29:
            s = "DELAY";
            break;
        case 30:
            s = "PUSH";
            break;
        case 31:
            s ="POP";
            break;
        case 32:
            s = "ERR";
            break;
        case 33:
            s = "INC";
            break;
        case 34:
            s = "DEC";
            break;
        case 35:
            s = "SHL";
            break;
        case 36:
            s = "SHR";
            break;
        case 37:
            s = "ROL";
            break;
        case 38:
            s = "ROR";
            break;
        case 39:
            s = "JZ";
            break;
        case 40:
            s = "JNZ";
            break;
        case 41:
            s = "JGE";
            break;
        case 42:
            s = "JLE";
            break;
        case 43:
            s = "SAL";
            break;
        case 44:
            s = "SAR";
            break;
        case 45:
            s = "NEG";
            break;
        case 46:
            s = "JTL";
            break;
        default:
            s = "XXX";
        }
    }
    else
        s = operand(n,m);
    return s;
};

//--------------------------WARNING: FUNCTION LOG_ERROR() IS FUBAR. DEBUG AT OWN RISK--------------------------

void log_error(int n, int i, string ov) //this can be done with throwing exceptions and errors
{
    int j, k;
    string s;
    if(!logging_errors)
    {
 //need robot[n]^ for this
        switch (i)
        {
        case 1:
            s = "Stack full - Too many CALLs?";
            break;
        case 2:
            s = "Label not found. Hmmm.";
            break;
        case 3:
            s = "Can''t assign value - Tisk tisk.";
            break;
        case 4:
            s = "Illegal memory reference";
            break;
        case 5:
            s = "Stack empty - Too many RETs?";
            break;
        case 6:
            s = "Illegal instruction. How bizarre.";
            break;
        case 7:
            s = "Return out of range - Woops!";
            break;
        case 8:
            s = "Divide by zero";
            break;
        case 9:
            s = "Unresolved !label. WTF?";
            break;
        case 10:
            s = "Invalid Interrupt Call";
            break;
        case 11:
            s = "Invalid Port Access";
            break;
        case 12:
            s = "Com Queue empty";
            break;
        case 13:
            s = "No mine-layer, silly.";
            break;
        case 14:
            s = "No mines left";
            break;
        case 15:
            s = "No shield installed - Arm the photon torpedoes instead. :-)";
            break;
        case 16:
            s = "Invalid Microcode in instruction.";
            break;
        default:
            s = "Unknown error";
        }
    }
    cout<<robot[n].errorlog<<'<'<<i<<'> '<<s<<' (Line #'<<robot[n].ip<<') [Cycle: '<<game_cycle<<', Match: '<<played<<'/'<<matches<<']'<<endl;
    cout<<robot[n].errorlog<<' '<<mnemonic(robot[n].code.code[robot[n].ip].op[0],robot[n].code.code[robot[n].ip].op[3] and 15)      //WE NEED TO GET THIS TO WORK
     ;//       <<'  '<<operand(robot[n].code.code[robot[n].ip].op[1],(robot[n].code.code[robot[n].ip].op[3] shr 4) and 15)<<', '<<operand(robot[n].code.code[robot[n].ip].op[2],(robot.code.code[robot[n].ip].op[3] shr 8) and 15)<<endl;
    if (ov!="")
    {
        cout<<robot[n].errorlog<<"    (Values: "<<ov<<")"<<endl;
    }
    else
    {
        cout<<robot[n].errorlog<<endl;
    }
    cout<<robot[n].errorlog<<" AX="<<addrear((robot[n].ram[65])+",",7)<<endl;
    cout<<robot[n].errorlog<<" BX="<<addrear((robot[n].ram[66])+",",7)<<endl;
    cout<<robot[n].errorlog<<" CX="<<addrear((robot[n].ram[67])+",",7)<<endl;
    cout<<robot[n].errorlog<<" DX="<<addrear((robot[n].ram[68])+",",7)<<endl;
    cout<<robot[n].errorlog<<" EX="<<addrear((robot[n].ram[69])+",",7)<<endl;
    cout<<robot[n].errorlog<<" FX="<<addrear((robot[n].ram[70])+",",7)<<endl;
    cout<<robot[n].errorlog<<" Flags="<<robot[n].ram[64]<<endl;
    cout<<robot[n].errorlog<<" AX="<<addrear(robot[n].ram[65]+",",7)<<endl;
    cout<<robot[n].errorlog<<" BX="<<addrear(robot[n].ram[66]+",",7)<<endl;
    cout<<robot[n].errorlog<<" CX="<<addrear(robot[n].ram[67]+",",7)<<endl;
    cout<<robot[n].errorlog<<" DX="<<addrear(robot[n].ram[68]+",",7)<<endl;
    cout<<robot[n].errorlog<<" EX="<<addrear(robot[n].ram[69]+",",7)<<endl;
    cout<<robot[n].errorlog<<" FX="<<addrear(robot[n].ram[70]+",",7)<<endl;
    cout<<robot[n].errorlog<<" Flags="<<robot[n].ram[64]<<endl;
    cout<<robot[n].errorlog<<endl;
};

int max_shown()
{
    switch(stats_mode)
    {
    case 1:
        return 12;
        break;
    case 2:
        return 32;
        break;
    default:
        return 6;
        break;
    }
};


bool graph_check(int n) //GUI
{
    bool ok = true;
    if ((!graphix) || (n<0) || (n>num_robots) || (n>=(int)max_shown))
        ok=false;
    return ok;
};


void robot_graph(int n) //GUI
{
    switch (stats_mode)
    {
    case 1:
        viewport(480,4+n*35,635,37+n*35);
        max_gx = 155;
        max_gy = 33; //all of the stats_mode below is graphics of visual stats
        break;
    case 2:
        viewport(480,4+n*13,635,15+n*13);
        max_gx = 155;
        max_gy = 11;
        break;
    default:
        viewport(480,4+n*70,635,70+n*70);
        max_gx = 155;
        max_gy = 66;
        break;
    }
    //setfillstyle(1,robot_color(n));
    //setcolor(robot_color(n));
};

/*
void update_armor(int n); //takes in variables to change the health based on what is happening to robot
begin
 if graph_check(n) and (step_mode<=0) then
 with robot[n]^ do
  begin
   robot_graph(n);
   if health>0 then
    case stats_mode of
     1:bar(30,13,29+armor,18);
     2:bar(88,03,87+(armor shr 2),08);
     else bar(30,25,29+armor,30);
    end;
   setfillstyle(1,8);
   ifhealth<100 then
    case stats_mode of
     1:bar(30+armor,13,129,18);
     2:bar(88+(armor shr 2),03,111,08);
     else bar(30+armor,25,129,30);
    end;
  end;
end;
*/
//{ ?
void update_heat(int n) //takes in variables to change the heat based on the action the robot is performing
{/*
    if (graph_check(n) && (step_mode<=0))
    {
        //with robot[n]^ do
        robot_graph(n);
        if (robot[n].heat>5)
        {
            switch (stats_mode)
            {
                case 1:bar(30,23,29+(heat / 5),28);
                break;
                case 2:bar(127,3,126+(heat / 20),8);
                break;
                default: bar(30,35,29+(heat / 5),40);
                break;
            }
        }
        setfillstyle(1,8);
        if (robot[n].heat<500)
        {
            switch (stats_mode)
            {
                case 1:bar(030+(heat / 5),23,129,28);
                break;
                case 2:bar(127+(heat / 20),3,151,8);
                break;
                default: bar(30+(heat / 5),35,129,40);
                break;
            }
        }
    }
    */
}

  void robot_error(int n,int i,string ov)
{
    if (graph_check(n) && (step_mode<=0))
    //with robot[n]. do
    {
        if (stats_mode==0)
        {
/*            robot_graph(n);
            setfillstyle(1,0);
            bar(66,56,154,64);
            setcolor(robot_color(n));
            outtextxy(66,56,addrear((string)(i),7)+hex(i));
            chirp;
*/
        }
        if (logging_errors)
        {
            log_error(n,i,ov);
        }
        //robot[n].error_count++; //WE STILL NEED TO MAKE ROBOT[N] WORK
    }
}

void update_lives(int n) //lives go down
{
    if ((graph_check(n)) && (stats_mode == 0) && (step_mode<=0))
    {
        /*
        with robot[n]^ do
        robot_graph(n);
        setcolor(robot_color(n)-8);
        setfillstyle(1,0);
        bar(011,46,130,53);
        outtextxy(011,46,'K:');
        outtextxy(029,46,zero_pad(kills,4));
        outtextxy(080,46,'D:');
        outtextxy(098,46,zero_pad(deaths,4));
        */
    }
}

  //changes the info of the current match on screen
void update_cycle_window()
{
    /*
    if (!graphix)
    {
        cout<<"Match "<<played<<"/"<<matches<<", Cycle: "<<zero_pad(game_cycle,9);
    }
    else
    {
        viewport(480,440,635,475);
        setfillstyle(1,0);
        bar(59,2,154,10);
        setcolor(7);
        outtextxy(75,03,zero_pad(game_cycle,9));
    }
    */
}


void setscreen() //draws the screeen and everything in it
{
   int i = 0;
   /*
   if (!graphix)
    exit();
   viewport(0,0,639,479);
   box(0,0,639,479);
   stats_mode = 0;
   case num_robots of
    0..5:stats_mode = 0;
    6..11:stats_mode = 1;
    12..max_robots:stats_mode = 2;
    else stats_mode = 0;
   end;
   {--main arena--}
   hole(4,4,475,475);
   {--cycle window--}
   viewport(480,430,635,475);
   hole(0,0,155,45);
   setcolor(7);
   outtextxy(3,03,'FreeMem: '+cstr(memavail));
   outtextxy(3,13,'Cycle:   ');
   outtextxy(3,23,'Limit:   '+zero_pad(game_limit,9));
   outtextxy(3,33,'Match:   '+cstr(played)+'/'+cstr(matches));
   update_cycle_window;
   {--robot windows--}
   for i = 0 to max_robots do
    if i<max_shown then with robot[i]^ do
     begin
      robot_graph(i);
      hole(0,0,max_gx,max_gy);
      if i<=num_robots then
       begin
        setcolor(robot_color(i));
        outtextxy(003,02,base_name(no_path(fn)));
        case stats_mode of //switch case used to differentiate robots and give them colors depending on their number
         1:begin
            outtextxy(003,12,' A:');
            outtextxy(003,22,' H:');
           end;
         2:begin
            setcolor(robot_color(i) and 7);
            outtextxy(080,02,'A');
            outtextxy(118,02,'H');
           end;
         else begin
            outtextxy(003,24,' A:');
            outtextxy(003,34,' H:');
           end;
        end;
        setcolor(robot_color(i));
        if stats_mode<=1 then
         begin
          outtextxy(080,02,'Wins:');
          outtextxy(122,02,zero_pad(wins,4));
         end;
        if stats_mode=0 then
         begin
          outtextxy(003,56,' Error:');
          setcolor(robot_color(i) and 7);
          outtextxy(003,12,name);
          setcolor(8);
          outtextxy(66,56,'None');
         end;
        lx = 1000-x; ly = 1000-y;
        update_armor(i); //updates the stats of the robot after they are placed
        update_heat(i);
        update_lives(i);
       end
      else
       begin
        setfillpattern(gray50,8);
        bar(1,1,max_gx-1,max_gy-1);
       end;
     end;
     */
}
//graphics again, used inbetween matches to clear the graph
void graph_mode(bool on)
{

/*    if (on && (!graphix))   //Turns on
    {
        Graph_VGA;
        cleardevice();
        setscreen();
        graphix = true;
    }
    else
    {
        if ((!on) && graphix)   //closes the GUI
        {
            closegraph();
            graphix = false;
        }
    }
*/
}
  void prog_error(int n, string ss)
 {
     string s;

     cout<<"Error #"<<n<<": ";
     switch(n)
     {
         case 0:s = ss;   //(*user error*)
         case 1:s = "Invalid :label - "+ss+", silly mortal.";
         case 2:s = "Undefined identifier - "+ss+". A typo perhaps?";
         case 3:s = "Memory access out of range - "+ss;
         case 4:s = "Not enough robots for combat. Maybe we should just drive in circles.";
         case 5:s = "Robot names and settings must be specified. An empty arena is no fun.";
         case 6:s = "Config file not found - "+ss;
         case 7:s = "Cannot access a config file from a config file - "+ss;
         case 8:s = "Robot not found "+ss+". Perhaps you mistyped it?";
         case 9:s = "Insufficient RAM to load robot: "+ss+"... This is not good.";
         case 10:s = "Too many robots! We can only handle 31! Blah.. limits are limits.";
         case 11:s = "You already have a perfectly good #def for "+ss+", silly.";
         case 12:s = "Variable name too long! (Max:16) "+ss;
         case 13:s = "!Label already defined "+ss+", silly.";
         case 14:s = "Too many variables! (Var Limit: 256)";
         case 15:s = "Too many !labels! (!Label Limit: 256)";
         case 16:s = "Robot program too long! Boldly we simplify, simplify along..."+ss;
         case 17:s = "!Label missing error. !Label #"+ss+".";
         case 18:s = "!Label out of range: "+ss;
         case 19:s = "!Label not found. "+ss;
         case 20:s = "Invalid config option: "+ss+". Inventing a new device?";
         case 21:s = "Robot is attempting to cheat; Too many config points ("+ss+")";
         case 22:s = "Insufficient data in data statement: "+ss;
         case 23:s = "Too many asterisks: "+ss;
         case 24:s = "Invalid step count: "+ss+". 1-9 are valid conditions.";
         case 25:s = "'"+ss+"'";
         default: s = ss;
     }
     cout<<s<<endl<<endl;
 }

void print_code(int n, int p) //this function is putting code into hexadecimal then printing it out, not sure why we would need it
{
  /*
   int i = 0;
     write(hex(p)+': ');
     for i = 0 to max_op do
      write(zero_pad(robot[n].code[p].op[i],5),' ');
     write(' =  ');
     for i = 0 to max_op do
      write(hex(robot[n].code[p].op[i]),'h ');
     writeln; writeln;
     */
}

char toUpper(char l)
{
    if (l > 96 && l < 123)
        l -= 32;

    return l;
}

void parse1(int n, int p, string s) //it seems that this procedure will be very important to look at, as it is dealing with the robot's code, we may be able to simplify it
{
    /*Something here doesn't want to work at all ever*//*


    int i,j,k,opcode,microcode;
    bool found, indirect;
    string ss;

    for(i = 0; i < (max_op - 1); i++)
    {
        k = 0;
        found = false;
        opcode = 0;
        microcode = 0;
        s[i] = toUpper(s[i]);
        s[i] = btrim(s[i]);
        indirect = false;*/
        /*
        (*
        Microcode:
            0 = instruction, number, constant
            1 = variable, memory access
            2 = :label
            3 = !label (unresolved)
            4 = !label (resolved)
            8h mask = inderect addressing (enclosed in [])
        *)
        *//*
        if (s[i]==0)
        {
            opcode = 0;
            microcode = 0;
            found = true;
        }

        if ((lstr(s[i],1)='[') && (rstr(s[i],1)=']'))
        {
            s[i] = s[i].substr(2,(s[i].length() - 2)); //You can't take a substring of a character. either s.substr(2,...) or s.substr(i,...)
            indirect = true;
        }

        if ((!found) && (s[i][1]='!'))
        {
            ss = s[i];
            ss = btrim(rstr(ss,ss.length()-1));
            if (numlabels>0)
            {
                for (j = 1; j < numlabels; j++)
                {
                    if (ss == labelname[j])
                    {
                        found = true;
                        if (labelnum[j]>=0)
                        {
                            opcode = labelnum[j];
                            microcode = 4;
                        }
                        else
                        {
                            opcode = j;
                            microcode = 3;
                        }
                    }
                }
            }
            if (!found)
            {
                numlabels++;
                if (numlabels>max_labels)
                    prog_error(15,0);
                else
                {
                    labelname[numlabels] = ss;
                    labelnum[numlabels] = -1;
                    opcode = numlabels;
                    microcode = 3;
                    found = true;
                }
            }
        }

    //variables
    if ((numvars>0) && (!found))
    {
        for (j = 1; j < numvars; j++)
        {
            if (s[i]=varname[j])
            {
                opcode = varloc[j];
                microcode = 1;
                found = true;
            }
        }
    //instructions
    if (s[i] == "NOP")
    {
        opcode = 0;
        found = true;
    }
    else if(s[i] == ("ADD"))
    {
        opcode = 1;
        found = true;
    }
    else if(s[i] == ("SUB"))
    {
        opcode = 2;
        found = true;
    }
    else if(s[i] == ("OR"))
    {
        opcode = 3;
        found = true;
    }
    else if(s[i] == ("AND"))
    {
        opcode = 4;
        found = true;
    }
    else if(s[i] == ("XOR"))
    {
        opcode = 5;
        found = true;
    }
    else if(s[i] == ("NOT"))
    {
        opcode = 6;
        found = true;
    }
    else if(s[i] == ("MPY"))
    {
        opcode = 7;
        found = true;
    }
    else if(s[i] == ("DIV"))
    {
        opcode = 8;
        found = true;
    }
    else if(s[i] == ("MOD"))
    {
        opcode = 9;
        found = true;
    }
    else if(s[i] == ("RET") || s[i] == ("RETURN"))
    {
        opcode = 10;
        found = true;
    }
    else if(s[i] == ("GSB") || s[i] == ("GOSUB") || s[i] == ("CALL"))
    {
        opcode = 11;
        found = true;
    }
    else if(s[i] == ("JMP") || s[i] == ("JUMP") || s[i] == ("GOTO"))
    {
        opcode = 12;
        found = true;
    }
    else if(s[i] == ("JLS") || s[i] == ("JB"))
    {
        opcode = 13;
        found = true;
    }
    else if(s[i] == ("JGR") || s[i] == ("JA"))
    {
        opcode = 14;
        found = true;
    }
    else if(s[i] == ("JNE"))
    {
        opcode = 15;
        found = true;
    }
    else if(s[i] == ("JEQ") || s[i] == ("JE"))
    {
        opcode = 16;
        found = true;
    }
    else if(s[i] == ("XCHG") || s[i] == ("SWAP"))
    {
        opcode = 17;
        found = true;
    }
    else if(s[i] == ("DO"))
    {
        opcode = 18;
        found = true;
    }
    else if(s[i] == ("LOOP"))
    {
        opcode = 19;
        found = true;
    }
    else if(s[i] == ("CMP"))
    {
        opcode = 20;
        found = true;
    }
    else if(s[i] == ("TEST"))
    {
        opcode = 21;
        found = true;
    }
    else if(s[i] == ("SET") || s[i] == ("MOV"))
    {
        opcode = 22;
        found = true;
    }
    else if(s[i] == ("LOC") || s[i] == ("ADDR"))
    {
        opcode = 23;
        found = true;
    }
    else if(s[i] == ("GET"))
    {
        opcode = 24;
        found = true;
    }
    else if(s[i] == ("PUT"))
    {
        opcode = 25;
        found = true;
    }
    else if(s[i] == ("INT"))
    {
        opcode = 26;
        found = true;
    }
    else if(s[i] == ("IPO") || s[i] == ("IN"))
    {
        opcode = 27;
        found = true;
    }
    else if(s[i] == ("OPO") || s[i] == ("OUT"))
    {
        opcode = 28;
        found = true;
    }
    else if(s[i] == ("DEL") || s[i] == ("DELAY"))
    {
        opcode = 29;
        found = true;
    }
    else if(s[i] == ("PUSH"))
    {
        opcode = 30;
        found = true;
    }
    else if(s[i] == ("POP"))
    {
        opcode = 31;
        found = true;
    }
    else if(s[i] == ("ERR") || s[i] == ("ERROR"))
    {
        opcode = 32;
        found = true;
    }
    else if(s[i] == ("INC"))
    {
        opcode = 33;
        found = true;
    }
    else if(s[i] == ("DEC"))
    {
        opcode = 34;
        found = true;
    }
    else if(s[i] == ("SHL"))
    {
        opcode = 35;
        found = true;
    }
    else if(s[i] == ("SHR"))
    {
        opcode = 36;
        found = true;
    }
    else if(s[i] == ("ROL"))
    {
        opcode = 37;
        found = true;
    }
    else if(s[i] == ("ROR"))
    {
        opcode = 38;
        found = true;
    }
    else if(s[i] == ("JZ"))
    {
        opcode = 39;
        found = true;
    }
    else if(s[i] == ("JNZ"))
    {
        opcode = 40;
        found = true;
    }
    else if(s[i] == ("JAE") || s[i] == ("JGE"))
    {
        opcode = 41;
        found = true;
    }
    else if(s[i] == ("JLE") || s[i] == ("JBE"))
    {
        opcode = 42;
        found = true;
    }
    else if(s[i] == ("SAL"))
    {
        opcode = 43;
        found = true;
    }
    else if(s[i] == ("SAR"))
    {
        opcode = 44;
        found = true;
    }
    else if(s[i] == ("NEG"))
    {
        opcode = 45;
        found = true;
    }
    else if(s[i] == ("JTL"))
    {
        opcode = 46;
        found = true;
    }
    else
    {
        //error proofing
    }

//registers
// can check if the code was already found
    if(!found)
    {
        if(s[i] == ("COLCNT"))
        {
            opcode = 8;
            microcode = 1;
            found = true;
        }
        else if(s[i] == ("METERS"))
        {
            opcode = 9;
            microcode = 1;
            found = true;
        }
        else if(s[i] == ("COMBASE"))
        {
            opcode = 10;
            microcode = 1;
            found = true;
        }
        else if(s[i] == ("COMEND"))
        {
            opcode = 11;
            microcode = 1;
            found = true;
        }
        else if(s[i] == ("FLAGS"))
        {
            opcode = 64;
            microcode = 1;
            found = true;
        }
        else if(s[i] == ("AX"))
        {
            opcode = 65;
            microcode = 1;
            found = true;
        }
        else if(s[i] == ("BX"))
        {
            opcode = 66;
            microcode = 1;
            found = true;
        }
        else if(s[i] == ("CX"))
        {
            opcode = 67;
            microcode = 1;
            found = true;
        }
        else if(s[i] == ("DX"))
        {
            opcode = 68;
            microcode = 1;
            found = true;
        }
        else if(s[i] == ("EX"))
        {
            opcode = 69;
            microcode = 1;
            found = true;
        }
        else if(s[i] == ("FX"))
        {
            opcode = 70;
            microcode = 1;
            found = true;
        }
        else if(s[i] == ("SP"))
        {
            opcode = 71;
            microcode = 1;
        }
        else
        {
            //error proofing - if needed
        }
    }

    //constants
    if(!found)
    {
        if(s[i] == ("MAXINT"))
        {
        opcode = 32767;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("MININT"))
        {
        opcode = 32768;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_SPEDOMETER"))
        {
        opcode = 1;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_HEAT"))
        {
        opcode = 2;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_COMPASS"))
        {
        opcode = 3;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_TANGLE") || s[i] == ("P_TURRET_OFS"))
        {
        opcode = 4;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_THEADING") || s[i] == ("P_TURRET_ABS"))
        {
        opcode = 5;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_ARMOR") || s[i] == ("P_DAMAGE"))
        {
        opcode = 6;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_SCAN"))
        {
        opcode = 7;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_ACCURACY"))
        {
        opcode = 8;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_RADAR"))
        {
        opcode = 9;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_RANDOM") || s[i] == ("P_RAND"))
        {
        opcode = 10;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_THROTTLE"))
        {
        opcode = 11;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_TROTATE") || s[i] == ("P_OFS_TURRET"))
        {
        opcode = 12;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_TAIM") || s[i] == ("P_ABS_TURRET"))
        {
        opcode = 13;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_STEERING"))
        {
        opcode = 14;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_WEAP") || s[i] == ("P_WEAPON") || s[i] == ("P_FIRE"))
        {
        opcode = 15;
        microcode = 0;
        found = true;
        }
        else if(s[i] == ("P_SONAR"))
        {
        opcode = 16;
        microcode = 0;
        found = true;
          }
          else if(s[i] == ("P_ARC") || s[i] == ("P_SCANARC"))
          {
            opcode = 17;
            microcode = 0;
            found = true;
          }
          else if(s[i] == ("P_OVERBURN"))
          {
            opcode = 18;
            microcode = 0;
            found = true;
          }
          else if(s[i] == ("P_TRANSPONDER"))
          {
            opcode = 19;
            microcode = 0;
            found = true;
          }
          else if(s[i] == ("P_SHUTDOWN"))
          {
            opcode = 20;
            microcode = 0;
            found = true;
          }
          else if(s[i] == ("P_CHANNEL"))
          {
            opcode = 21;
            microcode = 0;
            found = true;
          }
        else if(s[i] == ("P_MINELAYER"))
        {
            opcode = 22;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("P_MINETRIGGER"))
        {
            opcode = 23;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("P_SHIELD") || s[i] == ("P_SHIELDS"))
        {
            opcode = 24;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_DESTRUCT"))
        {
            opcode = 0;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_RESET"))
        {
            opcode = 1;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_LOCATE"))
        {
            opcode = 2;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_KEEPSHIFT"))
        {
            opcode = 3;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_OVERBURN"))
        {
            opcode = 4;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_ID"))
        {
            opcode = 5;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_TIMER"))
        {
            opcode = 6;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_ANGLE"))
        {
            opcode = 7;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_TID") || s[i] == ("I_TARGETID"))
        {
            opcode = 8;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_TINFO") || s[i] == ("I_TARGETINFO"))
        {
            opcode = 9;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_GINFO") || s[i] == ("I_GAMEINFO"))
        {
            opcode = 10;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_RINFO") || s[i] == ("I_ROBOTINFO"))
        {
            opcode = 11;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_COLLISIONS"))
        {
            opcode = 12;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_RESETCOLCNT"))
        {
            opcode = 13;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_TRANSMIT"))
        {
            opcode = 14;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_RECEIVE"))
        {
            opcode = 15;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_DATAREADY"))
        {
            opcode = 16;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_CLEARCOM"))
        {
            opcode = 17;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_KILLS") || s[i] == ("I_DEATHS"))
        {
            opcode = 18;
            microcode = 0;
            found = true;
        }
        else if(s[i] == ("I_CLEARMETERS"))
        {
            opcode = 19;
            microcode = 0;
            found = true;
        }
        }
    }

//memory addresses
    if ((!found) && (s[i][1]='@') && ((s[i][2] > '0') && (s[i][2] < '9')))
     {
      opcode = (int)rstr(s[i],s[i].length()-1);
      if ((opcode<0) || (opcode>(max_ram+1)+(((max_code+1) << 3)-1))) then
         prog_error(3,s[i]);
      microcode = 1;
      found = true;
    }


    if (!found) && (s[i][1] > '0' && s[i][1] < 9 || s[i][1] == "-") //this is if the code has a number to indicate what they want to change something to
    {
      opcode = (int)(s[i]);
      found = true;
    }

    if (found)
    {
      code[p].op[i] = opcode;
      if (indirect)
        microcode = microcode | 8;
      code[p].op[max_op] = code[p].op[max_op] | (microcode << (i*4));
    }
    else if (s[i] != 0)
      prog_error(2,s[i]);
    if (show_code)
      print_code(n,p);
    if (compile_by_line)
      readkey();*/
//}
};

void check_plen(int plen) //will need this if we have a max amount of code
{/*                     //Check your arguments. I don't know what to drop and what to pass
if (plen > maxcode)
prog_error(16,hex(13),hex(10)+"Maximum program length exceeded, (Limit: "+ cstr(maxcode+1)+" compiled lines)");*/
}

/*
procedure compile(n:integer;filename:string); //may need this to compile the robots, but we may not have to if we make them their own classes
var
pp:parsetype;
s,s1,s2,s3,orig_s,msg:string;
i,j,k,l,linecount,mask,locktype:integer;
ss:array[0..max_op] of string[16];
c,lc:char;
begin
lock_code = '';
lock_pos = 0;
locktype = 0;
lock_dat = 0;
if not exist(filename) then prog_error(8,filename);
textcolor(robot_color(n));
writeln('Compiling robot #',n+1,': ',filename);
with robot[n]^ do
begin
{FIFI}
is_locked = false; {assume unlocked robot}
{/FIFI}
textcolor(robot_color(n));
numvars = 0;
numlabels = 0;
for k = 0 to max_code do
for i = 0 to max_op do
code[k].op[i] = 0;
plen = 0;
assign(f,filename);
reset(f);
s = '';
linecount = 0;

{--first pass, compile--}
while (not eof(f)) and (s<>'#END') {and (plen<=maxcode)} do
begin
readln(f,s);
inc(linecount);
if locktype<3 then lock_pos = 0;
if lock_code<>'' then
 for i = 1 to length(s) do
  begin
   inc(lock_pos); if lock_pos>length (lock_code) then lock_pos = 1;
   case locktype of
    3:s[i] = char((ord(s[i])-1) xor (ord(lock_code[lock_pos]) xor lock_dat));
    2:s[i] = char(ord(s[i]) xor (ord(lock_code[lock_pos]) xor 1));
    else s[i] = char(ord(s[i]) xor ord(lock_code[lock_pos]));
   end;
   lock_dat = ord(s[i]) and 15;
  end;
s = btrim(s);
orig_s = s;
for i = 1 to length(s) do
 if s[i] in [#0..#32,',',#128..#255] then s[i] = ' ';
if show_source and ((lock_code='') or debugging_compiler) then
   writeln(zero_pad(linecount,3)+':'+zero_pad(plen,3)+' ',s);
if debugging_compiler then
   begin if readkey=#27 then halt; end;
{-remove comments-}
k = 0;
for i = length(s) downto 1 do
    if s[i]=';' then k = i;
if k>0 then s = lstr(s,k-1);
s = btrim(ucase(s));
for i = 0 to max_op do pp[i] = '';
if (length(s)>0) and (s[1]<>';') then
 begin
  case s[1] of
   '#':begin (*  Compiler Directives  *)
        s1 = ucase(btrim(rstr(s,length(s)-1)));
        msg = btrim(rstr(orig_s,length(orig_s)-5));
        k = 0;
        for i = 1 to length(s1) do
         if (k=0) and (s1[i]=' ') then k = i;
        dec(k);
        if k>1 then
         begin
          s2 = lstr(s1,k);
          s3 = ucase(btrim(rstr(s1,length(s1)-k)));
          k = 0;
          if numvars>0 then
           for i = 1 to numvars do
            if s3=varname[i] then k = i;
          if (s2='DEF') and (numvars<max_vars) then
           begin
            if length(s3)>max_var_len then prog_error(12,s3)
             else
            if k>0 then prog_error(11,s3)
             else
              begin
               inc(numvars);
               if numvars>max_vars then prog_error(14,'')
                else begin
                      varname[numvars] = s3;
                      varloc [numvars] = 127+numvars;
                     end;
              end;
           end
          else if (lstr(s2,4)='LOCK') then
           begin
            {FIFI}
            is_locked = true; {this robot is locked}
            {/FIFI}
            if length(s2)>4 then locktype = value(rstr(s2,length(s2)-4));
            lock_code = btrim(ucase(s3));
            writeln('Robot is of LOCKed format from this point forward. [',locktype,']');
            {writeln('Using key: "',lock_code,'"');}
            for i = 1 to length(lock_code) do
             lock_code[i] = char(ord(lock_code[i])-65);
           end
          else if (s2='MSG') then name = msg
          else if (s2='TIME') then
           begin
            robot_time_limit = value(s3);
            if robot_time_limit<0 then robot_time_limit = 0;
           end
          else if (s2='CONFIG') then
           begin
            if (lstr(s3,8)='SCANNER=') then config.scanner =
                value(rstr(s3,length(s3)-8))
            else if (lstr(s3,7)='SHIELD=') then config.shield =
                    value(rstr(s3,length(s3)-7))
            else if (lstr(s3,7)='WEAPON=') then config.weapon =
                    value(rstr(s3,length(s3)-7))
            else if (lstr(s3,6)='ARMOR=') then config.health =
                    value(rstr(s3,length(s3)-6))
            else if (lstr(s3,7)='ENGINE=') then config.engine =
                    value(rstr(s3,length(s3)-7))
            else if (lstr(s3,10)='HEATSINKS=') then config.heatsinks =
                    value(rstr(s3,length(s3)-10))
            else if (lstr(s3,6)='MINES=') then config.mines =
                    value(rstr(s3,length(s3)-6))
            else prog_error(20,s3);
            with config do
              begin
                if scanner<0 then scanner = 0; if scanner>5 then scanner = 5;
                if shield<0 then shield = 0;   if shield>5 then shield = 5;
                if weapon<0 then weapon = 0;   if weapon>5 then weapon = 5;
                ifhealth<0 thenhealth = 0;     ifhealth>5 thenhealth = 5;
                if engine<0 then engine = 0;   if engine>5 then engine = 5;
                if heatsinks<0 then heatsinks = 0; if heatsinks>5 then heatsinks = 5;
                if mines<0 then mines = 0;     if mines>5 then mines = 5;
              end;
           end
          else writeln('Warning: unknown directive "'+s2+'"');
         end;
       end;
   '*':begin  (*  Inline Pre-Compiled Machine Code  *)
        check_plen(plen);
        for i = 0 to max_op do pp[i] = '';
        for i = 2 to length(s) do
         if s[i]='*' then prog_error(23,s);
        k = 0; i = 1; s1 = '';
        if length(s)<=2 then prog_error(22,s);
        while (i<length(s)) and (k<=max_op) do
         begin
          inc(i);
          if ord(s[i]) in [33..41,43..127] then pp[k] = pp[k]+s[i]
            else if (ord(s[i]) in [0..32,128..255]) and
                    (ord(s[i-1]) in [33..41,43..127]) then inc(k);
         end;
        for i = 0 to max_op do
         code[plen].op[i] = str2int(pp[i]);
        inc(plen);
       end;
   ':':begin  (*  :labels  *)
        check_plen(plen);
        s1 = rstr(s,length(s)-1);
        for i = 1 to length(s1) do
         if not (s1[i] in ['0'..'9']) then
            prog_error(1,s);
        code[plen].op[0] = str2int(s1);
        code[plen].op[max_op] = 2;
        if show_code then print_code(n,plen);
        inc(plen);
       end;
   '!':begin (*  !labels  *)
        check_plen(plen);
        s1 = btrim(rstr(s,length(s)-1));
        k = 0;
        for i = length(s1) downto 1 do
            if s1[i] in [';',#8,#9,#10,' ',','] then k = i;
        if k>0 then s1 = lstr(s1,k-1);
        k = 0;
        for i = 1 to numlabels do
         if (labelname[i]=s1) then
          begin
           if (labelnum[i]>=0) then prog_error(13,'"!'+s1+'" ('+cstr(labelnum[i])+')');
           k = i;
          end;
        if (k=0) then
         begin
          inc(numlabels);
          if numlabels>max_labels then prog_error(15,'');
          k = numlabels;
         end;
        labelname[k] = s1;
        labelnum [k] = plen;
       end;
   else begin  (*  Instructions/Numbers  *)
         check_plen(plen);
         {-parse instruction-}
         {-remove comments-}
         k = 0;
         for i = length(s) downto 1 do
             if s[i]=';' then k = i;
         if k>0 then s = lstr(s,k-1);
         {-setup variables for parsing-}
         k = 0; for j = 0 to max_op do pp[j] = '';
         for j = 1 to length(s) do
          begin
           c = s[j];
           if (not (c in [' ',#8,#9,#10,','])) and (k<=max_op) then pp[k] = pp[k]+c
              else if not (lc in [' ',#8,#9,#10,',']) then k = k+1;
           lc = c;
          end;
         parse1(n,plen,pp);
         inc(plen);
        end;
  end;
 end;
end;
close(f);
*/
/*{-Add our implied NOP if there's room. This was originally to make sure
no one tries using an empty robot program, kinda pointless otherwise-}*/
/*
if plen<=maxcode then
begin
for i = 0 to max_op do pp[i] = '';
pp[0] = 'NOP';
parse1(n,plen,pp);
end
else
dec(plen);


{--second pass, resolving !labels--}
if numlabels>0 then
for i = 0 to plen do
for j = 0 to max_op-1 do
 if code[i].op[max_op] shr (j*4)=3 {unresolved !label} then
  begin
   k = code[i].op[j];
   if (k>0) and (k<=numlabels) then
    begin
     l = labelnum[k];
     if (l<0) then prog_error(19,'"!'+labelname[k]+'" ('+cstr(l)+')');
     if (l<0) or (l>maxcode) then prog_error(18,'"!'+labelname[k]+'" ('+cstr(l)+')')
      else
       begin
        code[i].op[j] = l;
        mask = not($F shl (j*4));
        code[i].op[max_op] = (code[i].op[max_op] and mask) or (4 shl (j*4));
                             {resolved !label}
       end;
    end
   else prog_error(17,cstr(k));
  end;
end;
textcolor(7);
end;
*/

void robot_config(int n)
{
    int i,j,k;
    //with robot[n]^ do
    switch(robot[n].config.scanner){ //configures the scan range
    case 5:
        robot[n].scanrange = 1500;
        break;
    case 4:
        robot[n].scanrange = 1000;
        break;
    case 3:
        robot[n].scanrange = 700;
        break;
    case 2:
    robot[n].scanrange = 500;
    break;
  case 1:
    robot[n].scanrange = 350;
    break;
  default:
    robot[n].scanrange = 250;
    break;
}
switch(robot[n].config.scanner){
  case 5:
    robot[n].shotstrength = 1.5;
    break;
  case 4:
    robot[n].shotstrength = 1.35;
    break;
  case 3:
    robot[n].shotstrength = 1.2;
    break;
  case 2:
    robot[n].shotstrength = 1;
    break;
  case 1:
    robot[n].shotstrength = 0.8;
    break;
  default:
    robot[n].shotstrength = 0.5;
    break;
}
switch(robot[n].config.armor){
  case 5:
    robot[n].damageadj = 0.66;
    robot[n].speedadj = 0.66;
    break;
  case 4:
    robot[n].damageadj = 0.77;
    robot[n].speedadj = 0.75;
    break;
  case 3:
    robot[n].damageadj = 0.83;
    robot[n].speedadj = 0.85;
    break;
  case 2:
    robot[n].damageadj = 1;
    robot[n].speedadj = 1;
    break;
  case 1:
    robot[n].damageadj = 1.5;
    robot[n].speedadj = 1.2;
    break;
  default:
    robot[n].damageadj = 2;
    robot[n].speedadj = 1.33;
    break;
}
switch(robot[n].config.engine){
  case 5:
    robot[n].speedadj = robot[n].speedadj * 1.5;
    break;
  case 4:
    robot[n].speedadj = robot[n].speedadj * 1.35;
    break;
  case 3:
    robot[n].speedadj = robot[n].speedadj * 1.2;
    break;
  case 2:
    robot[n].speedadj = robot[n].speedadj * 1;
    break;
  case 1:
    robot[n].speedadj = robot[n].speedadj * 0.8;
    break;
  default:
    robot[n].speedadj = robot[n].speedadj * 0.5;
    break;
}
switch(robot[n].config.mines){
  case 5:
    robot[n].mines = 24;
    break;
  case 4:
    robot[n].mines = 16;
    break;
  case 3:
    robot[n].mines = 10;
    break;
  case 2:
    robot[n].mines = 6;
    break;
  case 1:
    robot[n].mines = 4;
    break;
  default:
    robot[n].mines = 2;
    //config.mines = 0; not entirely sure what this is
    break;
  }
  robot[n].shields_up = false;

  if((robot[n].config.shield < 3) || (robot[n].config.shield > 5))
    robot[n].config.shield = 0;
  if((robot[n].config.heatsinks < 0) || (robot[n].config.heatsinks > 5)) //code states that heatsingks are handled separately
    robot[n].config.heatsinks = 0; //will come back to*/
}

void reset_software(int n)
{
    int i;

    //with robot[n]. do
    for (i = 0; i < max_ram;i++)
    {
        robot[n].ram[i] = 0;
    }
    robot[n].ram[71] = 768;
    robot[n].thd = robot[n].hd;
    robot[n].tspd = 0;
    robot[n].scanarc = 8;
    robot[n].shift = 0;
    robot[n].err = 0;
    robot[n].overburn = false;
    robot[n].keepshift = false;
    robot[n].ip = 0;
    robot[n].accuracy = 0;
    robot[n].meter = 0;
    robot[n].delay_left = 0;
    robot[n].time_left = 0;
    robot[n].shields_up = false;
}

void reset_hardware(int n)
{
  int i;
  double d, dd;
  //with robot[n]^ do
    robot[n].ltx = 0;   //USED TO BE A FOR LOOP, BUT THESE ARE INTS NOT ARRAYS
    robot[n].tx = 0;
    robot[n].lty = 0;
    robot[n].ty = 0;
  do{
    robot[n].x = rand() % 1000;
    robot[n].y = rand() % 1000;
    dd = 1000;
    for (i = 0; i < num_robots; i++)
    {
      if (robot[i].x < 0)
        robot[i].x = 0;
      if (robot[i].x > 1000)
        robot[i].x = 1000;
      if (robot[i].y < 0)
        robot[i].y = 0;
      if (robot[i].y > 1000)
        robot[i].y = 1000;
      d = distance(robot[n].x,robot[n].y,robot[i].x,robot[i].y);
      if ((robot[i].armor > 0) && (i != n) && (d < dd))
        dd = d;
    }
  }
  while (dd > 32);
  for (i = 0; i < max_mines; i++)
  {
    robot[n].mine[i].x = -1;
    robot[n].mine[i].y = -1;
    robot[n].mine[i].yield = 0;
    robot[n].mine[i].detonate = false;
    robot[n].mine[i].detect = 0;
  }
  robot[n].lx = -1;
  robot[n].ly = -1;
  robot[n].hd=rand()%256;
  robot[n].shift = 0;
  robot[n].lhd = robot[n].hd+1;
  robot[n].lshift = robot[n].shift+1;
  robot[n].spd = 0;
  robot[n].speed = 0;
  robot[n].cooling = false;
  robot[n].armor = 100;
  robot[n].larmor = 0;
  robot[n].heat = 0;
  robot[n].lheat = 1;
  robot[n].match_shots = 0;
  robot[n].won = false;
  robot[n].last_damage = 0;
  robot[n].last_hit = 0;
  robot[n].transponder = n+1;
  robot[n].meter = 0;
  robot[n].shutdown = 400;
  robot[n].shields_up = false;
  robot[n].channel = robot[n].transponder;
  robot[n].startkills = robot[n].kills;
  robot_config(n);
}

void init_robot(int n)
{
    int i,j,k,l;

    //with robot[n]. do

    robot[n].wins = 0;
    robot[n].trials = 0;
    robot[n].kills = 0;
    robot[n].deaths = 0;
    robot[n].shots_fired = 0;
    robot[n].match_shots = 0;
    robot[n].hits = 0;
    robot[n].damage_total = 0;
    robot[n].cycles_lived = 0;
    robot[n].error_count = 0;
    robot[n].plen = 0;
    robot[n].max_time = 0;
    robot[n].name = "";
    robot[n].fn = "";
    robot[n].speed = 0;
    robot[n].arc_count = 0;
    robot[n].sonar_count = 0;
    robot[n].robot_time_limit = 0;
    robot[n].scanrange = 1500;
    robot[n].shotstrength = 1;
    robot[n].damageadj = 1;
    robot[n].speedadj = 1;
    robot[n].mines = 0;
    //with config do

    robot[n].config.scanner = 5;
    robot[n].config.weapon = 2;
    robot[n].config.armor = 2;
    robot[n].config.engine = 2;
    robot[n].config.heatsinks = 1;
    robot[n].config.shield = 0;

    for (i = 0; i<max_ram; i++)
    {
        robot[n].ram[i] = 0;
    }
    robot[n].ram[71]  =  768;
    for (i = 0; i<max_code; i++)
    {
        for (k = 0; k < max_op; k++)
        {
            robot[n].code.code[i].op[k] = 0;
        }
    }

   reset_hardware(n);
   reset_software(n);
}


void create_robot(int n, string filename)
{
  int i,j,k;
  int maxavail = 999999; //Wasn't declared, so I made it big
  if (maxavail < sizeof(struct robot_rec))
    prog_error(9, filename);
//    robot[n] = new robot_rec;     //WE SHOULDNT NEED THIS
    //with robot[n]^ do
    init_robot(n);
    if (filename[1]='?')
      filename = filename + locked_ext;
    else
      filename = filename + robot_ext;
    if (filename[1]='?')
        filename = rstr(filename,(filename.length()-1));
    robot[n].fn = filename;
    robot_config(n);
//    compile(n,filename);              //NIKKI MIGHT BE DUMB
    k = robot[n].config.scanner+robot[n].config.armor+robot[n].config.weapon+robot[n].config.engine+robot[n].config.heatsinks+robot[n].config.shield+robot[n].config.mines;
    if (k > max_config_points)
      prog_error(21,(k)+"/"+(max_config_points));
}

void shutdown()
{
    int i,j,k;
    graph_mode(false);
    if (show_cnotice)
    {
//        textcolor(3);
        cout<<progname<<" "<<version<<" ";
        cout<<cnotice1<<endl;
        cout<<cnotice2<<endl;
        cout<<cnotice3<<endl;
    }
//    textcolor(7);
    if (!registered)
    {
//        textcolor(4);
        cout<<"Unregistered version"<<endl;
    }
    else
    {
        cout<<"Registered to: "<<reg_name<<endl;
    }
//    textcolor(7);
    cout<<endl;
    if (logging_errors)
    {
        for (i = 0; i < num_robots; i++)
        {
            //with robot[i]^ do
//            cout<<"Robot error-log created: "<<(base_name(robot[i].fn)+".ERR")<<endl; //FUNCTION BASE_NAME DOES NOT EXIST

        }

    }
    string wait;
    cout<<"Enter any key to continue: ";
    cin>>wait;
}
/*
procedure delete_compile_report;
begin
if exist(main_filename+compile_ext) then
delete_file(main_filename+compile_ext);
end;
*//*
procedure write_compile_report;
var
f:text;
i,j,k:integer;
begin
assign(f,main_filename+compile_ext);
rewrite(f);
writeln(f,num_robots+1);
for i = 0 to num_robots do
with robot[i]^ do
writeln(f,fn);
close(f);
textcolor(15);
writeln;
writeln('All compiles successful!');
writeln;
shutdown;
end;
*/

void parse_param(string s) //this function has the file open portion, i'm placing my format code here and we can put in our fopen calls
{
/*  var
  f:text;
  fn,s1:string;
  found:boolean;
  begin
  found = false;
  s = btrim(ucase(s));
  if s='' then exit;
  if s[1]='#' then
  begin
  fn = rstr(s,length(s)-1);
  if fn=base_name(fn) then fn = fn+config_ext;
  if not exist(fn) then prog_error(6,fn);
  assign(f,fn); reset(f);
  while not eof(f) do
  begin
  readln(f,s1);
  s1 = ucase(btrim(s1));
  if s1[1]='#' then prog_error(7,s1)
     else parse_param(s1);
  end;
  close(f);
  found = true;
  end
  else if s[1] in ['/','-','='] then
  begin
  s1 = rstr(s,length(s)-1);
  {FIFI}
  if s1[1]='X' then
  begin
  step_mode = value(rstr(s1,length(s1)-1));
  found = true;
  if step_mode=0 then step_mode = 1;
  if (step_mode<1) or (step_mode>9) then prog_error(24,rstr(s1,length(s1)-1));
  end;
  {/FIFI}
  if s1[1]='D' then begin game_delay = value(rstr(s1,length(s1)-1)); found = true; end;
  if s1[1]='T' then begin time_slice = value(rstr(s1,length(s1)-1)); found = true; end;
  if s1[1]='L' then begin game_limit = value(rstr(s1,length(s1)-1))*1000; found = true; end;
  if s1[1]='Q' then begin sound_on = false; found = true; end;
  if s1[1]='M' then begin matches = value(rstr(s1,length(s1)-1)); found = true; end;
  if s1[1]='S' then begin show_source = false; found = true; end;
  if s1[1]='G' then begin no_gfx = true; found = true; end;
  if s1[1]='R' then begin report = true; found = true;
                  if (length(s1)>1) then
                     report_type = value(rstr(s1,length(s1)-1)); end;
  if s1[1]='C' then begin compile_only = true; found = true; end;
  if s1[1]='^' then begin show_cnotice = false; found = true; end;
  if s1[1]='A' then begin show_arcs = true; found = true; end;
  if s1[1]='W' then begin windoze = false; found = true; end;
  if s1[1]='$' then begin debug_info = true; found = true; end;
  if s1[1]='#' then begin maxcode = value(rstr(s1,length(s1)-1))-1; found = true; end;
  if s1[1]='!' then begin insane_missiles = true; if (length(s1)>1) then
                  insanity = value(rstr(s1,length(s1)-1)); found = true; end;
  if s1[1]='@' then begin old_shields = true; found = true; end;
  if s1[1]='E' then begin logging_errors = true; found = true; end;
  if insanity<0 then insanity = 0;
  if insanity>15 then insanity = 15;
  end
  else if s[1]=';' then found = true
  else if (num_robots<max_robots) and (s<>'') then
  begin
  inc(num_robots);
  create_robot(num_robots,s);
  found = true;
  if num_robots=max_robots then writeln('Maximum number of robots reached.');
  end
  else prog_error(10,'');
  if not found then prog_error(8,s);
*/
}


void init()
{
    int i;

    if (debugging_compiler || compile_by_line || show_code)
    {
        cout<<"!!! Warning !!! Compiler Debugging enabled !!!";
        flushkey();
//        readkey();    FUNCTION DOESNT EXIST
        cout<<endl;
    }

    step_mode = 0; //{stepping disabled}
    logging_errors = false;
    stats_mode = 0;
    insane_missiles = false;
    insanity = 0;
    delay_per_sec = 0;
    windoze = true;
    graphix = false;
    no_gfx = false;
    sound_on = true;
    timing = true;
    matches = 1;
    played = 0;
    old_shields = false;
    quit = false;
    compile_only = false;
    show_arcs = false;
    debug_info = false;
    show_cnotice = true;
    show_source = true;
    report = false;
    kill_count = 0;
    maxcode = max_code;
    make_tables();
    //randomize();      FUNCTION DOESNT EXIST
    num_robots = -1;
    game_limit = 100000;
    game_cycle = 0;
    game_delay = default_delay;
    time_slice = default_slice;
    for (i = 0; i < max_missiles; i++)
    //with missile[i]. do
    {
        missile[i].a = 0;
        missile[i].source = -1;
        missile[i].x = 0;
        missile[i].y = 0;
        missile[i].lx = 0;
        missile[i].ly = 0;
        missile[i].mult = 1;
    }

    registered = false;
    reg_name = "Unregistered";
    reg_num = 65535;
    check_registration();

    cout<<endl;
    //textcolor(3);
    cout<<progname<<" "<<version<<" "<<cnotice1<<endl<<cnotice2<<endl;
//    textcolor(7);
    if (!registered)
    {
//        textcolor(4);
        cout<<"Unregistered version"<<endl;
    }
    else
    {
        cout<<"Registered to: "<<reg_name<<endl;
    }
//    textcolor(7);
    cout<<endl;
    //{create_robot(0,'SDUCK');}

//    delete_compile_report();      FUNCTION DOES NOT EXIST
    /*if (paramcount>0)             VARIABLE DOES NOT EXIST
    {
        for (i = 1; i <= paramcount;i++)
        {
            parse_param(btrim(ucase(paramstr(i))));
        }
    }

    else
    {
        prog_error(5,"");
    }*/

    temp_mode = step_mode;  //{store initial step_mode}

    if (logging_errors)
    {
        for (i = 0; i < num_robots; i++)
        //with robot[i]^ do
        {
//            assign(errorlog,base_name(fn)+'.ERR');
//            rewrite(errorlog);
        }
    }

    if (compile_only)
    {
//        write_compile_report();   FUNCTION DOES NOT EXIST
    }
    if (num_robots<1)
    {
        prog_error(4,"");
    }

    if (!no_gfx)
    {
        graph_mode(true);
    }

    //{--fix ups--}
    if (matches>100000)
    {
        matches = 100000;
    }
    if (matches<1)
    {
        matches = 1;
    }
    if (game_delay>1000)
    {
        game_delay = 1000;
    }
    if (game_delay<0)
    {
        game_delay = 0;
    }
    if (time_slice>100)
    {
        time_slice = 100;
    }
    if (time_slice<1)
    {
        time_slice = 1;
    }
    if (game_limit<0)
    {
        game_limit = 0;
    }
    if (game_limit>100000)
    {
        game_limit = 100000;
    }
    if (maxcode<1)
    {
        maxcode = 1; //{0 based, so actually 2 lines}
    }
    if (maxcode>max_code)
    {
        maxcode = max_code;
    }

    if (!graphix)
    {
        cout<<"Freemem: "/*<<memavail*/<<endl<<endl;
    }
}

/*
procedure draw_robot(n:integer);
var
i,t:integer;
xx,yy:real;
begin
if not (n in [0..num_robots]) then exit;
with robot[n]^ do
begin
if x>1000 then x = 1000;
if y>1000 then y = 1000;
if x<0 then x = 0;
if y<0 then y = 0;
{if (lhd=hd) and (lx=x) and (ly=y) then exit;}

{--set up for erase--}
{xx = lx*screen_scale; yy = ly*screen_scale;
lhd = (lhd+1024) and 255;
ltx[1] = round(xx+sint[lhd]*robot_scale);
lty[1] = round(yy-cost[lhd]*robot_scale);
ltx[2] = round(xx+sint[(lhd+$68) and 255]*robot_scale);
lty[2] = round(yy-cost[(lhd+$68) and 255]*robot_scale);
ltx[3] = round(xx+sint[(lhd+$98) and 255]*robot_scale);
lty[3] = round(yy-cost[(lhd+$98) and 255]*robot_scale);
t = (lhd+(lshift and 255)+1024) and 255;
ltx[4] = round(xx+sint[t and 255]*robot_scale);
lty[4] = round(yy-cost[t and 255]*robot_scale);}

{--set up for draw--}
xx = x*screen_scale+screen_x; yy = y*screen_scale+screen_y;
hd = (hd+1024) and 255;
tx[1] = round(xx+sint[hd]*5);
ty[1] = round(yy-cost[hd]*5);
tx[2] = round(xx+sint[(hd+$68) and 255]*robot_scale);
ty[2] = round(yy-cost[(hd+$68) and 255]*robot_scale);
tx[3] = round(xx+sint[(hd+$98) and 255]*robot_scale);
ty[3] = round(yy-cost[(hd+$98) and 255]*robot_scale);
t = (hd+(shift and 255)+1024) and 255;
tx[4] = round(xx); ty[4] = round(yy);
tx[5] = round(xx+sint[t]*robot_scale*0.8);
ty[5] = round(yy-cost[t]*robot_scale*0.8);
tx[6] = round(xx+sint[(t+scanarc+1024) and 255]*scanrange*screen_scale);
ty[6] = round(yy-cost[(t+scanarc+1024) and 255]*scanrange*screen_scale);
tx[7] = round(xx+sint[(t-scanarc+1024) and 255]*scanrange*screen_scale);
ty[7] = round(yy-cost[(t-scanarc+1024) and 255]*scanrange*screen_scale);
startarc = round(((256-((t+scanarc+1024) and 255))/256*360)+90);
endarc = round(((256-((t-scanarc+1024) and 255))/256*360)+90);


if graphix then
begin
main_viewport;
{--erase--}
setcolor(0);
if lshields then
   circle(ltx[4],lty[4],robot_scale);
if (arc_count>0) then
 begin
  line(ltx[4],lty[4],ltx[6],lty[6]);
  line(ltx[4],lty[4],ltx[7],lty[7]);
  if scanrange<1500 then
     arc(ltx[4],lty[4],lstartarc,lendarc,round(scanrange*screen_scale));
 end;
if (sonar_count>0) then
 begin
  circle(ltx[4],lty[4],round(max_sonar*screen_scale));
 end;
ifhealth>0 then
 begin {only erase body if still alive, that way we leave a "corpse" when dead}
  line(ltx[1],lty[1],ltx[2],lty[2]);
  line(ltx[2],lty[2],ltx[3],lty[3]);
  line(ltx[3],lty[3],ltx[1],lty[1]);
  line(ltx[4],lty[4],ltx[5],lty[5]);
 end;
end;
ifhealth>0 then //{If we're still alive we need to redraw}
begin
if arc_count>0   then dec(arc_count);
if sonar_count>0 then dec(sonar_count);
if graphix then
 begin
  {--draw--}
  setcolor(robot_color(n) and 7);
  if shields_up then
   circle(tx[4],ty[4],robot_scale);
  setcolor(robot_color(n));
  line(tx[1],ty[1],tx[2],ty[2]);
  line(tx[2],ty[2],tx[3],ty[3]);
  line(tx[3],ty[3],tx[1],ty[1]);
  setcolor(7);
  line(tx[4],ty[4],tx[5],ty[5]);
  setcolor(8);
  if show_arcs and (arc_count>0) then
   begin
    line(tx[4],ty[4],tx[6],ty[6]);
    line(tx[4],ty[4],tx[7],ty[7]);
    if scanrange<1500 then
       arc(tx[4],ty[4],startarc,endarc,round(scanrange*screen_scale));
   end;
  if show_arcs and (sonar_count>0) then
     circle(tx[4],ty[4],round(max_sonar*screen_scale));
end;
lx = x; ly = y; lhd = hd; lshift = shift; lshields = shields_up;
for i = 1 to max_robot_lines do
 begin ltx[i] = tx[i]; lty[i] = ty[i]; end;
lstartarc = startarc; lendarc = endarc;
end;
end;
end;
*/
int get_from_ram(int n,int i,int j)
{
    int k,l;
    //with robot[n]. do
    if ((i<0) || (i>(max_ram+1)+(((max_code+1) << 3)-1)))
    {
       k = 0;
       robot_error(n,4,(i+""));
    }
    else
    {
        if (i<=max_ram)
        {
            k = robot[n].ram[i];
        }
        else
        {
            l = i-max_ram-1;
            k = robot[n].code.code[l >> 2].op[l & 3];
        }
    }

    return k;
}

int get_val(int n,int c,int o)
{
    int i,j,k;
    k = 0;
    //with robot[n]. do

    j = (robot[n].code.code[c].op[max_op] >> (4*o)) & 15;
    i = robot[n].code.code[c].op[o];
    if ((j & 7)==1)
    {
        k = get_from_ram(n,i,j);
    }
    else
    {
        k = i;
    }

    if ((j & 8)>0)
    {
        k = get_from_ram(n,k,j);
    }

    return k;
}


void put_val(int n,int c,int o,int v)
{
    int i,j,k;

    k = 0;
    i = 0;
    j = 0;
    //with robot[n]. do

    j = (robot[n].code.code[c].op[max_op] >> (4*o)) & 15;
    i = robot[n].code.code[c].op[o];
    if ((j & 7)==1)
    {
        if ((i<0) || (i>max_ram))
        {
            robot_error(n,4,i+"");
        }

        else
        {
            if ((j & 8)>0)
            {
                i = robot[n].ram[i];
                if ((i<0) || (i>max_ram))
                {
                    robot_error(n,4,i+"");
                }
                else
                {
                    robot[n].ram[i] = v;
                }
            }
            else
            {
                robot[n].ram[i] = v;
            }
        }
    }
    else
    {
        robot_error(n,3,"");
    }
}

void push(int n,int v)
{
    //with robot[n]. do

    if ((robot[n].ram[71] >= stack_base) && (robot[n].ram[71] < (stack_base + stack_size)))
    {
        robot[n].ram[robot[n].ram[71]] = v;
        robot[n].ram[71]++;
    }
    else
    {
        robot_error(n,1,(robot[n].ram[71]+""));
    }
}

int pop(int n)
{
    int k;

    //with robot[n]. do

    if ((robot[n].ram[71] > stack_base) && (robot[n].ram[71] <= (stack_base + stack_size)))
    {
        robot[n].ram[71]--;
        k = robot[n].ram[robot[n].ram[71]];
    }
   else
   {
       robot_error(n,5,(robot[n].ram[71]+""));
   }

    return k;
}

int find_label(int n,int l,int m)
{
    int i,j,k;
    k = -1;
    //with robot[n]. do

    if (m==3)
    {
        robot_error(n,9,"");
    }
    else
    {
        if (m==4)
        {
            k = l;
        }

        else
        {
            for (i = robot[n].plen;  i >= 0; i--)
            {
                j = robot[n].code.code[i].op[max_op] & 15;
                if ((j==2) && (robot[n].code.code[i].op[0]==l))
                {
                    k = i;
                }
            }
        }
    }

    return k;
}


void init_mine(int n,int detectrange,int sizeN)
{
    int i,j,k;
    //with robot[n]. do
    k = -1;
    for (i = 0; i < max_mines; i++)
    {
        if (((robot[n].mine[i].x<0) || (robot[n].mine[i].x>1000) || (robot[n].mine[i].y<0) || (robot[n].mine[i].y>1000) || (robot[n].mine[i].yield<=0)) && (k<0))
        {
            k = i;
        }
    }

    if (k>=0)
    {
        robot[n].mine[k].x = robot[n].x;
        robot[n].mine[k].y = robot[n].y;
        robot[n].mine[k].detect = detectrange;
        robot[n].mine[k].yield = sizeN;
        robot[n].mine[k].detonate = false;
        click();
    }
}

int count_missiles()
{
    int i,k;
    k = 0;
    for (i = 0; i < max_missiles; i++)
    {
        if (missile[i].a>0)
        {
            k++;
        }
    }

    return k;
}

void init_missile(double xx, double yy, double xxv, double yyv,int dir,int s,int blast,bool ob)
{
    int i,k;
    double m ;

    k =-1;
    for (i =max_missiles; i >= 0; i--)
    {
        if (missile[i].a==0)
        {
            k =i;
        }
    }
    if (k>=0)
    {
        //with missile[k]. do
        missile[k].source = s;
        missile[k].x = xx;
        missile[k].lx = missile[k].x;
        missile[k].y = yy;
        missile[k].ly = missile[k].y;
        missile[k].rad = 0;
        missile[k].lrad = 0;
        if (ob)
        {
            missile[k].mult = 1.25;
        }
        else
        {
            missile[k].mult = 1;
        }
        if (blast>0)
        {
            missile[k].max_rad = blast;
            missile[k].a = 2;
        }
        else
        {
            if ((s>=0) && (s<=num_robots))
            {
                missile[k].mult = missile[k].mult*(robot[s].shotstrength);
            }
            m = missile[k].mult;
            if (ob)
            {
                m = m+0.25;
            }
            missile[k].mspd = missile_spd*missile[k].mult;
            if (insane_missiles)
            {
                missile[k].mspd =100+(50*insanity)*missile[k].mult;
            }
            if ((s>=0) && (s<=num_robots))
            {
                robot[s].heat+=round(20*m);
                robot[s].shots_fired++;
                robot[s].match_shots++;
            }
            missile[k].a = 1;
            missile[k].hd = dir;
            missile[k].max_rad =mis_radius;
//          if debug_info then
//              begin writeln(#13,zero_pad(game_cycle,5),' F ',s,': hd=',hd,'           ');     //COUT
//          repeat until /*keypressed FIX WITH GUI*/false; flushkey; end;
        }
    }
}

void damage(int n,int d,bool physical)
{
    int i,k,h,dd;
    double m;
    if ((n<0) || (n>num_robots) || (robot[n].armor<=0))
    {
        return;
    }

    if (robot[n].config.shield<3)
    {
        robot[n].shields_up =false;
    }
    //with robot[n]^ do
    h =0;
    if ((robot[n].shields_up) && (!physical))
    {
        dd =d;
        if ((old_shields) && (robot[n].config.shield>=3))
        {
            d =0;
            h =0;
        }
        else
        {
            switch (robot[n].config.shield)
            {
                case 3: d =round(dd*2/3);
                    if (d<1)
                    {
                        d =1;
                        h =round(dd*2/3);
                    }
                case 4: h =(int)(dd/2);
                    d =dd-h;
                case 5: d =round(dd*1/3);
                    if (d<1)
                    {
                        d =1;
                    }
                default: h =round(dd*1/3);
                if (h<1)
                {
                    h =1;
                }
            }
        }
    }
    if (d<0)
    {
        d =0;
    }
 //   if debug_info then
 //       begin writeln(#13,zero_pad(game_cycle,5),' D ',n,': ',health,'-',d,'=',health-d,'           ');
 //       repeat until /*keypressed FIX WITH GUI*/false; flushkey; end;
    if (d>0)
    {
        d =round(d*robot[n].damageadj);
        if (d<1)
        {
            d =1;
        }
    }

    robot[n].armor -=d;
    robot[n].heat+=h;
    robot[n].last_damage =0;
    if (robot[n].armor<=0)
    {
        robot[n].armor =0;
//        update_health(n);
        robot[n].heat =500;
//        update_heat(n);
        robot[n].armor =0;
        kill_count = 0;
        robot[n].deaths = 0;
//        update_lives(n);
 //       if graphix && timing then time_delay(10);
 //       draw_robot(n);    //replace with new removal of robot from GUI
        robot[n].heat =0;
        //update_heat(n);       //STUFF TO DO WITH GUI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        init_missile(robot[n].x,robot[n].y,0,0,0,n,blast_circle,false);
        if (robot[n].overburn)
        {
            m =1.3;
        }
        else
        {
            m =1;
        }
        for (i =0 ; i<num_robots;i++)
        {
            if ((i!=n) && (robot[i].armor>0))
            {
                k =round(distance(robot[n].x,robot[n].y,robot[i].x,robot[i].y));
                if (k<blast_radius)
                {
                    damage(i,round(abso(blast_radius-k)*m),false);
                }
            }
        }
    }
}

int scan(int n)
{
    double r,d,acc;
    int dir,range,i,j,k,nn,xx,yy,sign;
    nn = -1;
    range = maxint;
    if (!(n  >= 0 && n <=num_robots))
    {
        return -3;                      //This result of an invalid entry placeholder return
    }
    //with robot[n]^ do
    if( robot[n].scanarc<0)
    {
        robot[n].scanarc=0;
    }

    robot[n].accuracy=0;
    nn=-1;
    dir=(robot[n].shift+robot[n].hd) && 255;      //possible robot class values
    if (debug_info)
    {
        cout<<"<SCAN Arc="<<robot[n].scanarc<<", Dir="<<dir<<">";
    }
    for (i=0; i < num_robots;i++)
    {
        if ((i!=n) && (robot[i].armor>0))
        {
            j = find_anglei(robot[n].x,robot[n].y,robot[i].x,robot[i].y);
            d = distance(robot[n].x,robot[n].y,robot[i].x,robot[i].y);
            k = round(d);
            if ((k<range) && (k<=robot[n].scanrange) && ((abso(j-dir)<=abso(robot[n].scanarc)) || (abso(j-dir)>=256-abso(robot[n].scanarc))))
            {
                dir = (dir+1024) && 255;
                xx = round( sint[dir]*d+robot[n].x);
                yy = round(-cost[dir]*d+robot[n].y);
                r = distance(xx,yy,robot[i].x,robot[i].y);
                if (debug_info)
                {
                    cout<<"SCAN HIT! Scan X,Y: "<<round(xx)<<","<<round(yy)<<"  Robot X,Y: "<<
                        round(robot[i].x)<<","<<round(robot[i].y)<<"  Dist="<<round(r);
                    //repeat until keypressed; flushkey;                //THIS WAS FOR DEBUG MODE!!!! UPDATE CODE LATER!!!!
                }
                if ((robot[n].scanarc>0) || (r<hit_range-2))
                {
                    range = k;
                    robot[n].accuracy = 0;
                    if (robot[n].scanarc>0)
                    {
                        j=(j+1024) && 255;
                        dir=(dir+1024) && 255;
                        if (j<dir)
                        {
                            sign=-1;
                        }
                        if (j>dir)
                        {
                            sign= 1;
                        }
                        if ((j>190) && (dir<66))
                        {
                            dir=dir+256;
                            sign=-1;
                        }
                        if ((dir>190) && (j<66))
                        {
                            j = j+256;
                            sign = 1;
                        }
                        acc = abso(j-dir)/robot[n].scanarc*2;
                        if (sign<0)
                        {
                            robot[n].accuracy=-abso(round(acc));
                        }

                        else
                        {
                            robot[n].accuracy = abso(round(acc));
                        }

                        if (robot[n].accuracy>2)
                        {
                            robot[n].accuracy = 2;
                        }
                        if (robot[n].accuracy<-2)
                        {
                            robot[n].accuracy=-2;
                        }
                    }
                    nn = i;
                    if (debug_info)
                    {
//                        cout<<13<<zero_pad(game_cycle,5)<<" S "<<n<<": nn="<<nn<<", range="<<range<<", acc="<<accuracy<<endl;           ');
//                        repeat until keypressed; flushkey;            //AGAIN DEBUG MENU
                    }
                }
            }
            if (nn  >= 0 && nn <=num_robots)
            {
                robot[n].ram[5] = robot[nn].transponder;
                robot[n].ram[6] = (robot[nn].hd-(robot[n].hd+robot[n].shift)+1024) && 255;
                robot[n].ram[7] = robot[nn].spd;
                robot[n].ram[13] = round(robot[nn].speed*100);
            }
        }
    }
    return range;
}

void com_transmit(int n,int chan,int data)
{
    int i;

    for (i=0; i < num_robots; i++)
    {
        //with robot[i]. do
        if ((robot[i].armor > 0) && (i!=n) && (robot[i].channel==chan))
        {
            if ((robot[i].ram[10]<0) || (robot[i].ram[10]>max_queue))
            {
                robot[i].ram[10] = 0;
            }
            if ((robot[i].ram[11]<0) or (robot[i].ram[11]>max_queue))
            {
                robot[i].ram[11] = 0;
            }
            robot[i].ram[robot[i].ram[11]+com_queue] = data;
            robot[i].ram[11]++;
            if (robot[i].ram[11]>max_queue)
            {
                robot[i].ram[11] = 0;
            }
            if (robot[i].ram[11]==robot[i].ram[10])
            {
                robot[i].ram[10]++;
            }
            if (robot[i].ram[10]>max_queue)
            {
                robot[i].ram[10] = 0;
            }
        }
    }
}


int com_receive(int n)
{
    int k;

    k = 0;
    //with robot[n]. do

    if (robot[n].ram[10]!=robot[n].ram[11])
    {
        if ((robot[n].ram[10]<0) || (robot[n].ram[10]>max_queue))
        {
            robot[n].ram[10] = 0;
        }
        if ((robot[n].ram[11]<0) || (robot[n].ram[11]>max_queue))
        {
            robot[n].ram[11] = 0;
        }
        k = robot[n].ram[robot[n].ram[10]+com_queue];
        robot[n].ram[10]++;
        if (robot[n].ram[10]>max_queue)
        {
            robot[n].ram[10] = 0;
        }
    }
    else
    {
        robot_error(n,12,"");
    }

    return k;
}

void out_port(int n,int p,int v, int* time_used)
{
    int i;

    //with robot[n]. do

    switch (p)
    {
        case 11:robot[n].tspd = v;
        break;
        case 12:robot[n].shift = (robot[n].shift+v+1024) & 255;
        break;
        case 13:robot[n].shift = (v+1024) & 255;
        break;
        case 14:robot[n].thd = (robot[n].thd+v+1024) & 255;
        break;
        case 15:
            *time_used +=3;
            if (v>4)
            {
                v = 4;
            }
            if (v<-4)
            {
                v = -4;
            }
            init_missile(robot[n].x,robot[n].y,robot[n].xv,robot[n].yv,(robot[n].hd+robot[n].shift+v) & 255,n,0,robot[n].overburn);
            break;
        case 17:
            robot[n].scanarc = v;
            break;
        case 18:
            if (v==0)
            {
                robot[n].overburn = false;
            }
            else
            {
                robot[n].overburn = true;
            }
        break;
        case 19:robot[n].transponder = v;
        break;
        case 20:robot[n].shutdown = v;
        break;
        case 21:robot[n].channel = v;
        break;
        case 22:
            if (robot[n].config.mines>=0)
            {
                if (robot[n].mines>0)
                {
                    init_mine(n,v,mine_blast);
                    robot[n].mines--;
                }
                else
                {
                    robot_error(n,14,"");
                }
            }
            else
            {
                robot_error(n,13,"");
            }
        break;
        case 23:
            if (robot[n].config.mines>=0)
            {
                for (i = 0; i < max_mines; i++)
                {
                    robot[n].mine[i].detonate = true;
                }
            }
            else
            {
                robot_error(n,13,"");
            }
        break;
        case 24:
            if (robot[n].config.shield>=3)
            {
                if (v==0)
                {
                    robot[n].shields_up = false;
                }
                else
                {
                    robot[n].shields_up = true;
                }
            }
            else
            {
                robot[n].shields_up = false;
                robot_error(n,15,"");
            }
        break;
        default: robot_error(n,11,p+"");
    }
    if (robot[n].scanarc>64)
    {
        robot[n].scanarc = 64;
    }
    if (robot[n].scanarc<0)
    {
        robot[n].scanarc = 0;
    }
}

int in_port(int n,int p,int* time_used)
{
    int v,i,j,k,l,nn;

    v = 0;
    //with robot[n]. do
    switch (p)
    {
        case 1:v = robot[n].spd;
        break;
        case 2:v = robot[n].heat;
        break;
        case 3:v = robot[n].hd;
        break;
        case 4:v = robot[n].shift;
        break;
        case 5:v = ((robot[n].shift+robot[n].hd) & 255);
        break;
        case 6:v = robot[n].armor;
        break;
        case 7:v = scan(n);
            *time_used+=1;
            if (show_arcs)
            {
                robot[n].arc_count = 2;
            }
        break;
        case 8:
            v = robot[n].accuracy;
            *time_used+=1;
        break;
        case 9:
            nn = -1;
            *time_used += 3;
            k = maxint;
            nn = -1;
            for (i = 0; i < num_robots;i++)
            {
                j = round(distance(robot[n].x,robot[n].y,robot[i].x,robot[i].y));
                if ((n!=i) && (j<k) && (robot[i].armor>0))
                {
                    k = j;
                    nn = i;
                }
            }
            v = k;
            if ((nn>=0) && (nn <num_robots))
            {
                robot[n].ram[5] = robot[nn].transponder;
            }
        break;
        case 10:v = (rand()%65535)+(rand()%2);
        break;
        case 16:
            nn = -1;
            if (show_arcs)
            {
                robot[n].sonar_count = 2;
            }
            *time_used += 40;
            l = -1;
            k = maxint;
            nn = -1;
            for (i = 0; i < num_robots; i++)
            {
                j = round(distance(robot[n].x,robot[n].y,robot[i].x,robot[i].y));
                if ((n!=i) && (j<k) && (j<max_sonar) && (robot[i].armor>0))
                {
                    k = j;
                    l = i;
                    nn = i;
                }
            }
            if (l>=0)
            {
                v = ((int)(round(find_angle(robot[n].x,robot[n].y,robot[l].x,robot[l].y)/pi*128+1024+(rand()%65)-32)) & 255);
            }
            else
            {
                v = minint;
            }
            if ((nn >= 0) && (nn<num_robots))
            {
                robot[n].ram[5] = robot[nn].transponder;
            }
        break;
        case 17:
            v = robot[n].scanarc;
        break;
        case 18:
            if (robot[n].overburn)
            {
                v = 1;
            }

            else v = 0;
        break;
        case 19:v = robot[n].transponder;
        break;
        case 20:v = robot[n].shutdown;
        break;
        case 21:v = robot[n].channel;
        break;
        case 22:v = robot[n].mines;
        break;
        case 23:
            if (robot[n].mines>=0)
            {
                k = 0;
                for (i = 0; i < max_mines; i++)
                {
                    //with mine[i]. do
                    if ((robot[n].mine[i].x>=0) && (robot[n].mine[i].x<=1000) && (robot[n].mine[i].y>=0) &&
                            (robot[n].mine[i].y<=1000) && (robot[n].mine[i].yield>0))
                    {
                        k++;
                    }

                }
                v = k;
            }
            else
            {
                v = 0;
            }
        break;
        case 24:
            if (robot[n].config.shield>0)
            {
                if (robot[n].shields_up)
                {
                    v = 1;
                }
                else
                {
                    v = 0;
                }
            }
            else
            {
                v = 0;
                robot[n].shields_up = false;
            }
        break;
        default: robot_error(n,11,p+"");
    }

    return v;
}


void call_int(int n,int int_num, int *time_used)
{
    int i,j,k;

 //with robot[n]^ do

    switch (int_num)
    {
        case 0:damage(n,1000,true);
        case 1:
            reset_software(n);
            *time_used = 10;
        break;
        case 2:
            *time_used = 5;
            robot[n].ram[69] = round(robot[n].x);
            robot[n].ram[70] = round(robot[n].y);
        break;
        case 3:
            *time_used = 2;
            if (robot[n].ram[65]==0)
            {
                robot[n].keepshift = false;
            }
            else
            {
                robot[n].keepshift = true;
            }

            robot[n].ram[70] = robot[n].shift & 255;
        break;
        case 4:
            if (robot[n].ram[65]==0)
            {
                robot[n].overburn = false;
            }
            else
            {
                robot[n].overburn = true;
            }
        break;
        case 5:
            *time_used = 2;
            robot[n].ram[70] = robot[n].transponder;
        break;
        case 6:
            *time_used = 2;
            robot[n].ram[69] = game_cycle >> 16;
            robot[n].ram[70] = game_cycle & 65535;
        break;
        case 7:
            j = robot[n].ram[69];
            k = robot[n].ram[70];
            if (j<0)
            {
                j = 0;
            }
            if (j>1000)
            {
                j = 1000;
            }
            if (k<0)
            {
                k = 0;
            }
            if (k>1000)
            {
                k = 1000;
            }
            robot[n].ram[65] = (int)(round(find_angle(round(robot[n].x),round(robot[n].y),j,k)/pi*128+256)) & 255;
            *time_used = 32;
        break;
        case 8:
            robot[n].ram[70] = robot[n].ram[5];
            *time_used = 1;
        break;
        case 9:
            robot[n].ram[69] = robot[n].ram[6];
            robot[n].ram[70] = robot[n].ram[7];
            *time_used = 2;
        break;
        case 10:
            k = 0;
            for (i = 0; i < num_robots;i++)
            {
                if (robot[i].armor>0)
                {
                    k++;
                }
            }

            robot[n].ram[68] = k;
            robot[n].ram[69] = played;
            robot[n].ram[70] = matches;
            *time_used = 4;
        break;
        case 11:
            robot[n].ram[68] = round(robot[n].speed*100);
            robot[n].ram[69] = robot[n].last_damage;
            robot[n].ram[70] = robot[n].last_hit;
            *time_used = 5;
        break;
        case 12:
            robot[n].ram[70] = robot[n].ram[8];
            *time_used = 1;
        break;
        case 13:
            robot[n].ram[8] = 0;
            *time_used = 1;
        break;
        case 14:
            com_transmit(n,robot[n].channel,robot[n].ram[65]);
            *time_used = 1;
        break;
        case 15:
            if (robot[n].ram[10]!=robot[n].ram[11])
            {
                robot[n].ram[70] = com_receive(n);
            }
            else
            {
                robot_error(n,12,"");
            }

            *time_used = 1;
        break;
        case 16:
            if (robot[n].ram[11]>=robot[n].ram[10])
            {
                 robot[n].ram[70] = robot[n].ram[11]-robot[n].ram[10];
            }

            else
            {
                robot[n].ram[70] = max_queue+1-robot[n].ram[10]+robot[n].ram[11];
            }
            *time_used = 1;
        break;
        case 17:
            robot[n].ram[10] = 0;
            robot[n].ram[11] = 0;
            *time_used = 1;
        break;
        case 18:
            robot[n].ram[68] = robot[n].kills;
            robot[n].ram[69] = robot[n].kills-robot[n].startkills;
            robot[n].ram[70] = robot[n].deaths;
            *time_used = 3;
        break;
        case 19:
                robot[n].ram[9] = 0;
                robot[n].meter = 0;
        break;
        default: robot_error(n,10,int_num+"");
    }
}

void jump(int n,int o, bool inc_ip)
{
    int loc;
    //with robot[n]. do

    loc = find_label(n,get_val(n,robot[n].ip,o),robot[n].code.code[robot[n].ip].op[max_op] >> (o*4));
    if ((loc>=0) && (loc<=robot[n].plen))
    {
        inc_ip = false;
        robot[n].ip = loc;
    }
    else
    {
        robot_error(n,2,(loc+""));
    }
}

void update_debug_bars()
{
    //THE FOLLOWING COUPLE FUNCTIONS ARE SHELLS TO BE FILLED OUT BY GUI PEOPLE
}

void update_debug_system()
{

}

void update_debug_registers()
{

}

void update_debug_flags()
{

}

void update_debug_memory()
{

}

void update_debug_code()
{

}


void update_debug_window()
{
    if (graphix && (step_mode>0))
    {
        //{armour + heat}
        update_debug_bars();

        //{system variables}
        update_debug_system();

        //{registers}
        update_debug_registers();

        //{flag register}
        update_debug_flags();

        //{memory}
        update_debug_memory();

        //{code}
        update_debug_code();

    }
}

/*
procedure init_debug_window;
var i:integer;
begin
if graphix then
begin

{debugger window}
viewport(480,4,635,430);
setfillstyle(1,7);
bar(0,0,155,422);
viewport(480,4,635,410);
hole(0,0,155,402);

with robot[0]^ do
begin
setcolor(robot_color(0));
outtextxy(003,02,base_name(no_path(fn)));

{armour}
outtextxy(080,02,'A');
setfillstyle(1,robot_color(0));

{heat}
outtextxy(118,02,'H');
setfillstyle(1,robot_color(0));

{system variables}
setcolor(robot_color(8));
outtextxy(007,22,'System:');
setcolor(robot_color(0));
outtextxy(015,32,'00:      07:');
outtextxy(015,42,'01:      08:');
outtextxy(015,52,'02:      09:');
outtextxy(015,62,'03:      10:');
outtextxy(015,72,'04:      11:');
outtextxy(015,82,'05:      12:');
outtextxy(015,92,'06:      13:');

{registers}
setcolor(robot_color(8));
outtextxy(007,112,'Registers:');
setcolor(robot_color(0));
outtextxy(015,122,'ax:      ex:');
outtextxy(015,132,'bx:      fx:');
outtextxy(015,142,'cx:');
outtextxy(015,152,'dx:      sp:');

{flag register}
setcolor(robot_color(8));
outtextxy(007,172,'Flags:');

{memory}
setcolor(robot_color(8));
outtextxy(007,202,'Memory:');
setcolor(7);
outtextxy(103,202,'-+,[]');
setcolor(robot_color(0));
for i = 0 to 9 do
outtextxy(035,212+(10*i),decimal((mem_watch+i),4) + ' :');

{code}
setcolor(robot_color(8));
outtextxy(007,322,'Code:');

end;
update_debug_window;

end;
end;
{/FIFI}

{FIFI}
procedure close_debug_window;
var i:integer;
begin
if graphix then
begin

stats_mode = 0;
case num_robots of
0..5:stats_mode = 0;
6..11:stats_mode = 1;
12..max_robots:stats_mode = 2;
else stats_mode = 0;
end;

viewport(480,4,635,430);
setfillstyle(1,7);
bar(0,0,155,426);
{ update_cycle_window;}

{--robot windows--}
for i = 0 to max_robots do
if i<max_shown then with robot[i]^ do
begin
robot_graph(i);
hole(0,0,max_gx,max_gy);
if i<=num_robots then
begin
 setcolor(robot_color(i));
 outtextxy(003,02,base_name(no_path(fn)));
 case stats_mode of
  1:begin
     outtextxy(003,12,' A:');
     outtextxy(003,22,' H:');
    end;
  2:begin
     setcolor(robot_color(i) and 7);
     outtextxy(080,02,'A');
     outtextxy(118,02,'H');
    end;
  else begin
     outtextxy(003,24,' A:');
     outtextxy(003,34,' H:');
    end;
 end;
 setcolor(robot_color(i));
 if stats_mode<=1 then
  begin
   outtextxy(080,02,'Wins:');
   outtextxy(122,02,zero_pad(wins,4));
  end;
 if stats_mode=0 then
  begin
   outtextxy(003,56,' Error:');
   setcolor(robot_color(i) and 7);
   outtextxy(003,12,name);
   setcolor(8);
   outtextxy(66,56,'None');
  end;
 lx = 1000-x; ly = 1000-y;
 update_armor(i);
 update_heat(i);
 update_lives(i);
end
else
begin
 setfillpattern(gray50,8);
 bar(1,1,max_gx-1,max_gy-1);
end;
end;

end;
end;
{/FIFI}
*/
bool gameover()
{
    int n,k;

    if ((game_cycle>=game_limit) && (game_limit>0))
    {
        return true;
    }
    if ((game_cycle & 31)== 0)
    {
        k = 0;
        for (n = 0; n<num_robots;n++)
        {
            if (robot[n].armor>0)
            {
                k++;
            }
        }

        if (k<=1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

bool invalid_microcode(int n,int ip)
{
    bool invalid;
    int i,k;
    invalid = false;
    for( i = 0;i < 2; i++)
    {
        k = (robot[n].code.code[ip].op[max_op] >> (i << 2)) & 7;
        if (!((k == 0) || (k == 1) || (k ==2) || (k ==4)))
        {
            invalid = true;
        }
    }
    return invalid;
}
/*
procedure process_keypress(c:char);
begin
case c of
'C':calibrate_timing;
'T':timing = not timing;
'A':show_arcs = not show_arcs;
'S','Q':begin
       if sound_on then chirp;
       sound_on = not sound_on;
       if sound_on then chirp;
      end;
'$':debug_info = not debug_info;
'W':windoze = not windoze;
#8:bout_over = true;
#27:begin quit = true; step_loop = false; end;
end;
end;
*/
void execute_instruction(int n)
{
    int j,k;
    int time_used,loc;
    bool inc_ip;


    //with robot[n]^ do

//    {--update system variables--}
    robot[n].ram[0]= robot[n].tspd;
    robot[n].ram[1]= robot[n].thd;
    robot[n].ram[2]= robot[n].shift;
    robot[n].ram[3]= robot[n].accuracy;

    time_used= 1;
    inc_ip= true;
    loc= 0;
    if ((robot[n].ip>robot[n].plen) || (robot[n].ip<0))
    {
        robot[n].ip= 0;
    }

    if (invalid_microcode(n,robot[n].ip))
    {
        time_used= 1;
        robot_error(n,16,hex(robot[n].code.code[robot[n].ip].op[max_op]));
    }
    else
    {
        if (graphix && (step_mode>0) && (n=0)) //{if stepping enabled...}
        {
            step_count++;
//            update_cycle_window();        //THIS FUNCTION UPDATES THE GRAPHICS IN THE GUI!!!!!!
            update_debug_window();
            if ((step_count % step_mode)==0)
            {
                step_loop= true;
            }
            else
            {
                step_loop= false;
            }

            while (step_loop && (!(quit || gameover() || bout_over)))
            {
                if (/*keypressed FIX WITH GUI*/false) //with robot[0]^ do
                {
/*                    const char c= upcase(readkey);                //GUI STUFF TO DO LATER!!!!!
                    switch (c)
                    {
                        case 88:
                            temp_mode= step_mode;
                            step_mode= 0;
                            step_loop= false;
                            close_debug_window();
                            break;
                        case 32:
                            step_count= 0;
                            step_loop= false;
                            break;
                        case 49:
                        case 50:
                        case 51:
                        case 52:
                        case 53:
                        case 54:
                        case 55:
                        case 56:
                        case 57:
                            step_mode= value(c);
                            step_count= 0;
                            step_loop= false;
                            break;
                        case 48:
                            step_mode= 10;
                            step_count= 0;
                            step_loop= false;
                            break;
                        case 45:
                        case 95:
                            if (robot[n].mem_watch>0)
                            {
                                setcolor(0);
                                for (i= 0; i < 9;i++)
                                {
                                    outtextxy(035,212+(10*i),decimal((mem_watch+i),4) + ' :');
                                }
                                    mem_watch--;
                                    update_debug_memory();
                            }
                            break;
                        case 43:
                        case 61:
                            if (mem_watch<1014)
                            {
                                setcolor(0);
                                for (i= 0; i < 9;i++)
                                {
                                    outtextxy(035,212+(10*i),decimal((mem_watch+i),4) + ' :');
                                }
                                mem_watch++;
                                update_debug_memory();
                            }
                            break;
                        case 91:
                        case 123:
                            if (mem_watch>0)
                            {
                                setcolor(0);
                                for (i= 0; i < 9;i++)
                                {
                                    outtextxy(035,212+(10*i),decimal((mem_watch+i),4) + ' :');
                                }
                                mem_watch-=10;
                                if (mem_watch<0)
                                {
                                    mem_watch= 0;
                                }
                                update_debug_memory();
                            }
                        break;
                        case 93:
                        case 125:
                            if (mem_watch<1014)
                            {
                                setcolor(0);
                                for (i= 0; i < 9;i++)
                                {
                                    outtextxy(035,212+(10*i),decimal((mem_watch+i),4) + ' :');
                                }
                                mem_watch+=10;
                                if (mem_watch>1014)
                                {
                                    mem_watch= 1014;
                                }
                                update_debug_memory();
                            }
                            break;
                        case 71:
                            toggle_graphix();
                            temp_mode= step_mode;
                            step_mode= 0;
                            step_loop= false;
                            break;
                        default: process_keypress(c);
                   }
                 }
*/            }
        }
    }


    if (! (((robot[n].code.code[robot[n].ip].op[max_op] & 7) > 0)&& ( robot[n].code.code[robot[n].ip].op[max_op]<1)))
    {
        time_used= 0;
    }
    else
    {
        switch (get_val(n,robot[n].ip,0))
        {
            case 0: //(*NOP*)
                executed++;
                break;

            case 1: //(*ADD*)
                put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,1)+get_val(n,robot[n].ip,2));
                executed++;
                break;

            case 2: //(*SUB*)
                put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,1)-get_val(n,robot[n].ip,2));
                executed++;
                break;

            case 3: //(*OR*)
                put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,1) | get_val(n,robot[n].ip,2));
                executed++;
                break;

            case 4: //(*AND*)
                put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,1) & get_val(n,robot[n].ip,2));
                executed++;
                break;

            case 5: //(*XOR*)
                put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,1) ^ get_val(n,robot[n].ip,2));
                executed++;
                break;

            case 6: //(*NOT*)
                put_val(n,robot[n].ip,1,!(get_val(n,robot[n].ip,1)));
                executed++;
                break;

            case 7: //(*MPY*)
                put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,1)*get_val(n,robot[n].ip,2));
                time_used= 10;
                executed++;
                break;

            case 8: //(*DIV*)
                j= get_val(n,robot[n].ip,2);
                if (j!=0)
                {
                    put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,1) / j);
                }
                else
                {
                    robot_error(n,8,"");
                }
                time_used= 10;
                executed++;
                break;

            case 9: //(*MOD*)
                j= get_val(n,robot[n].ip,2);
                if (j!=0)
                {
                    put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,1) % j);
                }
                else
                {
                    robot_error(n,8,"");
                }
                time_used= 10;
            executed++;
            break;

            case 10: //(*RET*)
                robot[n].ip= pop(n);
                if ((robot[n].ip<0) || (robot[n].ip>robot[n].plen))
                {
                    robot_error(n,7,(robot[n].ip + ""));
                }
                executed++;
                break;

            case 11: //(*GSB*)
                loc= find_label(n,get_val(n,robot[n].ip,1),robot[n].code.code[robot[n].ip].op[max_op]<<(1*4));
                if (loc>=0)
                {
                    push(n,robot[n].ip);
                    inc_ip= false;
                    robot[n].ip= loc;
                }
                else
                {
                    robot_error(n,2,(get_val(n,robot[n].ip,1)+""));
                }
                executed++;
                break;

            case 12: //(*JMP*)
                jump(n,1,inc_ip);
                executed++;
                break;

            case 13: //(*JLS,JB*)
                if ((robot[n].ram[64] & 2)>0)
                {
                    jump(n,1,inc_ip);
                }

                time_used= 0;
                executed++;
                break;

            case 14: //(*JGR,JA*)
                if ((robot[n].ram[64] & 4)>0)
                {
                    jump(n,1,inc_ip);
                }

                time_used= 0;
                executed++;
                break;

            case 15: //(*JNE*)
                if ((robot[n].ram[64] & 1)==0)
                    jump(n,1,inc_ip);
                time_used= 0;
                executed++;
                break;

            case 16: //(*JEQ,JE*)
                if ((robot[n].ram[64] & 1)>0)
                {
                    jump(n,1,inc_ip);
                }

                time_used= 0;
                executed++;
                break;

            case 17: //(*SWAP, XCHG*)
                robot[n].ram[4]= get_val(n,robot[n].ip,1);
                put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,2));
                put_val(n,robot[n].ip,2,robot[n].ram[4]);
                time_used= 3;
                executed++;
                break;

            case 18: //(*DO*)
                robot[n].ram[67]= get_val(n,robot[n].ip,1);
                executed++;
                break;

            case 19: //(*LOOP*)
                robot[n].ram[67]--;
                if (robot[n].ram[67]>0)
                {
                    jump(n,1,inc_ip);
                }

                executed++;
                break;

            case 20: //(*CMP*)
                k = get_val(n,robot[n].ip,1)-get_val(n,robot[n].ip,2);
                robot[n].ram[64]= robot[n].ram[64] & 65520;
                if (k==0)
                {
                    robot[n].ram[64]= robot[n].ram[64] | 1;
                }
                if (k<0)
                {
                    robot[n].ram[64]= robot[n].ram[64] | 2;
                }
                if (k>0)
                {
                    robot[n].ram[64]= robot[n].ram[64] | 4;
                }
                if ((get_val(n,robot[n].ip,2)==0) && (k==0))
                {
                    robot[n].ram[64]= robot[n].ram[64] | 8;
                }

                executed++;
                break;

            case 21: //(*TEST*)
                k = get_val(n,robot[n].ip,1) & get_val(n,robot[n].ip,2);
                robot[n].ram[64]= robot[n].ram[64] & 65520;
                if (k==get_val(n,robot[n].ip,2))
                {
                    robot[n].ram[64]= robot[n].ram[64] | 1;
                }
                if (k==0)
                {
                    robot[n].ram[64]= robot[n].ram[64] | 8;
                }
                executed++;
                break;

            case 22: //(*MOV, SET*)
                put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,2));
                executed++;
                break;

            case 23: //(*LOC*)
                put_val(n,robot[n].ip,1,robot[n].code.code[robot[n].ip].op[2]);
                time_used= 2;
                executed++;
                break;

            case 24: //(*GET*)
                k= get_val(n,robot[n].ip,2);
                if ((k>=0) && (k<=max_ram))
                {
                    put_val(n,robot[n].ip,1,robot[n].ram[k]);
                }

                else if ((k>max_ram) && (k<=(max_ram+1)+(((max_code+1) >> 3)-1)))
                {
                        j= k-max_ram-1;
                        put_val(n,robot[n].ip,1,robot[n].code.code[j << 2].op[j & 3]);
                }
                else
                {
                    robot_error(n,4,(k+""));
                }
                time_used= 2;
                executed++;
                break;

            case 25: //(*PUT*)
                k = get_val(n,robot[n].ip,2);
                if ((k>=0) && (k<=max_ram))
                {
                    robot[n].ram[k]= get_val(n,robot[n].ip,1);
                }

                else
                {
                    robot_error(n,4,(k + ""));
                }
                time_used= 2;
                executed++;
                break;

            case 26: //(*INT*)
                call_int(n,get_val(n,robot[n].ip,1),time_used);
                executed++;
                break;

            case 27: //(*IPO,IN*)
                time_used= 4;
                put_val(n,robot[n].ip,2,in_port(n,get_val(n,robot[n].ip,1),&time_used));
                executed++;
                break;

            case 28: //(*OPO,OUT*)
                time_used= 4;
                out_port(n,get_val(n,robot[n].ip,1),get_val(n,robot[n].ip,2),&time_used);
                executed++;
                break;

            case 29: //(*DEL,DELAY*)
                time_used= get_val(n,robot[n].ip,1);
                executed++;
                break;

            case 30: //(*PUSH*)
                push(n,get_val(n,robot[n].ip,1));
                executed++;
                break;

            case 31: //(*POP*)
                put_val(n,robot[n].ip,1,pop(n));
                executed++;
                break;

            case 32: //(*ERR*)
                robot_error(n,get_val(n,robot[n].ip,1),"");
                time_used= 0;
                executed++;
                break;

            case 33: //(*INC*)
                put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,1)+1);
                executed++;
                break;

            case 34: //(*DEC*)
                put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,1)-1);
                executed++;
                break;

            case 35: //(*SHL*)
                put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,1)>>get_val(n,robot[n].ip,2));
                executed++;
                break;

            case 36: //(*SHR*)
                put_val(n,robot[n].ip,1,get_val(n,robot[n].ip,1)<<get_val(n,robot[n].ip,2));
                executed++;
                break;

            case 37: //(*ROL*)
                put_val(n,robot[n].ip,1,rol(get_val(n,robot[n].ip,1),get_val(n,robot[n].ip,2)));
                executed++;
                break;

            case 38: //(*ROR*)
                put_val(n,robot[n].ip,1,ror(get_val(n,robot[n].ip,1),get_val(n,robot[n].ip,2)));
                executed++;
                break;

            case 39: //(*JZ*)
                time_used= 0;
                if ((robot[n].ram[64] & 8)>0)
                {
                    jump(n,1,inc_ip);
                }

                executed++;
                break;

            case 40: //(*JNZ*)
                time_used= 0;
                if ((robot[n].ram[64] & 8)==0)
                {
                    jump(n,1,inc_ip);
                }

                executed++;
                break;

            case 41: //(*JAE,JGE*)
                if (((robot[n].ram[64] & 1)>0) || ((robot[n].ram[64] & 4)>0))
                    jump(n,1,inc_ip);
                time_used= 0;
                executed++;
                break;

            case 42: //(*JBE,JLE*)
                if (((robot[n].ram[64] & 1)>0) || ((robot[n].ram[64] & 2)>0))
                {
                    jump(n,1,inc_ip);
                }

                time_used= 0;
                executed++;
                break;

            case 43: //(*SAL*)
                put_val(n,robot[n].ip,1,sal(get_val(n,robot[n].ip,1),get_val(n,robot[n].ip,2)));
                executed++;
                break;

            case 44: //(*SAR*)
                put_val(n,robot[n].ip,1,sar(get_val(n,robot[n].ip,1),get_val(n,robot[n].ip,2)));
                executed++;
                break;

            case 45: //(*NEG*)
                put_val(n,robot[n].ip,1,0-get_val(n,robot[n].ip,1));
                executed++;
                break;
            case 46: //(*JTL*)
                loc = get_val(n,robot[n].ip,1);
                if ((loc>=0) && (loc<=robot[n].plen))
                {
                    inc_ip= false;
                    robot[n].ip = loc;
                }
                else
                {
                    string temp = loc + "";
                    robot_error(n,2,temp);
                }
                break;
            default:
                robot_error(n,6,"");
                break;
        }
    }
    robot[n].delay_left+=time_used;
    if (inc_ip)
    {
        robot[n].ip++;
    }

    if (graphix && (n==0) && (step_mode>0))
    {
        update_debug_window();
    }

}}           //so, uh, this is weird


void do_robot(int n)
{
    int i,k,tthd;
    double heat_mult,ttx,tty;

    if ((n<0) || (n>max_robots))
    {
        return;
    }
    //with robot[n].^ do
    //{--time slice--}
    robot[n].time_left = time_slice;
    if ((robot[n].time_left>robot[n].robot_time_limit) && (robot[n].robot_time_limit>0))
    {
        robot[n].time_left = robot[n].robot_time_limit;
    }

    if ((robot[n].time_left>robot[n].max_time) && (robot[n].max_time>0))
    {
        robot[n].time_left=robot[n].max_time;
    }

    executed=0;

   //{--execute timeslice--}
    while ((robot[n].time_left>0) && (!robot[n].cooling) && (executed<20+time_slice) && (robot[n].armor>0))
    {
        if (robot[n].delay_left<0)
        {
            robot[n].delay_left=0;
        }
        if (robot[n].delay_left>0)
        {
            robot[n].delay_left--;
            robot[n].time_left--;
        }
        if ((robot[n].time_left>=0) && (robot[n].delay_left=0))
        {
            execute_instruction(n);
        }

        if (robot[n].heat>=robot[n].shutdown)
        {
            robot[n].cooling = true;
            robot[n].shields_up = false;
        }
        if (robot[n].heat>=500)
        {
            damage(n,1000,true);
        }

    }

    //{--fix up variables--}
    robot[n].thd = (robot[n].thd+1024) & 255;
    robot[n].hd = (robot[n].hd+1024) & 255;
    robot[n].shift = (robot[n].shift+1024) & 255;
    if (robot[n].tspd<-75)
    {
       robot[n].tspd = -75;
    }
    if (robot[n].tspd>100)
    {
        robot[n].tspd = 100;
    }
    if (robot[n].spd<-75)
    {
        robot[n].spd=-75;
    }
    if (robot[n].spd>100)
    {
        robot[n].spd=100;
    }
    if (robot[n].heat<0)
    {
        robot[n].heat=0;
    }
    if (robot[n].last_damage<maxint)
    {
        robot[n].last_damage++;
    }
    if (robot[n].last_hit<maxint)
    {
        robot[n].last_hit++;
    }

    //{--update heat--}
    if (robot[n].shields_up && ((game_cycle & 3)==0))
    {
        robot[n].heat++;
    }
    if (!robot[n].shields_up)
    {
        if (robot[n].heat>0)
        {
            switch (robot[n].config.heatsinks)//(*heat adjustments*)
            {
                case 5:
                    if ((game_cycle & 1)==0) //?????
                    {
                        robot[n].heat--;
                    }
                case 4:
                    if ((game_cycle % 3)==0)
                    {
                        robot[n].heat--;
                    }
                case 3:
                    if ((game_cycle & 3)==0)
                    {
                        robot[n].heat--;
                    }
                case 2:
                    if ((game_cycle & 7)==0)
                    {
                        robot[n].heat--;
                    }
            }
        }

        else if ((game_cycle & 3)==0)
        {
            robot[n].heat++;
        }

        if (robot[n].overburn && ((game_cycle % 3)==0))
        {
            robot[n].heat++;
        }

        if (robot[n].heat>0)
        {
            robot[n].heat--;
        }
        if ((robot[n].heat>0) && ((game_cycle & 7)==0) && (abso(robot[n].tspd)<=25))
        {
            robot[n].heat--;
        }
        if ((robot[n].heat<=robot[n].shutdown-50) || (robot[n].heat<=0))
        {
            robot[n].cooling = false;
        }
    }

    if (robot[n].cooling)
    {
        robot[n].tspd=0;
    }

    heat_mult=1;
    if(robot[n].heat>=80 && robot[n].heat<=99)
    {
        heat_mult=0.98;
    }
    if(robot[n].heat>=100 && robot[n].heat<=149)
    {
        heat_mult=0.95;
    }
    if(robot[n].heat>=150 && robot[n].heat<=199)
    {
        heat_mult=0.85;
    }
    if(robot[n].heat>=200 && robot[n].heat<=249)
    {
        heat_mult=0.75;
    }
    if(robot[n].heat>=250 && robot[n].heat<=maxint)
    {
        heat_mult=0.50;
    }

    if (robot[n].overburn)
    {
        heat_mult = heat_mult*1.30;
    }
    if ((robot[n].heat>=475) && ((game_cycle & 3)==0))
    {
        damage(n,1,true);
    }
    else if ((robot[n].heat>=450) && ((game_cycle & 7)==0))
    {
        damage(n,1,true);
    }
    else if ((robot[n].heat>=400) && ((game_cycle & 15)==0))
    {
        damage(n,1,true);
    }
    else if ((robot[n].heat>=350) && ((game_cycle & 31)==0))
    {
        damage(n,1,true);
    }
    else if ((robot[n].heat>=300) && ((game_cycle & 63)==0))
    {
        damage(n,1,true);
    }

    //{--update robot in physical world--}
    //{-acceleration-}
    if(abso(robot[n].spd-robot[n].tspd)<=acceleration)
    {
        robot[n].spd = robot[n].tspd;
    }

    else
    {
        if( robot[n].tspd>robot[n].spd)
        {
            robot[n].spd += acceleration;
        }
        else
        {
            robot[n].spd -= acceleration;
        }
    }
   //{-turning-}
    tthd = robot[n].hd + robot[n].shift;
    if ((abso(robot[n].hd-robot[n].thd)<=turn_rate) || (abso(robot[n].hd-robot[n].thd)>=256-turn_rate))
    {
        robot[n].hd = robot[n].thd;
    }
    else if( robot[n].hd != robot[n].thd)
    {
        k = 0;
        if (((robot[n].thd > robot[n].hd) && (abso(robot[n].hd-robot[n].thd)<=128)) || ((robot[n].thd<robot[n].hd) && (abso(robot[n].hd-robot[n].thd)>=128)))
        {
            k = 1;
        }
        if (k == 1)
        {
            robot[n].hd =(robot[n].hd+turn_rate) & 255;
        }
        else
        {
            robot[n].hd = (robot[n].hd+256-turn_rate) & 255;
        }
    }
    robot[n].hd = robot[n].hd & 255;
    if(robot[n].keepshift)
    {
        robot[n].shift = (tthd-robot[n].hd+1024) & 255;
    }

    //{-moving-}
    robot[n].speed = robot[n].spd/100*(max_vel*heat_mult*robot[n].speedadj);
    robot[n].xv = sint[robot[n].hd]*robot[n].speed;
    robot[n].yv =-cost[robot[n].hd]*robot[n].speed;
    if ((robot[n].hd==0) || (robot[n].hd==128))
    {
        robot[n].xv = 0;
    }

    if ((robot[n].hd=64) || (robot[n].hd=192))
    {
        robot[n].yv = 0;
    }
    if (robot[n].xv!=0)
    {
        ttx = robot[n].x+robot[n].xv;
    }
    else
    {
        ttx = robot[n].x;
    }

    if (robot[n].yv!=0)
    {
        tty = robot[n].y+robot[n].yv;
    }
    else
    {
        tty = robot[n].y;
    }

    if ((ttx<0) || (tty<0) || (ttx>1000) || (tty>1000))
    {
        robot[n].ram[8]++;
        robot[n].tspd=0;
        if (abso(robot[n].speed)>=max_vel/2)
        {
            damage(n,1,true);
        }
        robot[n].spd=0;
        //{ttx=x; tty=y;}
    }

    for (i =0; i < num_robots;i++)
    {
        if ((i!=n) && (robot[i].armor>0) && (distance(ttx,tty,robot[i].x,robot[i].y)<crash_range))
        {
            robot[n].tspd=0;
            robot[n].spd=0;
            ttx=robot[n].x;
            tty=robot[n].y;
            robot[i].tspd=0;
            robot[i].spd=0;
            robot[n].ram[8]++;
            robot[i].ram[8]++;
            if (abso(robot[n].speed)>=max_vel/2)
            {
                damage(n,1,true);
                damage(i,1,true);
            }
        }
    }

    if (ttx<0)
    {
        ttx=0;
    }
    if (tty<0)
    {
        tty=0;
    }
    if (ttx>1000)
    {
        ttx=1000;
    }
    if (tty>1000)
    {
        tty=1000;
    }
    robot[n].meter = robot[n].meter+distance(robot[n].x,robot[n].y,ttx,tty);
    if (robot[n].meter>=maxint)
    {
        robot[n].meter = robot[n].meter-maxint;
    }

    robot[n].ram[9]=trunc(robot[n].meter);
    robot[n].x=ttx;
    robot[n].y=tty;

   //{--draw robot--}
   if (robot[n].armor<0)
   {
       robot[n].armor = 0;
   }

   if (robot[n].heat<0)
   {
       robot[n].heat = 0;
   }
   if (graphix)
   {
        if (robot[n].armor!=robot[n].larmor)
        {
            //update_armor(n);                                                  //UPDATE THE GUI TO MATCH!!!!
        }

        if (robot[n].heat / 5.0 !=robot[n].lheat / 5.0)
        {
            //update_heat(n);                                               //UPDATE THE GUI TO MATCH!!!!
        }
        //draw_robot(n);                                                    //UPDATE THE GUI TO MATCH!!!!
   }

   robot[n].lheat = robot[n].heat;
   robot[n].larmor = robot[n].armor;

   //{--Cycle complete, inc counter--}
   robot[n].cycles_lived++;
}


void do_mine(int n, int m) //this is an indicator for when to explode and place mines, can be MUCH more efficient
{
  int i,j,k,l;
  double d,r;
  bool source_alive;
  //with robot[n].mine[m] do
  if ((robot[n].mine[m].x>=0) && (robot[n].mine[m].x<=1000) && (robot[n].mine[m].y>=0) && (robot[n].mine[m].y<=1000) && (robot[n].mine[m].yield>0))
  {
    for (i = 0; i < num_robots; i++)
//      if (robot[i].armor>0) and (i<>n) then     //WE NEED TO GET THIS IF STATEMENT FIXED
      {
        d = distance(robot[n].mine[m].x,robot[n].mine[m].y,robot[i].x,robot[i].y);
        if (d <= robot[n].mine[m].detect)
          robot[n].mine[m].detonate = true;
      }
      if (robot[n].mine[m].detonate)
      {
        init_missile(robot[n].mine[m].x,robot[n].mine[m].y,0,0,0,n,mine_circle,false);
        kill_count = 0;
        if (robot[n].armor > 0)
          source_alive = true;
        else
          source_alive = false;
      }
        for (i = 0; i < num_robots; i++)
          if ((i != n) && (robot[i].armor > 0))
          {
            k = round(distance(robot[n].mine[m].x,robot[n].mine[m].y,robot[i].x,robot[i].y));
            if (k < robot[n].mine[m].yield)
            {
              damage(i,round(abso(robot[n].mine[m].yield - k)),false);
              if ((n >= 0) && (n <= num_robots) && (i != n))
                robot[n].damage_total += round(abso(robot[n].mine[m].yield-k));
            }
          }
      if ((kill_count > 0) && (source_alive) && (robot[n].armor <= 0))
        kill_count--;
      if (kill_count > 0)
      {
        robot[n].kills += kill_count;
        update_lives(n);
      }
 /*if graphix then
  putpixel(round(x*screen_scale)+screen_x,round(y*screen_scale)+screen_y,0);
 yield = 0; x = -1; y = -1;
end
else
begin
 (* Draw mine *)
 if graphix and (game_cycle and 1=0) then
  begin
   main_viewport;
   setcolor(robot_color(n));
   line(round(x*screen_scale)+screen_x,round(y*screen_scale)+screen_y-1,
        round(x*screen_scale)+screen_x,round(y*screen_scale)+screen_y+1);
   line(round(x*screen_scale)+screen_x+1,round(y*screen_scale)+screen_y,
        round(x*screen_scale)+screen_x-1,round(y*screen_scale)+screen_y);
  end;
end;*/
  }
}

void do_missile(int n) //code for missle, will need to look at but can make more efficient
{
double llx,lly,r,d,dir,xv,yv;;
int i,j,k,l,xx,yy,tx,ty,dd,dam;
bool source_alive;
if (missile[n].a = 0)
  return;
else
{
  if (missile[n].a = 1)
  {
    if ((missile[n].x<-20) || (missile[n].x>1020) || (missile[n].y<-20) || (missile[n].y>1020))
      missile[n].a = 0;

    llx = missile[n].lx;
    lly = missile[n].ly;
    missile[n].lx = missile[n].x;
    missile[n].ly = missile[n].y;
    if (missile[n].a > 0)
    {
      missile[n].hd = ((missile[n].hd+256) & 255);
      missile[n].xv =  sint[missile[n].hd] * missile[n].mspd;
      missile[n].yv = -cost[missile[n].hd] * missile[n].mspd;
      missile[n].x = missile[n].x + missile[n].xv;
      missile[n].y = missile[n].y + missile[n].yv;
    }
    k = -1;
    l = maxint;
    for (i = 0; i < num_robots; i++)
      if ((robot[i].armor > 0) && (i != missile[n].source))
      {
        d = distance(missile[n].lx,missile[n].ly,robot[i].x,robot[i].y);
        dir = find_angle(missile[n].lx,missile[n].ly,robot[i].x,robot[i].y);
//        j = (round(dir/M_PI*128)+1024) & 255;       //WE NEED TO GET THIS TO WORK
        missile[n].hd = missile[n].hd & 255;
        xx = round(sint[missile[n].hd] * d + missile[n].lx);
        yy = round(-cost[missile[n].hd] * d + missile[n].ly);
        r = distance(xx,yy,robot[i].x,robot[i].y);
        if ((d <= missile[n].mspd) && (r < hit_range) && (round(d) <= l))
        {
          k = i;
          l = round(d);
          dd = round(r);
          tx = xx;
          ty = yy;
        }
      }
      if (k >= 0)
      {
        missile[n].x = tx;
        missile[n].y = ty;
        missile[n].a = 2;
        missile[n].rad = 0;
        missile[n].lrad = 0;
      }
      if ((missile[n].source > 0) && (missile[n].source < num_robots))
      {
        robot[missile[n].source].last_hit = 0;
        robot[missile[n].source].hits++;
      }
      for (i = 0; i < num_robots; i++)
      {
        dd = round(distance(missile[n].x,missile[n].y,robot[i].x,robot[i].y));
        if (dd <= hit_range)
        {
          dam = round(abso(hit_range - dd) * missile[n].mult);
          if (dam <= 0)
            dam = 1;
          kill_count = 0;
          if (robot[missile[n].source].armor > 0)
            source_alive = true;
          else
          {
            source_alive = false;
            damage(i,dam,false);
          }
          if ((missile[n].source > 0) && (missile[n].source < num_robots) && (i != missile[n].source))
            robot[missile[n].source].damage_total += dam;
          if ((kill_count > 0) && (source_alive) && (robot[missile[n].source].armor <= 0))
            kill_count--;
          if (kill_count > 0)
          {
            robot[missile[n].source].kills += kill_count;
            update_lives(missile[n].source);
          }
        }
      }
    }

//{--draw missile--}
/*if graphix then
 begin
  main_viewport;
  setcolor(0);
  line(round(llx*screen_scale)+screen_x,round(lly*screen_scale)+screen_y,
       round(lx*screen_scale)+screen_x,round(ly*screen_scale)+screen_y);
  if a=1 then
   begin
    if mult>robot[source]*.shotstrength then setcolor(14+(game_cycle and 1)) else setcolor(15);
    line(round(x*screen_scale)+screen_x,round(y*screen_scale)+screen_y,
         round(lx*screen_scale)+screen_x,round(ly*screen_scale)+screen_y);
   end;
 end;
end;
if (a=2) then
begin
lrad = rad;
inc(rad);
if rad>max_rad then a = 0;
if graphix then
 begin
  main_viewport;
  setcolor(0);
  circle(round(x*screen_scale)+screen_x,round(y*screen_scale)+screen_y,lrad);
  if mult>1 then setcolor(14+(game_cycle and 1)) else setcolor(15);
  if max_rad>=blast_circle then setcolor(14);
  if max_rad>=mine_circle then setcolor(11);
  if a>0 then
   circle(round(x*screen_scale)+screen_x,round(y*screen_scale)+screen_y,rad);
 end;
end;
end;*/
}}

string victor_string(int k, int n)
{
  string s = NULL;
  if (k = 1)
  {
      s = "Robot #";
      s += (n+1);
      s += " (";
      s += robot[n].fn;
      s+= ") wins!";
  }
  if (k = 0)
    s = "Simultaneous destruction, match is a tie.";
  if (k > 1)
    s = "No clear victor, match is a tie.";
  return s;
}
/*
procedure show_statistics; //was seen in another file, can use either function
var
i,j,k,n,sx,sy:integer;
begin
if not windoze then exit;
if graphix then
begin
sx = 24; sy = 93-num_robots*3;
viewport(0,0,639,479);
box(sx+000,sy,sx+591,sy+102+num_robots*12);
hole(sx+004,sy+004,sx+587,sy+098+num_robots*12);
setfillpattern(gray50,1);
bar(sx+005,sy+005,sx+586,sy+097+num_robots*12);
setcolor(15);
outtextxy(sx+016,sy+020,'Robot            Scored   Wins  Matches health  Kills  Deaths    Shots');
outtextxy(sx+016,sy+030,'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~');
n = -1; k = 0;
for i = 0 to num_robots do
with robot[i]^ do
if (armor>0) or (won) then begin inc(k); n = i; end;
for i = 0 to num_robots do
with robot[i]^ do
begin
 setcolor(robot_color(i));
 if (k=1) and (n=i) then j = 1 else j = 0;
 outtextxy(sx+016,sy+042+i*12,addfront(cstr(i+1),2)+' - '+addrear(fn,15)+cstr(j)
          +addfront(cstr(wins),8)+addfront(cstr(trials),8)
          +addfront(cstr(armor)+'%',9)+addfront(cstr(kills),7)
          +addfront(cstr(deaths),8)+addfront(cstr(match_shots),9));
end;
setcolor(15);
outtextxy(sx+016,sy+064+num_robots*12,victor_string(k,n));
if windoze then
begin
outtextxy(sx+016,sy+076+num_robots*12,'Press any key to continue...');
flushkey; readkey;
end;
end
else
begin
textcolor(15);
write(#13+space(79)+#13);
writeln('Match ',played,'/',matches,' results:');
writeln;
writeln('Robot            Scored   Wins  Matches  Armor  Kills  Deaths    Shots');
writeln('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~');
n = -1; k = 0;
for i = 0 to num_robots do
with robot[i]^ do
if (armor>0) or (won) then begin inc(k); n = i; end;
for i = 0 to num_robots do
with robot[i]^ do
begin
 textcolor(robot_color(i));
 if (k=1) and (n=i) then j = 1 else j = 0;
 writeln(addfront(cstr(i+1),2)+' - '+addrear(fn,15)+cstr(j)
        +addfront(cstr(wins),8)+addfront(cstr(trials),8)
        +addfront(cstr(armor)+'%',9)+addfront(cstr(kills),7)
        +addfront(cstr(deaths),8)+addfront(cstr(match_shots),9));
end;
textcolor(15);
writeln;
writeln(victor_string(k,n));
writeln;
{writeln('Press any key to continue...'); readkey;}
end;
end;
*/
void score_robots()
{
    int n,i,j,k,l;

    k = 0;
    n = -1;
    for (i = 0; i < num_robots; i++)
    {
        robot[i].trials++;
        if (robot[i].armor>0)
        {
            k++;
            n = i;
        }
    }
    if ((k=1) && (n>=0))
    {
        robot[n].wins++;
        robot[n].won = true;
    }
}

void init_bout() //just sets up the initial match, not necessary
{
  int i,j,k;
  game_cycle = 0;
  for (i = 0; i < max_missiles; i++)
  {//with missile[i]. do
    missile[i].a = 0;
    missile[i].source = -1;
    missile[i].x = 0;
    missile[i].y = 0;
    missile[i].lx = 0;
    missile[i].ly = 0;
    missile[i].mult = 1;
  }
  for (i = 0; i < num_robots; i++)
  //with robot[i]. do
  {
    robot[i].mem_watch = 128;
    reset_hardware(i);
    reset_software(i);
  }
/*  if graphix then setscreen;
  {FIFI}
  if graphix and (step_mode>0) then init_debug_window;
  {/FIFI}
  if not graphix then
  begin
  textcolor(7);
  {writeln(#13+'Match ',played,'/',matches,', Battle in progress...');
  writeln;}
  end;*/
}


void toggle_graphix()
{
  graph_mode(!graphix);
  if (!graphix)
  {
//        textcolor(7);
      cout<<"Match "<<played<<"/"<<matches<<", Battle in progress..."<<endl<<endl;
  }
  else
  {
      setscreen();
  }
}
void bout() //this is the main function, it's keeping timers and setting everything up when it needs to be set up, so probably vital but can be changed to something better
{
  int i,j,k;
  char c;
  float timer;
  int n;// {loop count for mem_watch}
  if (quit)
//    exit();       THIS DOES NOT EXIST IN C++
  played++;
  init_bout();
  bout_over = false;


  if (step_mode = 0)
    step_loop = false;
  else
    step_loop = true;
  step_count = -1;
  if ((graphix) && (step_mode>0))
    for (i = 0; i < num_robots; i++)
//      draw_robot(i);

  do{
    game_cycle++;
    for (i = 0; i < num_robots; i++)
      if (robot[i].armor > 0)
        do_robot(i);
    for (i = 0; i < max_missiles; i++)
      if (missile[i].a > 0)
        do_missile(i);
    for (i = 0; i < num_robots; i++)
      for (k = 0; k < max_mines; k++)
        if (robot[i].mine[k].yield>0)
          do_mine(i,k);

    if (graphix && timing)
      time_delay(game_delay);

/*    if keypressed then c = upcase(readkey) else c = #255;
    case c of
    {FIFI}
    'X': if not robot[0]^.is_locked then
        begin
         if not graphix then toggle_graphix;
         if robot[0]^.health>0 then
          begin
           if temp_mode>0 then step_mode = temp_mode else step_mode = 1;
           step_count = -1;
           init_debug_window;
          end;
        end;
    {/FIFI}
    '+','=':if game_delay<100 then
           case game_delay of
             000..004:game_delay = 005;
             005..009:game_delay = 010;
             010..014:game_delay = 015;
             015..019:game_delay = 020;
             020..029:game_delay = 030;
             030..039:game_delay = 040;
             040..049:game_delay = 050;
             050..059:game_delay = 060;
             060..074:game_delay = 075;
             075..100:game_delay = 100;
           end;
    '-','_':if game_delay>0 then
           case game_delay of
             000..005:game_delay = 000;
             006..010:game_delay = 005;
             011..015:game_delay = 010;
             016..020:game_delay = 015;
             021..030:game_delay = 020;
             031..040:game_delay = 030;
             041..050:game_delay = 040;
             051..060:game_delay = 050;
             061..075:game_delay = 060;
             076..100:game_delay = 075;
           end;
    'G':toggle_graphix;
    else process_keypress(c);
    end; flushkey;
    if game_delay<0 then game_delay = 0;
    if game_delay>100 then game_delay = 100;
    case game_delay of
    000..001:k = 100;
    002..005:k = 50;
    006..010:k = 25;
    011..025:k = 20;
    026..040:k = 10;
    041..070:k = 5;
    071..maxint:k = 1;
    else k = 10;
    end;
    if not graphix then k = 100;
    if graphix then
    begin
    if ((game_cycle mod k)=0) or (game_cycle=10) then update_cycle_window;
    end else begin
    if (update_timer<>mem[0:$46C] shr 1) then update_cycle_window;
    update_timer = mem[0:$46C] shr 1;
  }*/
}
  while(quit || gameover || bout_over);

  update_cycle_window();
  score_robots();
//  show_statistics();      FUNCTION DOES NOT EXIST
}

void write_report()
{
    int i,j,k;
    //f:text;

    /*assign(f,main_filename+report_ext);
    rewrite(f);
    writeln(f,num_robots+1);*/
    for (i = 0; i < num_robots; i++)
    //with robot[i]. do
    {

        switch (report_type)
        {
            case 2:cout<<robot[i].wins<<" "<<robot[i].trials<<" "<<robot[i].kills<<" "<<robot[i].deaths<<" "<<robot[i].fn<<" "<<endl;
            break;
            case 3:cout<<robot[i].wins<<" "<<robot[i].trials<<" "<<robot[i].kills<<" "<<robot[i].deaths<<" "<<robot[i].armor<<" "<<
                        robot[i].heat<<" "<<robot[i].shots_fired<<" "<<robot[i].fn<<" "<<endl;
            break;
            case 4:cout<<robot[i].wins<<" "<<robot[i].trials<<" "<<robot[i].kills<<" "<<robot[i].deaths<<" "<<robot[i].armor<<" "<<
                        robot[i].heat<<" "<<robot[i].shots_fired<<" "<<robot[i].hits<<" "<<robot[i].damage_total<<" "<<
                        robot[i].cycles_lived<<" "<<robot[i].error_count<<" "<<robot[i].fn<<" "<<endl;
            break;
            default: cout<<robot[i].wins<<" "<<robot[i].trials<<" "<<robot[i].fn<<" "<<endl;
        }
    }
    /*close(f);*/
}

/*
procedure begin_window;
var
s:string;
begin
if (not graphix) or (not windoze) then exit;
setscreen;
viewport(0,0,639,479);
box(100,150,539,200);
hole(105,155,534,195);
setfillpattern(gray50,1);
bar(105,155,534,195);
setcolor(15);
s = 'Press any key to begin!';
outtextxy(320-((length(s) shl 3) shr 1),172,s);
flushkey; readkey;
setscreen;
end;
*/
int main(void)
{
  int i, j, k, l, n, w;
  srand((unsigned) time(0)); //needed for random locations
  init();
  if (graphix) //GUI, just made it c++ code
//    begin_window();       FUNCTION DOES NOT EXIST
  if (matches > 0){
   for(i = 0; i < matches; i++)
    bout();
  }
  if (!graphix)
    cout<<endl; //no idea what this does
  if (quit)
//    exit();           FUNCTION DOES NOT EXIST
  if (matches > 1) { //Calculate overall statistics
    cout<<endl<<endl;
    graph_mode(false);
//    textcolor(15); //GUI
    cout<<"Bout complete! ("<<matches<<" matches)"<<endl;
    cout<<endl;
    k = 0;
    w = 0;
    for(i = 0; i < num_robots; i++){
     //need robot[i]^
       if (robot[i].wins = w) //packet_robot ???
          k++;
       if (robot[i].wins > w)
       {
          k = 1;
          n = i;
          w = robot[i].wins;
        }
      }
    cout<<"Robot           Wins  Matches  Kills  Deaths    Shots"<<endl;
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    for (i = 0; i < num_robots; i++)
//     with robot[i]^ do
      {
//       textcolor(robot_color(i)); //GUI
//       writeln(addfront(cstr(i+1),2)+' - '+addrear(fn,8)
//              +addfront(cstr(wins),7)+addfront(cstr(trials),8)
//              +addfront(cstr(kills),8)+addfront(cstr(deaths),8)
//              +addfront(cstr(shots_fired),9));
      }
//    textcolor(15); //GUI
    cout<<endl;
//    if (k = 1)
        //writeln('Robot #',n+1,' (',robot[n].fn,') wins the bout! (score: ',w,'/',matches,')')
//    else
        //writeln('There is no clear victor!');
    //writeln;
  if (graphix)
  {
    graph_mode(false);
//    show_statistics();        FUNCTION DOES NOT EXIST
  }
  if (report)
    write_report();
  shutdown();
}
}
