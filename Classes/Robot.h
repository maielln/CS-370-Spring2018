#include <string>
#include <cstdlib>
#include "SimpleSwitch.h"

#ifndef ROBOT
#define ROBOT

//Contributor(s): Nicole Maiello
//shell of robot, holds all variables

using namespace std;

class Robot{

  public:
    static const int max_ram = 1023, max_stack = 256; //ram is going to hold all registers
    uint16_t RAM[max_ram]; //ram for 16 bit integers
    string Stack[max_stack];
    int acceleration, turn_rate, crash_range, speed; //robot moving
    int shields, overburn, keepshift, cooling, scanrange;
    int x, y; //Robot coordinates
    int max_sonar, max_missiles, max_mines, max_queue, missile_spd, hit_range; //details
    int mines, shield, heatsinks;
    int kills, wins, deaths, hits, shots; //stats variables, can add/remove
    double shotstrength, damageadj, speedadj, health;
    int turretAngle;
    int arraySize;
    int tarx = 0, tary = 0;
    bool shields_up;
    string Name;
    int tarName;
    string compiledFile;
    int attackNum;



    Robots(string, string); //initializes robot
    void robot_config(int, int, int, int, int, int, int);//note int order for this
    void set_AttackArray();
    string sendAttack();

  private:
    bool isLocked;
};

#endif // Robot
