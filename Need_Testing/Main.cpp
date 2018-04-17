#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <string.h>
#include <ctime>
#include "Robot.h"
#include "fOpen.h"
#include "Missile.h"
//#include "Mine.h"         //include these files later

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
#define maxint 32787

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
#define minint -32768

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
 Robot robot[max_robots];
 Missile missile[max_missiles];

// {--compiler variables--}
// ftext;
int numvars,numlabels,maxcode,lock_pos,lock_dat;
string varname[max_vars],reg_name;
int varloc[max_vars];
string labelname[max_vars];
int labelnum[max_labels];
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
Mine mine[600];

//string ltrim(string s1);
//string rtrim(s1 string);
//string btrim(string s1);
double distance(int x1, int y1, int x2, int y2);
void damage(int n,int d,bool physical);
void robot_error(int n,int i,string ov);
void do_robot(int n);
void prog_error(int n, string ss);
void execute_instruction(int n);
void init_missile(double xx, double yy, double xxv, double yyv,int dir,int s,int blast,bool ob);
string addrear(string b,int l);
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
int rol(int n,int k);
int ror(int n,int k);
int sal(int n,int k);
int sar(int n,int k);
bool gameover();
int get_val(int n,int c,int o);
int get_from_ram(int n,int i,int j);
int find_label(int n,int l,int m);
void put_val(int n,int c,int o,int v);
int pop(int n);
void push(int n,int v);
void jump(int n,int o, bool inc_ip);
void call_int(int n,int int_num, int time_used);
double find_angle(double xx,double yy,double tx,double ty);
void reset_software(int n);
void com_transmit(int n,int chan,int data);
int com_receive(int n);
int in_port(int n,int p);
int scan(int n);
int find_anglei(double xx,double yy,double tx,double ty);
void out_port(int n,int p,int v, int* time_used);

/*string ltrim(string s1)
{
    while ((s1.length()>0) && ((s1.substr(0,1)==" ") || (s1.substr(1,1)==(string)((char)(8))) || (s1.substr(0,1)==(string((char)(9))))
    {
        s1 = s1.substr(1,s1.length()-1);
    }
    return s1;
}

string rtrim(s1 string)
{
    while ((s1.length()>0) && ((s1.substr(s1.length()-1,1)==" ") || (s1.substr(s1.length()-1,1)==(char)(8)) || (s1.substr(s1.length(),1)==(char)(9))))
    {
        s1 = s1.substr(0,s1.length()-1);
    }
    return s1;
}

string btrim(string s1)
{
    return ltrim(rtrim(s1));
}
*/

string operand(int n,int m)
{
    string s;
    s = n;

    switch (m & 7)
    {
        case 1:s = "@"+s;
            break;
        case 12:s = ":"+s;
            break;
        case 13:s = "$"+s;
            break;
        case 14:s = "!"+s;
            break;
        default: s = n;
            break;
    }
    if (m && 8>0)
    {
        s = "["+s+"]";
    }
    return s;
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

string mnemonic(int n,int m)
{
    string s;
    s = n;
    if (m==0)
    {
        switch (n)
        {
            case 0:s="NOP";
                break;
            case 1:s="ADD";
                break;
            case 2:s="SUB";
                break;
            case 3:s="OR";
                break;
            case 4:s="AND";
                break;
            case 5:s="XOR";
                break;
            case 6:s="NOT";
                break;
            case 7:s="MPY";
                break;
            case 8:s="DIV";
                break;
            case 9:s="MOD";
                break;
            case 10:s="RET";
                break;
            case 11:s="CALL";
                break;
            case 12:s="JMP";
                break;
            case 13:s="JLS";
            break;
            case 14:s="JGR";
                break;
            case 15:s="JNE";
                break;
            case 16:s="JE";
                break;
            case 17:s="SWAP";
                break;
            case 18:s="DO";
                break;
            case 19:s="LOOP";
                break;
            case 20:s="CMP";
                break;
            case 21:s="TEST";
                break;
            case 22:s="MOV";
                break;
            case 23:s="LOC";
                break;
            case 24:s="GET";
                break;
            case 25:s="PUT";
                break;
            case 26:s="INT";
                break;
            case 27:s="IPO";
                break;
            case 28:s="OPO";
                break;
            case 29:s="DELAY";
                break;
            case 30:s="PUSH";
                break;
            case 31:s="POP";
                break;
            case 32:s="ERR";
                break;
            case 33:s="INC";
                break;
            case 34:s="DEC";
                break;
            case 35:s="SHL";
                break;
            case 36:s="SHR";
                break;
            case 37:s="ROL";
                break;
            case 38:s="ROR";
                break;
            case 39:s="JZ";
                break;
            case 40:s="JNZ";
                break;
            case 41:s="JGE";
                break;
            case 42:s="JLE";
                break;
            case 43:s="SAL";
                break;
            case 44:s="SAR";
                break;
            case 45:s="NEG";
                break;
            case 46:s="JTL";
                break;
            default: s="XXX";
                break;
        }
    }
    else
    {
        s = operand(n,m);
    }

    return s;
}

double find_angle(double xx,double yy,double tx,double ty)
{
    int v,z;
    double q;
    q=0;
    v=abs(round(tx-xx));
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
        if ((i!=n) && (robot[i].health>0))
        {
            j = find_anglei(robot[n].x,robot[n].y,robot[i].x,robot[i].y);
            d = distance(robot[n].x,robot[n].y,robot[i].x,robot[i].y);
            k = round(d);
            if ((k<range) && (k<=robot[n].scanrange) && ((abs(j-dir)<=abs(robot[n].scanarc)) ||
                        (abs(j-dir)>=256-abs(robot[n].scanarc))))
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
                        acc = abs(j-dir)/robot[n].scanarc*2;
                        if (sign<0)
                        {
                            robot[n].accuracy=-abs(round(acc));
                        }

                        else
                        {
                            robot[n].accuracy = abs(round(acc));
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
        case 6:v = robot[n].health;
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
                if ((n!=i) && (j<k) && (robot[i].health>0))
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
                if ((n!=i) && (j<k) && (j<max_sonar) && (robot[i].health>0))
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
            if (robot[n].shield>0)
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

void init_mine(int n,int detectrange,int sIze)
{
    int i,k;

    //with robot[n]. do

    k = -1;
    for (i = 0; i < max_mines; i++)
    {
        if (((mine[i].x<0) || (mine[i].x>1000) || (mine[i].y<0) || (mine[i].y>1000)
                || (mine[i].yield<=0)) && (k<0))
        {
            k = i;
        }
        if (k>=0)
        {
            mine[k].x = robot[n].x;
            mine[k].y = robot[n].y;
            mine[k].detect = detectrange;
            mine[k].yield = sIze;
            mine[k].detonate = false;
            //click;    make noise
        }
    }
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
        case 17:robot[n].scanarc = v;
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
            if (config.mines>=0)
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

void com_transmit(int n,int chan,int data)
{
    int i;

    for (i=0; i < num_robots; i++)
    {
        //with robot[i]. do
        if ((robot[i].health > 0) && (i!=n) && (robot[i].channel==chan))
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
    robot[n].meters = 0;
    robot[n].delay_left = 0;
    robot[n].time_left = 0;
    robot[n].shields_up = false;
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
                if (robot[i].health>0)
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
                robot[n].meters = 0;
        break;
        default: robot_error(n,10,int_num+"");
    }

}

void jump(int n,int o, bool inc_ip)
{
    int loc;
    //with robot[n]. do

    loc = find_label(n,get_val(n,robot[n].ip,o),robot[n].code[robot[n].ip].op[max_op] >> (o*4));
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


void put_val(int n,int c,int o,int v)
{
    int i,j,k;

    k = 0;
    i = 0;
    j = 0;
    //with robot[n]. do

    j = (robot[n].code[c].op[max_op] >> (4*o)) & 15;
    i = robot[n].code[c].op[o];
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
                j = robot[n].code[i].op[max_op] & 15;
                if ((j==2) && (robot[n].code[i].op[0]==l))
                {
                    k = i;
                }
            }
        }
    }

    return k;
}
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
            k = robot[n].code[l >> 2].op[l & 3];
        }
    }

    return k;
}

int get_val(int n,int c,int o)
{
    int i,j,k;
    k = 0;
    //with robot[n]. do

    j = (robot[n].code[c].op[max_op] >> (4*o)) & 15;
    i = robot[n].code[c].op[o];
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
            if (robot[n].health>0)
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

string addrear(string b,int l)
{
    while ((int)(b.length())< l)
    {
        b = b+' ';
    }
    return b;
}

double distance(int x1, int y1, int x2, int y2)
{
    return pow((pow(x2-x1,2) + pow(y2-y1,2)),.5);
}

int max_shown()
{
    int max_show = 0;
    switch (stats_mode)
    {
        case 1:max_show = 12;
        case 2:max_show = 32;
        default: max_show =6;
    }
    return max_show;
}

bool invalid_microcode(int n,int ip)
{
    bool invalid;
    int i,k;
    invalid = false;
    for( i = 0;i < 2; i++)
    {
        k = (robot[n].code[ip].op[max_op] >> (i << 2)) & 7;
        if (!((k == 0) || (k == 1) || (k ==2) || (k ==4)))
        {
            invalid = true;
        }
    }
    return invalid;
}

bool graph_check(int n)
{
    bool ok;
    ok = true;
    if ((!graphix) || (n<0) || (n>num_robots) || (n>=max_shown()))
    {
        ok = false;
    }
    return ok;
}

void log_error(int n,int i,string ov)
{
    string s;
    if (!logging_errors)
    {
        return;
    }
    //with robot[n]^ do
    switch (i)
    {
        case 1:s="Stack full - Too many CALLs?";
            break;
        case 2:s="Label not found. Hmmm.";
            break;
        case 3:s="Can""t assign value - Tisk tisk.";
            break;
        case 4:s="Illegal memory reference";
            break;
        case 5:s="Stack empty - Too many RETs?";
            break;
        case 6:s="Illegal instruction. How bizarre.";
            break;
        case 7:s="Return out of range - Woops!";
            break;
        case 8:s="Divide by zero";
            break;
        case 9:s="Unresolved !label. WTF?";
            break;
        case 10:s="Invalid Interrupt Call";
            break;
        case 11:s="Invalid Port Access";
            break;
        case 12:s="Com Queue empty";
            break;
        case 13:s="No mine-layer, silly.";
            break;
        case 14:s="No mines left";
            break;
        case 15:s="No shield installed - Arm the photon torpedoes instead. :-)";
            break;
        case 16:s="Invalid Microcode in instruction.";
            break;
        default: s="Unkown error";
            break;
    }

    cout<<robot[n].errorlog<<"<"<<i<<"> "<<s<<" (Line #"<<robot[n].ip<<") [Cycle: "<<game_cycle<<", Match: "<<played<<"/"<<matches<<"]"<<endl;
    cout<<robot[n].errorlog<<" "<<mnemonic(robot[n].code[robot[n].ip].op[0],robot[n].code[robot[n].ip].op[3] & 15)<<"  "<<
                      operand(robot[n].code[robot[n].ip].op[1]<<(robot[n].code[robot[n].ip].op[3]<<4) & 15)<<", "<<
                      operand(robot[n].code[robot[n].ip].op[2]<<(robot[n].code[robot[n].ip].op[3]<<8) & 15));
    if (ov!="")
    {
        cout<<robot[n].errorlog<<"    (Values: "<<ov<<")"<<endl;
    }
    else
    {
        cout<<robot[n].errorlog<<endl;
    }
    cout<<robot[n].errorlog<<" AX="<<addrear((robot[n].ram[65])+",",7);
    cout<<robot[n].errorlog<<" BX="<<addrear((robot[n].ram[66])+",",7);
    cout<<robot[n].errorlog<<" CX="<<addrear((robot[n].ram[67])+",",7);
    cout<<robot[n].errorlog<<" DX="<<addrear((robot[n].ram[68])+",",7);
    cout<<robot[n].errorlog<<" EX="<<addrear((robot[n].ram[69])+",",7);
    cout<<robot[n].errorlog<<" FX="<<addrear((robot[n].ram[70])+",",7);
    cout<<robot[n].errorlog<<" Flags="<<robot[n].ram[64]<<endl;
    cout<<robot[n].errorlog<<" AX="<<addrear(robot[n].ram[65]+",",7);
    cout<<robot[n].errorlog<<" BX="<<addrear(robot[n].ram[66]+",",7);
    cout<<robot[n].errorlog<<" CX="<<addrear(robot[n].ram[67]+",",7);
    cout<<robot[n].errorlog<<" DX="<<addrear(robot[n].ram[68]+",",7);
    cout<<robot[n].errorlog<<" EX="<<addrear(robot[n].ram[69]+",",7);
    cout<<robot[n].errorlog<<" FX="<<addrear(robot[n].ram[70]+",",7);
    cout<<robot[n].errorlog<<" Flags="<<robot[n].ram[64]<<endl;
    cout<<robot[n].errorlog<<endl;
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
        robot[n].error_count++;
    }
}

void execute_instruction(int n)
{
    int j,k;
    int time_used,loc;
    bool inc_ip;


    //with robot[n]^ do

   //{--update system variables--}
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
        robot_error(n,16,hex(robot[n].code[robot[n].ip].op[max_op]));
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
*/                 }
            }
        }
    }


    if (! (((robot[n].code[robot[n].ip].op[max_op] & 7) > 0)&& ( robot[n].code[robot[n].ip].op[max_op]<1)))
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
                loc= find_label(n,get_val(n,robot[n].ip,1),robot[n].code[robot[n].ip].op[max_op]<<(1*4));
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
                put_val(n,robot[n].ip,1,robot[n].code[robot[n].ip].op[2]);
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
                        put_val(n,robot[n].ip,1,robot[n].code[j << 2].op[j & 3]);
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
//              begin writeln(#13,zero_pad(game_cycle,5),' F ',s,': hd=',hd,'           ');
//          repeat until /*keypressed FIX WITH GUI*/false; flushkey; end;
        }
    }
}


void damage(int n,int d,bool physical)
{
    int i,k,h,dd;
    double m;
    if ((n<0) || (n>num_robots) || (robot[n].health<=0))
    {
        return;
    }

    if (robot[n].shield<3)
    {
        robot[n].shields_up =false;
    }
    //with robot[n]^ do
    h =0;
    if ((robot[n].shields_up) && (!physical))
    {
        dd =d;
        if ((old_shields) && (robot[n].shield>=3))
        {
            d =0;
            h =0;
        }
        else
        {
            switch (robot[n].shield)
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

    robot[n].health -=d;
    robot[n].heat+=h;
    robot[n].last_damage =0;
    if (robot[n].health<=0)
    {
        robot[n].health =0;
//        update_health(n);
        robot[n].heat =500;
//        update_heat(n);
        robot[n].health =0;
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
            if ((i!=n) && (robot[i].health>0))
            {
                k =round(distance(robot[n].x,robot[n].y,robot[i].x,robot[i].y));
                if (k<blast_radius)
                {
                    damage(i,round(abs(blast_radius-k)*m),false);
                }
            }
        }
    }
}


//Awaiting more members to be added to the robot class
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
    while ((robot[n].time_left>0) && (!robot[n].cooling) && (executed<20+time_slice) && (robot[n].health>0))
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
            switch (robot[n].heatsinks)////(*heat adjustments*)
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
        if ((robot[n].heat>0) && ((game_cycle & 7)==0) && (abs(robot[n].tspd)<=25))
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
    if(abs(robot[n].spd-robot[n].tspd)<=acceleration)
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
    if ((abs(robot[n].hd-robot[n].thd)<=turn_rate) || (abs(robot[n].hd-robot[n].thd)>=256-turn_rate))
    {
        robot[n].hd = robot[n].thd;
    }
    else if( robot[n].hd != robot[n].thd)
    {
        k = 0;
        if (((robot[n].thd > robot[n].hd) && (abs(robot[n].hd-robot[n].thd)<=128)) ||
                ((robot[n].thd<robot[n].hd) && (abs(robot[n].hd-robot[n].thd)>=128)))
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
        if (abs(robot[n].speed)>=max_vel/2)
        {
            damage(n,1,true);
        }
        robot[n].spd=0;
        //{ttx=x; tty=y;}
    }

    for (i =0; i < num_robots;i++)
    {
        if ((i!=n) && (robot[i].health>0) && (distance(ttx,tty,robot[i].x,robot[i].y)<crash_range))
        {
            robot[n].tspd=0;
            robot[n].spd=0;
            ttx=robot[n].x;
            tty=robot[n].y;
            robot[i].tspd=0;
            robot[i].spd=0;
            robot[n].ram[8]++;
            robot[i].ram[8]++;
            if (abs(robot[n].speed)>=max_vel/2)
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
    robot[n].meters = robot[n].meters+distance(robot[n].x,robot[n].y,ttx,tty);
    if (robot[n].meters>=maxint)
    {
        robot[n].meters = robot[n].meters-maxint;
    }

    robot[n].ram[9]=trunc(robot[n].meters);
    robot[n].x=ttx;
    robot[n].y=tty;

   //{--draw robot--}
   if (robot[n].health<0)
   {
       robot[n].health = 0;
   }

   if (robot[n].heat<0)
   {
       robot[n].heat = 0;
   }
   if (graphix)
   {
        if (robot[n].health!=robot[n].larmor)
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
   robot[n].larmor = robot[n].health;

   //{--Cycle complete, inc counter--}
   robot[n].cycles_lived++;
}


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
    for (int i = 0; i < max_missiles;i++)
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
        prog_error(5,"");
    }

    temp_mode = step_mode;  //{store initial step_mode}

    if (logging_errors)
    {
        for (int i = 0; i<num_robots; i++)     //check what this is
        {
//            robot[i].errorlog = robot[i].base_name(robot[i].fn)+".ERR";
        }
    }

    if (compile_only)
    {
//        write_compile_report();
    }
    if (num_robots<1)
    {
        prog_error(4,"");
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
        robot[i] = Robot();
    }

    srand((unsigned)time(0));
    return;
}

//have fun testing!!!
int main()
{
    init();
    fOpen f1;

    cout<<f1.getInPath()<<endl;

    return 0;
}
