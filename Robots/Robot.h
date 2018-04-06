#include <string>
#include <cstdlib>

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
    int shields, scanrange, scanarc, accuracy;
    int x, y; //Robot coordinates
    int max_sonar, max_missiles, max_mines, max_queue, missile_spd, hit_range; //details
    int mines, shield, heatsinks;
    int kills, wins, deaths, hits, shots, trials; //stats variables, can add/remove
    double shotstrength, damageadj, speedadj, health;
    int turretAngle;
    int arraySize;
    int tarx = 0, tary = 0;
    string Name;
    int tarName;
    string compiledFile;
    int attackNum;
    int mem_watch;
    int lx, ly, xv, yv, meters, hd, thd, lhd, spd, tspd, larmor, lheat, ip, plen; //misc variables that can be removed if not needed
    int shift, err, delay_left, robot_time_limit,max_time, time_left;
    int lshift, arc_count, sonar_count, last_damage, last_hit, transponder, shutdown, channel;
    int lendarc, endarc, lstartarc, startarc, mines;
    int tx, ltx, ty, lty, startkills, shots_fired, match_shots, damage_total, cycles_lived;
    int error_count, config_rec;
    bool shields_up, lshields, overburn, keepshift, cooling, won;

    Robot(); //creates an empty robot
    Robots(string, string); //initializes robot
    void robot_config(int, int, int, int, int, int, int);//note int order for this
    void set_AttackArray();
    string sendAttack();

  private:
    bool isLocked;
};

#endif // Robot
