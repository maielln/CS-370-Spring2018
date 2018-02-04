/*File Name: Robot.cpp
  Contributer(s): Nicole Maiello
  Date: 2/3/2018
  Language: C++ (gcc compiler)
  Version 1.01
  Description: Currently not finished, will contain multiple variables and info for each
  robot. Just a concept right now.
*/
#include <string>
using namespace std;

int wins = 0;
int trials = 0;
int kills = 0;
int deaths = 0;
int shots_fired = 0;
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
//this is initialized in init_robot with robot_config

void Robot(int n, string fileName)
{
  //here will initialize all global variables from the compile file
  //porting create_robot from ATR2.PAS
  if(maxavail < )

  
}

void init_Robot(int n)
{
  for(int i = 0; i < max_ram; i++)
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

  reset_hardware(n);
  reset_software(n);
  //if we look to include these funcitons, it should probably be placed in a different file?
}

string getName() //function to retrieve name for scoreboard and robot display
{
  return name;
}

int getWins() //function to retrieve wins for scoreboard and posible robot display
{
  return wins;
}

int getTrials() //function to retrive trials assumably for number of games lost
{
  return trials;
}

int getKills() //function to retrieve kills for scoreboard
{
  return kills;
}

int getDeaths() //fucntion to retrieve deaths for scoreboard
{
  return deaths;
}

int getShotsFired() //function to retrieve number of shots fired as a whole, can be used in stats for nerds
{
  return shots_fired;
}

int getMatchShots() //function to retrieve number of shots fired in a given match, can be used in stats for nerds
{
  return match_shots;
}

int getHits() //assuming will be returning the number of times the robot was hit, stats for nerds and scoreboard
{
  return hits;
}

double getDamageTotal() //returns number of damage taken, scoreboard and stats for nerds
{
  return damage_total;
}

int getCyclesLived() //returns the number of cycles the robot has survived, does not have to be in a row
{
  return cycles_lived;
}

int getErrorCount() //returns the number of errors encountered during cycles, stats for nerds if used
{
  return error_count;
}
//got tired of returning all values so will continue this later if we want it
