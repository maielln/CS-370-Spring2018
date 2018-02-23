/*File Name: Robot.cpp
  Contributer(s): Nicole Maiello
  Date: 2/3/2018
  Language: C++ (gcc compiler)
  Version 1.01
  Description: Currently not finished, will contain multiple variables and info for each
  robot. Just a concept right now
*/
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

#include "Robot.h"

/*int wins = 0;
int trials = 0;
int kills = 0;
int deaths = 0;
int match_shots = 0;
int hits = 0;
double damage_total = 0;
int cycles_lived = 0;
int error_count = 0;
//each of the variables above has a function to give the values of each statistic
int plen = 0;
double max_time = 0;


string name = "", fn = "";
double arc_count = 0, sonar_count = 0;
double robot_time_limit = 0;
int scanrange = 1500;
double shotstrength = 1;
double damageadj = 1, speedadj = 1;
int mines = 0;
bool shields_up; //this was taken from robot_config in ATR2.PAS
//these variables were taken from init_robot in ATR2.PAS

int scanner = 5;
int weapon = 2;
int armor = 2;
int engine = 2;
int heatsinks = 1;
int shield = 0;
//this is initialized in init_robot with robot_config*/
//above are gathered variables

//below variables are declared in the header
//int max_ram, max_labels, max_vars, max_op; //hardware data pieces
//int acceleration, turn_rate, crash_range, speed; //robot moving
//int shields, overburn, keepshift, cooling, scanrange;
//int max_sonar, max_missiles, max_mines, max_queue, missile_spd, hit_range; //details
//int mines;
//int kills, wins, deaths, hits; //stats variables, can add/remove
//double shotstrength, damageadj;
//bool shields_up;

//all variables can be accessed outside of the class, so all variables
//can be incremented and utilized outside without update and get functions

Robot::Robot(string fileName) //basically the create_robot (line 1305)
{
  //here will initialize all global variables from the compile file
  //porting create_robot from ATR2.PAS
  //instead of passing individual variables, use a typeDef array and put
  //each variable in a specific slot and initialize it in this function
    Name = fileName;
    cout<<Name<<" has been created."<<endl;
    srand((unsigned) time(0));
    x = rand() % 100; //base code for now, getting a random number then having it go from 0-100
    y = rand() % 100; //same as above but for y
    cout<<x<<" "<<y<<endl;
    max_ram = 1023;
    RAM[max_ram];
    health = 100;



}

void Robot::init_Robot()
{
 /* for(int i = 0; i < max_ram; i++)
  {
    ram[i] = 0; //frees up all memory
  }

  ram[71] = 768; //not sure why this is needed but ok

  for(i = 0; i < max_code; i++)
  {
    for(int k = 0; k < max_op; k++)
    {
      code[i].op[k] = 0; //taking what operation was determined and resetting it?
    }
  }

  reset_hardware(n); //line 1220 in my files
  reset_software(n); //line 1202 in my files
  //if we look to include these funcitons, it should probably be placed in a different file?
    */
}

void Robot::robot_config(int scan, int weap, int arm, int eng, int mine, int shielding, int heat) //line 1149 in my file
{
  //the original code uses switch cases to update scanrange and other configs
  //robot[n]^ is the singular robot, since this is a class of a single robot, we will already be in that robot's data
  //because we are already in the data, have the numbers of config passed to this function
  switch(scan){ //configures the scan range
    case 5:
      scanrange = 1500;
      break;
    case 4:
      scanrange = 1000;
      break;
    case 3:
      scanrange = 700;
      break;
    case 2:
      scanrange = 500;
      break;
    case 1:
      scanrange = 350;
      break;
    default:
      scanrange = 250;
      break;
  }
  switch(weap){
    case 5:
      shotstrength = 1.5;
      break;
    case 4:
      shotstrength = 1.35;
      break;
    case 3:
      shotstrength = 1.2;
      break;
    case 2:
      shotstrength = 1;
      break;
    case 1:
      shotstrength = 0.8;
      break;
    default:
      shotstrength = 0.5;
      break;
  }
  switch(arm){
    case 5:
      damageadj = 0.66;
      speedadj = 0.66;
      break;
    case 4:
      damageadj = 0.77;
      speedadj = 0.75;
      break;
    case 3:
      damageadj = 0.83;
      speedadj = 0.85;
      break;
    case 2:
      damageadj = 1;
      speedadj = 1;
      break;
    case 1:
      damageadj = 1.5;
      speedadj = 1.2;
      break;
    default:
      damageadj = 2;
      speedadj = 1.33;
  }
  switch(eng){
    case 5:
      speedadj = speedadj * 1.5;
      break;
    case 4:
      speedadj = speedadj * 1.35;
      break;
    case 3:
      speedadj = speedadj * 1.2;
      break;
    case 2:
      speedadj = speedadj * 1;
      break;
    case 1:
      speedadj = speedadj * 0.8;
      break;
    default:
      speedadj = speedadj * 0.5;
      break;
  }
  switch(mine){
    case 5:
      mines = 24;
      break;
    case 4:
      mines = 16;
      break;
    case 3:
      mines = 10;
      break;
    case 2:
      mines = 6;
      break;
    case 1:
      mines = 4;
      break;
    default:
      mines = 2;
      //config.mines = 0; not entirely sure what this is
      break;
  }
  shields_up = false;

  if((shielding < 3) || (shielding > 5))
    shield = 0;
  if((heat < 0) || (heat > 5)) //code states that heatsingks are handled separately
    heatsinks = 0; //will come back to
}

void Robot::set_coordinates(int x1, int y1)
{
  x = x1;
  y = y1;
}
