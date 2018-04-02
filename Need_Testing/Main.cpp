#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <string.h>
#include "Robot.h"
#include "fOpen.h"
//#include "Missile.h"      //include these files later
//#include "Mine.h"

//including old constants from the original code
//also ported over the old comments while removing the FIFI ones
#define    show_code false
#define    compile_by_line false
#define    max_var_len 16
#define    debugging_compiler false
//{robots}
#define max_robots 31    //{starts at 0, so total is max_robots+1}
#define max_code 1023  //{same here}
#define max_op 3     //{etc...}
#define stack_size 256
#define stack_base 768
#define max_ram 1023  //{but this does start at 0 (odd #, 2^n#define1)}
#define max_vars 256
#define max_labels 256
#define acceleration 4
#define turn_rate 8
#define max_vel 4
#define max_missiles 1023
#define missile_spd 32
#define hit_range 14
#define blast_radius 25
#define crash_range 8
#define max_sonar 250
#define com_queue 512
#define max_queue 255
#define max_config_points 12
#define max_mines 63
#define mine_blast 35

 //{simulator & graphics}
#define screen_scale 0.46
#define screen_x 5
#define screen_y 5
#define robot_scale 06
#define default_delay 20
#define default_slice 5
#define mine_circle (int)(mine_blast*screen_scale)+1
#define blast_circle (int)(blast_radius*screen_scale)+1
#define mis_radius (int)(hit_range/2)+1
#define max_robot_lines 8
#define pi 3.141592653589793238462643

#define progname "AT-Robots-SD";
#define version "1.00";
#define cnotice1 "Copyright 1997 ""99, Ed T. Toton III";
#define cnotice2 "All Rights Reserved.";
#define cnotice3 "Copyright 2014, William 'Amos' Confer";
#define main_filename "ATR2";
#define robot_ext ".AT2";
#define locked_ext ".ATL";
#define config_ext ".ATS";
#define compile_ext ".CMP";
#define report_ext ".REP";

using namespace std;

//{--robot variables--}
 int num_robots;
// Robot robotarray = new Robot[max_robots+2]; 
// Missle missilearray = new Missile[max_missiles];

// {--compiler variables--}
// ftext;
int numvars,numlabels,maxcode,lock_pos,lock_dat;
string varnamearray[max_vars],reg_name;
int varlocarray[max_vars];
string labelnamearray[max_vars];
int labelnumarray[max_labels];
bool show_source,compile_only;
string lock_code;

 //{--simulator/graphics variables--}
bool bout_over;     //{made global from procedure bout}
int step_mode;     //{0=off, for (0<step_mode<=9) = #of game cycles per step}
int temp_mode;     //{stores previous step_mode for return to step}
int step_count;    //{step counter used as break flag}
bool step_loop;     //{break flag for stepping}
 //{bool show_debugger;} {flag for viewing debugger panel vs. robot stats}

bool old_shields,insane_missiles,debug_info,windoze,no_gfx,graphix,sound_on;
bool logging_errors,timing,show_arcs,registered;
int game_delay,time_slice,insanity,update_timer,max_gx,max_gy,stats_mode,delay_per_sec,reg_num,paramcount;
long game_limit,game_cycle;
long matches,played,executed;

 //{--general settings--}
bool quit,report,show_cnotice;
int kill_count,report_type;
double sint[256],cost[256];

//Displays an error if it occurs taking in a string and prints it accordingly.
//It is just used for debugging.
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

//initializes the variables above before the game is done.
//Only run it when you want all the variables to be the starting value.
//A lot is commented out, because some isn't currently in yet.
void init()
{
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
    for( int i = 0; i < 255; i++ )
    {
        sint[i] = sin(i/128*pi);
        cost[i] = cos(i/128*pi);
    }
    num_robots = -1;
    game_limit = 100000;
    game_cycle = 0;
    game_delay = default_delay;
    time_slice = default_slice;
/*    for (int i = 0; i < max_missiles;i++)
    {
        missile[i].a = 0;
        missile[i].source = -1;
        missile[i].x = 0;
        missile[i].y = 0;
        missile[i].lx = 0;
        missile[i].ly = 0;
        missile[i].mult = 1;
    }
*/
    registered = false;
    reg_name = "Unregistered";
    reg_num = 65535;
//    check_registration;

    cout<<endl;
//    textcolor(3);
//    cout<<progname<<" "<<version<<" "<<endl;
//    cout<<cnotice1<<endl;
//    cout<<cnotice2<<endl;
 //textcolor(7);
 //if not registered then begin textcolor(4); writeln('Unregistered version'); end
 //                  else writeln('Registered to: ',reg_name);
 //textcolor(7);
 //writeln;
 //{create_robot(0,'SDUCK');}

//    delete_compile_report();  //????????????????????????????????????????????????? Need later????
    if (paramcount>0)
    {
        for (int i = 1; i < paramcount; i++)
        {
 //           parse_param(btrim(ucase(paramstr(i))));
        }
    }

    else
    {
//        prog_error(5,"");
    }

    temp_mode = step_mode;  //{store initial step_mode}

    if (logging_errors)
    {
        for (int i = 0; i<num_robots; i++)     //check what this is
        {
 //           robot[i].errorlog = robot[i].base_name(fn)+'.ERR';
        }
    }

    if (compile_only)
    {
//        write_compile_report();
    }
    if (num_robots<1)
    {
 //       prog_error(4,"");
    }

/*  if (!no_gfx)
    {
        graph_mode(true);
    }
*/
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
//    if (maxcode()>max_code)
//    {
//        maxcode = max_code();
//    }

    //{--Just to avoid floating pointers--}
    for ( int i = 0; i<max_robots ;i++)
    {
//        robot[i] = new Robot(/*Whatever we want to use to make a placeholder*/);
    }

    return;
}

//have fun testing!!!
int main()
{
    init();
    return 0;
}
