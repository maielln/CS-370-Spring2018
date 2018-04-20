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

//FULLY PORTED ATR2.PAS
#include <iostream>
#include <ctime>
#include <string>
//ALL COUTS AND WRITES NEED TO BE DISCUSSED AND REPLACED WITH GUI STUFF
using namespace std;

/* Ignoring these for now, putting in our own later
 progname       ="AT-Robots";
 version        ='2.11';
 cnotice1       ='Copyright 1997 ''99, Ed T. Toton III';
 cnotice2       ='All Rights Reserved.';
 cnotice3       ='Copyright 2014, William "Amos" Confer';
 main_filename  ='ATR2';
 robot_ext      ='.AT2';
 locked_ext     ='.ATL';
 config_ext     ='.ATS';
 compile_ext    ='.CMP';
 report_ext     ='.REP';
 */

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
// Gray50 : FillPatternType = ($AA, $55, $AA, $55,
//                             $AA, $55, $AA, $55); GUI can deal with this, I can't

struct op_rec
{
        int op[max_op];
};

op_rec prog_type[max_code];

struct config_rec //CONFIG FOR EACH BOT
{
        int scanner,weapon,armor,engine,heatsinks,shield,mines;
};

struct mine_rec //MINES
{
        float x,y;
        int detect,yield;
        bool detonate;
};

struct robot_rec //ROBOTS
{
        bool is_locked;
        int mem_watch;
        float x,y,lx,ly,xv,yv,speed,shotstrength,damageadj,speedadj,meter;
        int hd,thd,lhd,spd,tspd,armor,larmor,heat,lheat,ip,plen,scanarc,accuracy,shift,err,delay_left,robot_time_limit, max_time,time_left,lshift,arc_count,sonar_count,scanrange,last_damage,last_hit,transponder,shutdown,channel,lendarc,endarc,lstartarc,startarc,mines;
        int tx,ltx,ty,lty;
        long wins,trials,kills,deaths,startkills,shots_fired,match_shots,hits,damage_total,cycles_lived,error_count;
        config_rec config;
        string name;
        string fn[255];
        bool shields_up,lshields,overburn,keepshift,cooling,won;
        prog_type code;
        int ram[max_ram];
        mine_rec mine[max_mines];
        string errorlog;
};

string parsetype[max_op];

int* robot_ptr = &robot_rec; //robot_ptr = *robot_rec, robot_ptr is a pointer to the address of robot_rec

struct missile_rec
{
        float x,y,lx,ly,mult,mspd;
        int source,a,hd,rad,lrad,max_rad;
};

 //ROBOT VARIABLES
 int num_robots;
 robot_ptr robots[max_robots + 2];
 missile_rec missiles[max_missiles];

 //COMPILER VARIABLES
 string f; //I believe this is the contents of the file
 int numvars,numlabels,maxcode,lock_pos,lock_dat;
 string varname[max_vars];
 varloc:array[1..max_vars] of integer;
 labelname:array[1..max_vars] of string[max_var_len];
 labelnum:array[1..max_labels] of integer;
 show_source,compile_only:boolean;
 lock_code:string;

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


//may not be needed depending on how we approach
string operand(int n, int m) //function that takes in two integers (n & m) returns string
{
   string s = cstr(n);
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
      s = cstr(n);
      break;
   }
   if ((m & 8)>0)
    s ="["+s+"]";
   return s; //returns what s is, not sure we will need this function
};

string mnemonic(int n, int m)
{
  string s = cstr(n); //another cstr call
   if (m == 0){
    switch (n) { //this is going to be important when it comes to reading what the robots are coded to do
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

void log_error(int n, int i, string ov) //this can be done with throwing exceptions and errors
{
 int j, k;
 string s;
 if(!logging_errors)
 {
 //need robot[n]^ for this
   switch (i) {
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
   writeln(errorlog,'<',i,'> ',s,' (Line #',ip,') [Cycle: ',game_cycle,', Match: ',played,'/',matches,']');
   write(errorlog,' ',mnemonic(code[ip].op[0],code[ip].op[3] and 15),'  ',
                      operand(code[ip].op[1],(code[ip].op[3] shr 4) and 15),', ',
                      operand(code[ip].op[2],(code[ip].op[3] shr 8) and 15));
      if (ov!="")
      {
          cout<<robot[n].errorlog<<"    (Values: "<<ov<<")"<<endl;
      }
      else
      {
          cout<<robot[n].errorlog<<endl;
      }
      cout<<robot[n].errorlog<<" AX="<<addrear((string)(robot[n].ram[65])+",",7)<<endl;
      cout<<robot[n].errorlog<<" BX="<<addrear((string)(robot[n].ram[66])+",",7)<<endl;
      cout<<robot[n].errorlog<<" CX="<<addrear((string)(robot[n].ram[67])+",",7)<<endl;
      cout<<robot[n].errorlog<<" DX="<<addrear((string)(robot[n].ram[68])+",",7)<<endl;
      cout<<robot[n].errorlog<<" EX="<<addrear((string)(robot[n].ram[69])+",",7)<<endl;
      cout<<robot[n].errorlog<<" FX="<<addrear((string)(robot[n].ram[70])+",",7)<<endl;
      cout<<robot[n].errorlog<<" Flags="<<robot[n].ram[64]<<endl;
      cout<<robot[n].errorlog<<" AX="<<addrear(robot[n].ram[65]+",",7)<<endl;
      cout<<robot[n].errorlog<<" BX="<<addrear(robot[n].ram[66]+",",7)<<endl;
      cout<<robot[n].errorlog<<" CX="<<addrear(robot[n].ram[67]+",",7)<<endl;
      cout<<robot[n].errorlog<<" DX="<<addrear(robot[n].ram[68]+",",7)<<endl;
      cout<<robot[n].errorlog<<" EX="<<addrear(robot[n].ram[69]+",",7)<<endl;
      cout<<robot[n].errorlog<<" FX="<<addrear(robot[n].ram[70]+",",7)<<endl;
      cout<<robot[n].errorlog<<" Flags="<<robot[n].ram[64]<<endl;
      cout<<robot[n].errorlog<<endl;
 }
};

int max_shown()
{
 switch(stats_mode){
  case 1:
    max_shown=12;
    break;
  case 2:
    max_shown=32;
    break;
  default:
    max_shown=6;
    break;
 }
};

bool graph_check(int n) //GUI
{
 bool ok = true;
 if ((!graphix) || (n<0) || (n>num_robots) || (n>=max_shown))
    ok=false;
 return ok;
};

void robot_graph(int n); //GUI
{
 switch (stats_mode)
 {
  case 1:
    begin viewport(480,4+n*35,635,37+n*35); max_gx:=155; max_gy:=33; end; //all of the stats_mode below is graphics of visual stats
    break;
  case 2:
    begin viewport(480,4+n*13,635,15+n*13); max_gx:=155; max_gy:=11; end;
    break;
  default:
    begin viewport(480,4+n*70,635,70+n*70); max_gx:=155; max_gy:=66; end;
    break;
  }
 setfillstyle(1,robot_color(n));
 setcolor(robot_color(n));
};

void update_armor(int n); //takes in variables to change the armor based on what is happening to robot
begin
 if graph_check(n) {FIFI} and (step_mode<=0) {/FIFI} then
 with robot[n]^ do
  begin
   robot_graph(n);
   if armor>0 then
    case stats_mode of
     1:bar(30,13,29+armor,18);
     2:bar(88,03,87+(armor shr 2),08);
     else bar(30,25,29+armor,30);
    end;
   setfillstyle(1,8);
   if armor<100 then
    case stats_mode of
     1:bar(30+armor,13,129,18);
     2:bar(88+(armor shr 2),03,111,08);
     else bar(30+armor,25,129,30);
    end;
  end;
end;
{
  procedure update_heat(n:integer); //takes in variables to change the heat based on the action the robot is performing
  begin
   if graph_check(n) {FIFI} and (step_mode<=0) {/FIFI} then
   with robot[n]^ do
    begin
     robot_graph(n);
     if heat>5 then
      case stats_mode of
       1:bar(030,23,029+(heat div 5),28);
       2:bar(127,03,126+(heat div 20),08);
       else bar(30,35,29+(heat div 5),40);
      end;
     setfillstyle(1,8);
     if heat<500 then
      case stats_mode of
       1:bar(030+(heat div 5),23,129,28);
       2:bar(127+(heat div 20),03,151,08);
       else bar(30+(heat div 5),35,129,40);
      end;
    end;
  end;

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

  procedure update_lives(n:integer); //lives go down
  begin
   if graph_check(n) and (stats_mode=0) {FIFI} and (step_mode<=0) {/FIFI} then
   with robot[n]^ do
    begin
     robot_graph(n);
     setcolor(robot_color(n)-8);
     setfillstyle(1,0);
     bar(011,46,130,53);
     outtextxy(011,46,'K:');
     outtextxy(029,46,zero_pad(kills,4));
     outtextxy(080,46,'D:');
     outtextxy(098,46,zero_pad(deaths,4));
    end;
  end;

  procedure update_cycle_window; //changes the info of the current match on screen
  begin
   if not graphix then
    begin
     write(#13+'Match ',played,'/',matches,', Cycle: ',zero_pad(game_cycle,9));
    end
   else
    begin
     viewport(480,440,635,475);
     setfillstyle(1,0);
     bar(59,2,154,10);
     setcolor(7);
     outtextxy(75,03,zero_pad(game_cycle,9));
    end;
  end;

  procedure setscreen; //draws the screeen and everything in it
  var
   i:integer;
  begin
   if not graphix then exit;
   viewport(0,0,639,479);
   box(0,0,639,479);

   stats_mode:=0;
   case num_robots of
    0..5:stats_mode:=0;
    6..11:stats_mode:=1;
    12..max_robots:stats_mode:=2;
    else stats_mode:=0;
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
   for i:=0 to max_robots do
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
        lx:=1000-x; ly:=1000-y;
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
  end;

  procedure graph_mode(on:boolean); //graphics again, used inbetween matches to clear the graph
  begin
   if on and (not graphix) then
    begin
     Graph_VGA;
     cleardevice;
     setscreen;
     graphix:=true;
    end
   else
    if (not on) and graphix then
     begin
      closegraph;
      graphix:=false;
     end;
  end;

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

  procedure print_code(n,p:integer); //this function is putting code into hexadecimal then printing it out, not sure why we would need it
  //can forget it for now, but if it comes up later that we need it we can utilize it
  var
   i:integer;
  begin
    begin
     write(hex(p)+': ');
     for i:=0 to max_op do
      write(zero_pad(robot[n]^.code[p].op[i],5),' ');
     write(' =  ');
     for i:=0 to max_op do
      write(hex(robot[n]^.code[p].op[i]),'h ');
     writeln; writeln;
    end;
  end;

void parse1(int n, int p, parsetype s); //it seems that this procedure will be very important to look at, as it
  //is dealing with the robot's code, we may be able to simplify it
  int i,j,k,opcode,microcode;
  bool found, indirect;
  string ss;
     for(i = 0; i < (max_op - 1); i++)
      {
       k = 0; 
       found = false;
       opcode = 0;   
       microcode = 0; 
       s[i] = btrim(ucase(s[i]));
       indirect = false;
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
  */
       if (s[i]=='') 
       {
         opcode = 0;
         microcode = 0;
         found = true;
       }
       
       if (lstr(s[i],1)='[') && (rstr(s[i],1)=']')
       {
          s[i] = s[i].substr(2,(s[i].length() - 2));
          indirect = true;
       }

       if ((!found) && (s[i][1]='!'))
       {
         ss = s[i];
         ss = btrim(rstr(ss,ss.length()-1));
         if (numlabels>0)
         {
          for (j = 1; j < numlabels; j++){
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
              prog_error(15,'');
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
else if (s[i] != '')  
  prog_error(2,s[i]);
if (show_code)  
  print_code(n,p);
if (compile_by_line) 
  readkey();
};

void check_plen(int plen); //will need this if we have a max amount of code
{
if (plen > maxcode)
prog_error(16,#13#10+'Maximum program length exceeded, (Limit: '+
                      cstr(maxcode+1)+' compiled lines)');
}

procedure compile(n:integer;filename:string); //may need this to compile the robots, but we may not have to if we make them their own classes
var
pp:parsetype;
s,s1,s2,s3,orig_s,msg:string;
i,j,k,l,linecount,mask,locktype:integer;
ss:array[0..max_op] of string[16];
c,lc:char;
begin
lock_code:='';
lock_pos:=0;
locktype:=0;
lock_dat:=0;
if not exist(filename) then prog_error(8,filename);
textcolor(robot_color(n));
writeln('Compiling robot #',n+1,': ',filename);
with robot[n]^ do
begin
{FIFI}
is_locked:=false; {assume unlocked robot}
{/FIFI}
textcolor(robot_color(n));
numvars:=0;
numlabels:=0;
for k:=0 to max_code do
for i:=0 to max_op do
code[k].op[i]:=0;
plen:=0;
assign(f,filename);
reset(f);
s:='';
linecount:=0;

{--first pass, compile--}
while (not eof(f)) and (s<>'#END') {and (plen<=maxcode)} do
begin
readln(f,s);
inc(linecount);
if locktype<3 then lock_pos:=0;
if lock_code<>'' then
 for i:=1 to length(s) do
  begin
   inc(lock_pos); if lock_pos>length (lock_code) then lock_pos:=1;
   case locktype of
    3:s[i]:=char((ord(s[i])-1) xor (ord(lock_code[lock_pos]) xor lock_dat));
    2:s[i]:=char(ord(s[i]) xor (ord(lock_code[lock_pos]) xor 1));
    else s[i]:=char(ord(s[i]) xor ord(lock_code[lock_pos]));
   end;
   lock_dat:=ord(s[i]) and 15;
  end;
s:=btrim(s);
orig_s:=s;
for i:=1 to length(s) do
 if s[i] in [#0..#32,',',#128..#255] then s[i]:=' ';
if show_source and ((lock_code='') or debugging_compiler) then
   writeln(zero_pad(linecount,3)+':'+zero_pad(plen,3)+' ',s);
if debugging_compiler then
   begin if readkey=#27 then halt; end;
{-remove comments-}
k:=0;
for i:=length(s) downto 1 do
    if s[i]=';' then k:=i;
if k>0 then s:=lstr(s,k-1);
s:=btrim(ucase(s));
for i:=0 to max_op do pp[i]:='';
if (length(s)>0) and (s[1]<>';') then
 begin
  case s[1] of
   '#':begin (*  Compiler Directives  *)
        s1:=ucase(btrim(rstr(s,length(s)-1)));
        msg:=btrim(rstr(orig_s,length(orig_s)-5));
        k:=0;
        for i:=1 to length(s1) do
         if (k=0) and (s1[i]=' ') then k:=i;
        dec(k);
        if k>1 then
         begin
          s2:=lstr(s1,k);
          s3:=ucase(btrim(rstr(s1,length(s1)-k)));
          k:=0;
          if numvars>0 then
           for i:=1 to numvars do
            if s3=varname[i] then k:=i;
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
                      varname[numvars]:=s3;
                      varloc [numvars]:=127+numvars;
                     end;
              end;
           end
          else if (lstr(s2,4)='LOCK') then
           begin
            {FIFI}
            is_locked:=true; {this robot is locked}
            {/FIFI}
            if length(s2)>4 then locktype:=value(rstr(s2,length(s2)-4));
            lock_code:=btrim(ucase(s3));
            writeln('Robot is of LOCKed format from this point forward. [',locktype,']');
            {writeln('Using key: "',lock_code,'"');}
            for i:=1 to length(lock_code) do
             lock_code[i]:=char(ord(lock_code[i])-65);
           end
          else if (s2='MSG') then name:=msg
          else if (s2='TIME') then
           begin
            robot_time_limit:=value(s3);
            if robot_time_limit<0 then robot_time_limit:=0;
           end
          else if (s2='CONFIG') then
           begin
            if (lstr(s3,8)='SCANNER=') then config.scanner:=
                value(rstr(s3,length(s3)-8))
            else if (lstr(s3,7)='SHIELD=') then config.shield:=
                    value(rstr(s3,length(s3)-7))
            else if (lstr(s3,7)='WEAPON=') then config.weapon:=
                    value(rstr(s3,length(s3)-7))
            else if (lstr(s3,6)='ARMOR=') then config.armor:=
                    value(rstr(s3,length(s3)-6))
            else if (lstr(s3,7)='ENGINE=') then config.engine:=
                    value(rstr(s3,length(s3)-7))
            else if (lstr(s3,10)='HEATSINKS=') then config.heatsinks:=
                    value(rstr(s3,length(s3)-10))
            else if (lstr(s3,6)='MINES=') then config.mines:=
                    value(rstr(s3,length(s3)-6))
            else prog_error(20,s3);
            with config do
              begin
                if scanner<0 then scanner:=0; if scanner>5 then scanner:=5;
                if shield<0 then shield:=0;   if shield>5 then shield:=5;
                if weapon<0 then weapon:=0;   if weapon>5 then weapon:=5;
                if armor<0 then armor:=0;     if armor>5 then armor:=5;
                if engine<0 then engine:=0;   if engine>5 then engine:=5;
                if heatsinks<0 then heatsinks:=0; if heatsinks>5 then heatsinks:=5;
                if mines<0 then mines:=0;     if mines>5 then mines:=5;
              end;
           end
          else writeln('Warning: unknown directive "'+s2+'"');
         end;
       end;
   '*':begin  (*  Inline Pre-Compiled Machine Code  *)
        check_plen(plen);
        for i:=0 to max_op do pp[i]:='';
        for i:=2 to length(s) do
         if s[i]='*' then prog_error(23,s);
        k:=0; i:=1; s1:='';
        if length(s)<=2 then prog_error(22,s);
        while (i<length(s)) and (k<=max_op) do
         begin
          inc(i);
          if ord(s[i]) in [33..41,43..127] then pp[k]:=pp[k]+s[i]
            else if (ord(s[i]) in [0..32,128..255]) and
                    (ord(s[i-1]) in [33..41,43..127]) then inc(k);
         end;
        for i:=0 to max_op do
         code[plen].op[i]:=str2int(pp[i]);
        inc(plen);
       end;
   ':':begin  (*  :labels  *)
        check_plen(plen);
        s1:=rstr(s,length(s)-1);
        for i:=1 to length(s1) do
         if not (s1[i] in ['0'..'9']) then
            prog_error(1,s);
        code[plen].op[0]:=str2int(s1);
        code[plen].op[max_op]:=2;
        if show_code then print_code(n,plen);
        inc(plen);
       end;
   '!':begin (*  !labels  *)
        check_plen(plen);
        s1:=btrim(rstr(s,length(s)-1));
        k:=0;
        for i:=length(s1) downto 1 do
            if s1[i] in [';',#8,#9,#10,' ',','] then k:=i;
        if k>0 then s1:=lstr(s1,k-1);
        k:=0;
        for i:=1 to numlabels do
         if (labelname[i]=s1) then
          begin
           if (labelnum[i]>=0) then prog_error(13,'"!'+s1+'" ('+cstr(labelnum[i])+')');
           k:=i;
          end;
        if (k=0) then
         begin
          inc(numlabels);
          if numlabels>max_labels then prog_error(15,'');
          k:=numlabels;
         end;
        labelname[k]:=s1;
        labelnum [k]:=plen;
       end;
   else begin  (*  Instructions/Numbers  *)
         check_plen(plen);
         {-parse instruction-}
         {-remove comments-}
         k:=0;
         for i:=length(s) downto 1 do
             if s[i]=';' then k:=i;
         if k>0 then s:=lstr(s,k-1);
         {-setup variables for parsing-}
         k:=0; for j:=0 to max_op do pp[j]:='';
         for j:=1 to length(s) do
          begin
           c:=s[j];
           if (not (c in [' ',#8,#9,#10,','])) and (k<=max_op) then pp[k]:=pp[k]+c
              else if not (lc in [' ',#8,#9,#10,',']) then k:=k+1;
           lc:=c;
          end;
         parse1(n,plen,pp);
         inc(plen);
        end;
  end;
 end;
end;
close(f);
/*{-Add our implied NOP if there's room. This was originally to make sure
no one tries using an empty robot program, kinda pointless otherwise-}*/
if plen<=maxcode then
begin
for i:=0 to max_op do pp[i]:='';
pp[0]:='NOP';
parse1(n,plen,pp);
end
else
dec(plen);


{--second pass, resolving !labels--}
if numlabels>0 then
for i:=0 to plen do
for j:=0 to max_op-1 do
 if code[i].op[max_op] shr (j*4)=3 {unresolved !label} then
  begin
   k:=code[i].op[j];
   if (k>0) and (k<=numlabels) then
    begin
     l:=labelnum[k];
     if (l<0) then prog_error(19,'"!'+labelname[k]+'" ('+cstr(l)+')');
     if (l<0) or (l>maxcode) then prog_error(18,'"!'+labelname[k]+'" ('+cstr(l)+')')
      else
       begin
        code[i].op[j]:=l;
        mask:=not($F shl (j*4));
        code[i].op[max_op]:=(code[i].op[max_op] and mask) or (4 shl (j*4));
                             {resolved !label}
       end;
    end
   else prog_error(17,cstr(k));
  end;
end;
textcolor(7);
end;

procedure robot_config(n:integer);
var
i,j,k:integer;
begin
with robot[n]^ do
begin
case config.scanner of
5:scanrange:=1500;
4:scanrange:=1000;
3:scanrange:=700;
2:scanrange:=500;
1:scanrange:=350;
else scanrange:=250;
end;
case config.weapon of
5:shotstrength:=1.5;
4:shotstrength:=1.35;
3:shotstrength:=1.2;
2:shotstrength:=1;
1:shotstrength:=0.8;
else shotstrength:=0.5;
end;
case config.armor of
5:begin damageadj:=0.66; speedadj:=0.66; end;
4:begin damageadj:=0.77; speedadj:=0.75; end;
3:begin damageadj:=0.83; speedadj:=0.85; end;
2:begin damageadj:=1; speedadj:=1; end;
1:begin damageadj:=1.5; speedadj:=1.2; end;
else begin damageadj:=2; speedadj:=1.33; end;
end;
case config.engine of
5:speedadj:=speedadj*1.5;
4:speedadj:=speedadj*1.35;
3:speedadj:=speedadj*1.2;
2:speedadj:=speedadj*1;
1:speedadj:=speedadj*0.8;
else speedadj:=speedadj*0.5;
end;
{heatsinks are handled seperately}
case config.mines of
5:mines:=24;
4:mines:=16;
3:mines:=10;
2:mines:=6;
1:mines:=4;
else begin mines:=2; config.mines:=0; end;
end;
shields_up:=false;
if (config.shield<3) or (config.shield>5) then config.shield:=0;
if (config.heatsinks<0) or (config.heatsinks>5) then config.heatsinks:=0;
end;
end;

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

procedure reset_hardware(n:integer);
var
i:integer;
d,dd:real;
begin
with robot[n]^ do
begin
for i:=1 to max_robot_lines do
begin ltx[i]:=0; tx[i]:=0; lty[i]:=0; ty[i]:=0; end;
repeat
x:=random(1000); y:=random(1000);
dd:=1000;
for i:=0 to num_robots do
begin
 if robot[i]^.x<0 then robot[i]^.x:=0;
 if robot[i]^.x>1000 then robot[i]^.x:=1000;
 if robot[i]^.y<0 then robot[i]^.y:=0;
 if robot[i]^.y>1000 then robot[i]^.y:=1000;
 d:=distance(x,y,robot[i]^.x,robot[i]^.y);
 if (robot[i]^.armor>0) and (i<>n) and (d<dd) then dd:=d;
end;
until dd>32;
for i:=0 to max_mines do
with mine[i] do
begin x:=-1; y:=-1; yield:=0; detonate:=false; detect:=0; end;
lx:=-1; ly:=-1;
hd:=random(256); shift:=0;
lhd:=hd+1; lshift:=shift+1;
spd:=0; speed:=0;
cooling:=false;
armor:=100; larmor:=0;
heat:=0; lheat:=1;
match_shots:=0;
won:=false;
last_damage:=0;
last_hit:=0;
transponder:=n+1;
meters:=0;
shutdown:=400;
shields_up:=false;
channel:=transponder;
startkills:=kills;
robot_config(n);
end;
end;

procedure init_robot(n:integer);
var
i,j,k,l:integer;
begin
with robot[n]^ do
begin
wins:=0; trials:=0; kills:=0; deaths:=0; shots_fired:=0; match_shots:=0;
hits:=0; damage_total:=0; cycles_lived:=0; error_count:=0;
plen:=0; max_time:=0;
name:=''; fn:='';
speed:=0;
arc_count:=0;
sonar_count:=0;
robot_time_limit:=0;
scanrange:=1500;
shotstrength:=1;
damageadj:=1;
speedadj:=1;
mines:=0;
with config do
begin
scanner:=5;
weapon:=2;
armor:=2;
engine:=2;
heatsinks:=1;
shield:=0;
mines:=0;
end;
for i:=0 to max_ram do ram[i]:=0;
ram[71] := 768;
for i:=0 to max_code do
for k:=0 to max_op do
code[i].op[k]:=0;
reset_hardware(n);
reset_software(n);
end;
end;

procedure create_robot(n:integer; filename:string);
var
i,j,k:integer;
begin
if maxavail<sizeof(robot_rec) then prog_error(9,base_name(no_path(filename)));
new(robot[n]);
with robot[n]^ do
begin
init_robot(n);
filename:=ucase(btrim(filename));
if filename=base_name(filename) then
begin
if filename[1]='?' then filename:=filename+locked_ext
                   else filename:=filename+robot_ext;
end;
if filename[1]='?' then filename:=rstr(filename,length(filename)-1);
fn:=base_name(no_path(filename));
compile(n,filename);
robot_config(n);
with config do
begin
k:=scanner+armor+weapon+engine+heatsinks+shield+mines;
if (k)>max_config_points then
   prog_error(21,cstr(k)+'/'+cstr(max_config_points));
end;
end;
end;

procedure shutdown;
var
i,j,k:integer;
begin
graph_mode(false);
if show_cnotice then
begin
textcolor(3);
write  (progname,' ',version,' ');
writeln(cnotice1);
writeln(cnotice2);
writeln(cnotice3);
end;
textcolor(7);
if not registered then begin textcolor(4); writeln('Unregistered version'); end
              else writeln('Registered to: ',reg_name);
textcolor(7);
writeln;
if logging_errors then
for i:=0 to num_robots do
with robot[i]^ do
begin
writeln('Robot error-log created: ',base_name(fn)+'.ERR');
close(errorlog);
end;
halt;
end;

procedure delete_compile_report;
begin
if exist(main_filename+compile_ext) then
delete_file(main_filename+compile_ext);
end;

procedure write_compile_report;
var
f:text;
i,j,k:integer;
begin
assign(f,main_filename+compile_ext);
rewrite(f);
writeln(f,num_robots+1);
for i:=0 to num_robots do
with robot[i]^ do
writeln(f,fn);
close(f);
textcolor(15);
writeln;
writeln('All compiles successful!');
writeln;
shutdown;
end;


procedure parse_param(s:String);
var
f:text;
fn,s1:string;
found:boolean;
begin
found:=false;
s:=btrim(ucase(s));
if s='' then exit;
if s[1]='#' then
begin
fn:=rstr(s,length(s)-1);
if fn=base_name(fn) then fn:=fn+config_ext;
if not exist(fn) then prog_error(6,fn);
assign(f,fn); reset(f);
while not eof(f) do
begin
readln(f,s1);
s1:=ucase(btrim(s1));
if s1[1]='#' then prog_error(7,s1)
   else parse_param(s1);
end;
close(f);
found:=true;
end
else if s[1] in ['/','-','='] then
begin
s1:=rstr(s,length(s)-1);
{FIFI}
if s1[1]='X' then
begin
step_mode:=value(rstr(s1,length(s1)-1));
found:=true;
if step_mode=0 then step_mode:=1;
if (step_mode<1) or (step_mode>9) then prog_error(24,rstr(s1,length(s1)-1));
end;
{/FIFI}
if s1[1]='D' then begin game_delay:=value(rstr(s1,length(s1)-1)); found:=true; end;
if s1[1]='T' then begin time_slice:=value(rstr(s1,length(s1)-1)); found:=true; end;
if s1[1]='L' then begin game_limit:=value(rstr(s1,length(s1)-1))*1000; found:=true; end;
if s1[1]='Q' then begin sound_on:=false; found:=true; end;
if s1[1]='M' then begin matches:=value(rstr(s1,length(s1)-1)); found:=true; end;
if s1[1]='S' then begin show_source:=false; found:=true; end;
if s1[1]='G' then begin no_gfx:=true; found:=true; end;
if s1[1]='R' then begin report:=true; found:=true;
                if (length(s1)>1) then
                   report_type:=value(rstr(s1,length(s1)-1)); end;
if s1[1]='C' then begin compile_only:=true; found:=true; end;
if s1[1]='^' then begin show_cnotice:=false; found:=true; end;
if s1[1]='A' then begin show_arcs:=true; found:=true; end;
if s1[1]='W' then begin windoze:=false; found:=true; end;
if s1[1]='$' then begin debug_info:=true; found:=true; end;
if s1[1]='#' then begin maxcode:=value(rstr(s1,length(s1)-1))-1; found:=true; end;
if s1[1]='!' then begin insane_missiles:=true; if (length(s1)>1) then
                insanity:=value(rstr(s1,length(s1)-1)); found:=true; end;
if s1[1]='@' then begin old_shields:=true; found:=true; end;
if s1[1]='E' then begin logging_errors:=true; found:=true; end;
if insanity<0 then insanity:=0;
if insanity>15 then insanity:=15;
end
else if s[1]=';' then found:=true
else if (num_robots<max_robots) and (s<>'') then
begin
inc(num_robots);
create_robot(num_robots,s);
found:=true;
if num_robots=max_robots then writeln('Maximum number of robots reached.');
end
else prog_error(10,'');
if not found then prog_error(8,s);
end;


procedure init;
var
i:integer;
begin
if debugging_compiler or compile_by_line or show_code then
begin write('!!! Warning !!! Compiler Debugging enabled !!!'); flushkey; readkey; writeln; end;
{FIFI}
step_mode:=0; {stepping disabled}
{/FIFI}
logging_errors:=false; stats_mode:=0;
insane_missiles:=false; insanity:=0;
delay_per_sec:=0; windoze:=true;
graphix:=false; no_gfx:=false;
sound_on:=true; timing:=true;
matches:=1; played:=0; old_shields:=false;
quit:=false; compile_only:=false;
show_arcs:=false; debug_info:=false;
show_cnotice:=true;
show_source:=true;
report:=false;
kill_count:=0;
maxcode:=max_code;
make_tables;
randomize;
num_robots:=-1;
game_limit:=100000;
game_cycle:=0;
game_delay:=default_delay;
time_slice:=default_slice;
for i:=0 to max_missiles do
with missile[i] do
begin a:=0; source:=-1; x:=0; y:=0; lx:=0; ly:=0; mult:=1; end;

registered:=false;
reg_name:='Unregistered';
reg_num:=$FFFF;
check_registration;

writeln; textcolor(3);
write  (progname,' ',version,' ');
writeln(cnotice1);
writeln(cnotice2);
textcolor(7);
if not registered then begin textcolor(4); writeln('Unregistered version'); end
              else writeln('Registered to: ',reg_name);
textcolor(7);
writeln;
{create_robot(0,'SDUCK');}

delete_compile_report;
if paramcount>0 then
for i:=1 to paramcount do
parse_param(btrim(ucase(paramstr(i))))
else prog_error(5,'');
{FIFI}
temp_mode:=step_mode;  {store initial step_mode}
{/FIFI}
if logging_errors then
for i:=0 to num_robots do
with robot[i]^ do
begin
assign(errorlog,base_name(fn)+'.ERR');
rewrite(errorlog);
end;
if compile_only then write_compile_report;
if num_robots<1 then prog_error(4,'');

if not no_gfx then graph_mode(true);

{--fix ups--}
if matches>100000 then matches:=100000;
if matches<1 then matches:=1;
if game_delay>1000 then game_delay:=1000;
if game_delay<0 then game_delay:=0;
if time_slice>100 then time_slice:=100;
if time_slice<1 then time_slice:=1;
if game_limit<0 then game_limit:=0;
if game_limit>100000 then game_limit:=100000;
if maxcode<1 then maxcode:=1; {0 based, so actually 2 lines}
if maxcode>max_code then maxcode:=max_code;

{--Just to avoid floating pointers--}
for i:=num_robots+1 to max_robots+2 do
robot[i]:=robot[0];
robot[-1]:=robot[0];
robot[-2]:=robot[0];

if not graphix then
begin
writeln('Freemem: ',memavail);
writeln;
end;
end;

procedure draw_robot(n:integer);
var
i,t:integer;
xx,yy:real;
begin
if not (n in [0..num_robots]) then exit;
with robot[n]^ do
begin
if x>1000 then x:=1000;
if y>1000 then y:=1000;
if x<0 then x:=0;
if y<0 then y:=0;
{if (lhd=hd) and (lx=x) and (ly=y) then exit;}

{--set up for erase--}
{xx:=lx*screen_scale; yy:=ly*screen_scale;
lhd:=(lhd+1024) and 255;
ltx[1]:=round(xx+sint[lhd]*robot_scale);
lty[1]:=round(yy-cost[lhd]*robot_scale);
ltx[2]:=round(xx+sint[(lhd+$68) and 255]*robot_scale);
lty[2]:=round(yy-cost[(lhd+$68) and 255]*robot_scale);
ltx[3]:=round(xx+sint[(lhd+$98) and 255]*robot_scale);
lty[3]:=round(yy-cost[(lhd+$98) and 255]*robot_scale);
t:=(lhd+(lshift and 255)+1024) and 255;
ltx[4]:=round(xx+sint[t and 255]*robot_scale);
lty[4]:=round(yy-cost[t and 255]*robot_scale);}

{--set up for draw--}
xx:=x*screen_scale+screen_x; yy:=y*screen_scale+screen_y;
hd:=(hd+1024) and 255;
tx[1]:=round(xx+sint[hd]*5);
ty[1]:=round(yy-cost[hd]*5);
tx[2]:=round(xx+sint[(hd+$68) and 255]*robot_scale);
ty[2]:=round(yy-cost[(hd+$68) and 255]*robot_scale);
tx[3]:=round(xx+sint[(hd+$98) and 255]*robot_scale);
ty[3]:=round(yy-cost[(hd+$98) and 255]*robot_scale);
t:=(hd+(shift and 255)+1024) and 255;
tx[4]:=round(xx); ty[4]:=round(yy);
tx[5]:=round(xx+sint[t]*robot_scale*0.8);
ty[5]:=round(yy-cost[t]*robot_scale*0.8);
tx[6]:=round(xx+sint[(t+scanarc+1024) and 255]*scanrange*screen_scale);
ty[6]:=round(yy-cost[(t+scanarc+1024) and 255]*scanrange*screen_scale);
tx[7]:=round(xx+sint[(t-scanarc+1024) and 255]*scanrange*screen_scale);
ty[7]:=round(yy-cost[(t-scanarc+1024) and 255]*scanrange*screen_scale);
startarc:=round(((256-((t+scanarc+1024) and 255))/256*360)+90);
endarc:=round(((256-((t-scanarc+1024) and 255))/256*360)+90);


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
if armor>0 then
 begin {only erase body if still alive, that way we leave a "corpse" when dead}
  line(ltx[1],lty[1],ltx[2],lty[2]);
  line(ltx[2],lty[2],ltx[3],lty[3]);
  line(ltx[3],lty[3],ltx[1],lty[1]);
  line(ltx[4],lty[4],ltx[5],lty[5]);
 end;
end;
if armor>0 then //{If we're still alive we need to redraw}
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
lx:=x; ly:=y; lhd:=hd; lshift:=shift; lshields:=shields_up;
for i:=1 to max_robot_lines do
 begin ltx[i]:=tx[i]; lty[i]:=ty[i]; end;
lstartarc:=startarc; lendarc:=endarc;
end;
end;
end;

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


procedure init_mine(n,detectrange,size:integer);
var
i,j,k:integer;
begin
with robot[n]^ do
begin
k:=-1;
for i:=0 to max_mines do
if ((mine[i].x<0) or (mine[i].x>1000) or (mine[i].y<0) or (mine[i].y>1000)
  or (mine[i].yield<=0)) and (k<0) then k:=i;
if k>=0 then
begin
mine[k].x:=x;
mine[k].y:=y;
mine[k].detect:=detectrange;
mine[k].yield:=size;
mine[k].detonate:=false;
click;
end;
end;
end;

function count_missiles:integer;
var
i,k:integer;
begin
k:=0;
for i:=0 to max_missiles do
if missile[i].a>0 then inc(k);
count_missiles:=k;
end;

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

{FIFI}
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
for i:=0 to 9 do
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

stats_mode:=0;
case num_robots of
0..5:stats_mode:=0;
6..11:stats_mode:=1;
12..max_robots:stats_mode:=2;
else stats_mode:=0;
end;

viewport(480,4,635,430);
setfillstyle(1,7);
bar(0,0,155,426);
{ update_cycle_window;}

{--robot windows--}
for i:=0 to max_robots do
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
 lx:=1000-x; ly:=1000-y;
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

procedure toggle_graphix;
begin
graph_mode(not graphix);
if not graphix then
begin
textcolor(7);
writeln('Match ',played,'/',matches,', Battle in progress...');
writeln;
end
else setscreen;
end;

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
procedure process_keypress(c:char);
begin
case c of
'C':calibrate_timing;
'T':timing:=not timing;
'A':show_arcs:=not show_arcs;
'S','Q':begin
       if sound_on then chirp;
       sound_on:=not sound_on;
       if sound_on then chirp;
      end;
'$':debug_info:=not debug_info;
'W':windoze:=not windoze;
#8:bout_over:=true;
#27:begin quit:=true; step_loop:=false; end;
end;
end;

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
                call_int(n,get_val(n,robot[n].ip,1),&time_used);
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

procedure do_mine(n,m:integer); //this is an indicator for when to explode and place mines, can be MUCH more efficient
var
i,j,k,l:integer;
d,r:real;
source_alive:boolean;
begin
with robot[n]^.mine[m] do
if (x>=0) and (x<=1000) and (y>=0) and (y<=1000) and (yield>0) then
begin
for i:=0 to num_robots do
if (robot[i]^.armor>0) and (i<>n) then
 begin
  d:=distance(x,y,robot[i]^.x,robot[i]^.y);
  if d<=detect then detonate:=true;
 end;
if detonate then
begin
 init_missile(x,y,0,0,0,n,mine_circle,false);
 kill_count:=0;
 if (robot[n]^.armor>0) then source_alive:=true else source_alive:=false;
 for i:=0 to num_robots do
  if {(i<>n) and} (robot[i]^.armor>0) then
   begin
    k:=round(distance(x,y,robot[i]^.x,robot[i]^.y));
    if k<yield then
     begin
      damage(i,round(abs(yield-k)),false);
      if (n in [0..num_robots]) and (i<>n) then
         inc(robot[n]^.damage_total,round(abs(yield-k)));
     end;
   end;
  if (kill_count>0) and (source_alive) and (robot[n]^.armor<=0) then dec(kill_count);
  if kill_count>0 then
   begin
    inc(robot[n]^.kills,kill_count);
    update_lives(n);
   end;
 if graphix then
  putpixel(round(x*screen_scale)+screen_x,round(y*screen_scale)+screen_y,0);
 yield:=0; x:=-1; y:=-1;
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
end;
end;
end;

procedure do_missile(n:integer); //code for missle, will need to look at but can make more efficient
var
llx,lly,r,d,dir,xv,yv:real;
i,j,k,l,xx,yy,tx,ty,dd,dam:integer;
source_alive:boolean;
begin
with missile[n] do
if a=0 then exit else
begin
if a=1 then
begin
{--Look for collision w/ wall--}
if (x<-20) or (x>1020) or (y<-20) or (y>1020) then a:=0;

{--move missile--}
llx:=lx; lly:=ly;
lx:=x; ly:=y;
if a>0 then
 begin
  hd:=(hd+256) and 255;
  xv:= sint[hd]*mspd;
  yv:=-cost[hd]*mspd;
  x:=x+xv;
  y:=y+yv;
 end;

{--look for hit on a robot--}
k:=-1; l:=maxint;
for i:=0 to num_robots do
 if (robot[i]^.armor>0) and (i<>source) then
  begin
   d:=distance(lx,ly,robot[i]^.x,robot[i]^.y);
   {dir:=find_angle(lx,ly,robot[i]^.x,robot[i]^.y);
   j:=(round(dir/pi*128)+1024) and 255;}
   hd:=hd and 255;
   xx:=round( sint[hd]*d+lx);
   yy:=round(-cost[hd]*d+ly);
   r:=distance(xx,yy,robot[i]^.x,robot[i]^.y);
   if (d<=mspd) and (r<hit_range) and (round(d)<=l) then
    begin k:=i; l:=round(d); dd:=round(r); tx:=xx; ty:=yy; end;
  end;
if k>=0 then  (*hit a robot!*)
 begin
  x:=tx; y:=ty; a:=2; rad:=0; lrad:=0;
  if source in [0..num_robots] then
   begin
    robot[source]^.last_hit:=0;
    inc(robot[source]^.hits);
   end;
  for i:=0 to num_robots do
   begin
    dd:=round(distance(x,y,robot[i]^.x,robot[i]^.y));
    if dd<=hit_range then
      begin
       dam:=round(abs(hit_range-dd)*mult);
       if dam<=0 then dam:=1;
       kill_count:=0;
       if (robot[source]^.armor>0) then source_alive:=true else source_alive:=false;
       damage(i,dam,false);
       if (source in [0..num_robots]) and (i<>source) then
         inc(robot[source]^.damage_total,dam);
       if (kill_count>0) and (source_alive) and (robot[source]^.armor<=0) then dec(kill_count);
       if kill_count>0 then
        begin
         inc(robot[source]^.kills,kill_count);
         update_lives(source);
        end;
      end;
   end;
 end;

{--draw missile--}
if graphix then
 begin
  main_viewport;
  setcolor(0);
  line(round(llx*screen_scale)+screen_x,round(lly*screen_scale)+screen_y,
       round(lx*screen_scale)+screen_x,round(ly*screen_scale)+screen_y);
  if a=1 then
   begin
    if mult>robot[source]^.shotstrength then setcolor(14+(game_cycle and 1)) else setcolor(15);
    line(round(x*screen_scale)+screen_x,round(y*screen_scale)+screen_y,
         round(lx*screen_scale)+screen_x,round(ly*screen_scale)+screen_y);
   end;
 end;
end;
if (a=2) then
begin
lrad:=rad;
inc(rad);
if rad>max_rad then a:=0;
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
end;
end;

function victor_string(k,n:integer):string; //can make a simple function for doing this
var
s:string;
begin
s:='';
if k=1 then s:='Robot #'+cstr(n+1)+' ('+robot[n]^.fn+') wins!';
if k=0 then s:='Simultaneous destruction, match is a tie.';
if k>1 then s:='No clear victor, match is a tie.';
victor_string:=s;
end;

procedure show_statistics; //was seen in another file, can use either function
var
i,j,k,n,sx,sy:integer;
begin
if not windoze then exit;
if graphix then
begin
sx:=24; sy:=93-num_robots*3;
viewport(0,0,639,479);
box(sx+000,sy,sx+591,sy+102+num_robots*12);
hole(sx+004,sy+004,sx+587,sy+098+num_robots*12);
setfillpattern(gray50,1);
bar(sx+005,sy+005,sx+586,sy+097+num_robots*12);
setcolor(15);
outtextxy(sx+016,sy+020,'Robot            Scored   Wins  Matches  Armor  Kills  Deaths    Shots');
outtextxy(sx+016,sy+030,'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~');
n:=-1; k:=0;
for i:=0 to num_robots do
with robot[i]^ do
if (armor>0) or (won) then begin inc(k); n:=i; end;
for i:=0 to num_robots do
with robot[i]^ do
begin
 setcolor(robot_color(i));
 if (k=1) and (n=i) then j:=1 else j:=0;
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
n:=-1; k:=0;
for i:=0 to num_robots do
with robot[i]^ do
if (armor>0) or (won) then begin inc(k); n:=i; end;
for i:=0 to num_robots do
with robot[i]^ do
begin
 textcolor(robot_color(i));
 if (k=1) and (n=i) then j:=1 else j:=0;
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

procedure score_robots; //adjusts the score
var
n,i,j,k,l:integer;
begin
k:=0; n:=-1;
for i:=0 to num_robots do
begin
inc(robot[i]^.trials);
if robot[i]^.armor>0 then begin inc(k); n:=i; end;
end;
if (k=1) and (n>=0) then
with robot[n]^ do
begin inc(wins); won:=true; end;
end;

procedure init_bout; //just sets up the initial match, not necessary
var
i,j,k:integer;
begin
game_cycle:=0;
for i:=0 to max_missiles do
with missile[i] do
begin a:=0; source:=-1; x:=0; y:=0; lx:=0; ly:=0; mult:=1; end;
for i:=0 to num_robots do
with robot[i]^ do
begin
{FIFI}
mem_watch:=128;
{/FIFI}
reset_hardware(i);
reset_software(i);
end;
if graphix then setscreen;
{FIFI}
if graphix and (step_mode>0) then init_debug_window;
{/FIFI}
if not graphix then
begin
textcolor(7);
{writeln(#13+'Match ',played,'/',matches,', Battle in progress...');
writeln;}
end;
end;

{
procedure toggle_graphix;
begin
graph_mode(not graphix);
if not graphix then
begin
textcolor(7);
writeln('Match ',played,'/',matches,', Battle in progress...');
writeln;
end
else setscreen;
end;
}
procedure bout; //this is the main function, it's keeping timers and setting everything up when it needs to be set up, so probably vital but can be changed to something better
var
i,j,k:integer;
c:char;
timer:longint;
{FIFI}
n:integer; {loop count for mem_watch}
{/FIFI}
begin
if quit then exit;

inc(played);
init_bout;
bout_over:=false;

{FIFI}
if step_mode=0 then step_loop:=false else step_loop:=true; {set/clear loop flag}
step_count:=-1; {will be set to 0 at first iteration of REPEAT ;) }
if graphix and (step_mode>0) then for i:=0 to num_robots do draw_robot(i);
{/FIFI}

repeat
inc(game_cycle);
for i:=0 to num_robots do
if robot[i]^.armor>0 then do_robot(i);
for i:=0 to max_missiles do
if missile[i].a>0 then do_missile(i);
for i:=0 to num_robots do
for k:=0 to max_mines do
if robot[i]^.mine[k].yield>0 then do_mine(i,k);

if graphix and timing then time_delay(game_delay);

if keypressed then c:=upcase(readkey) else c:=#255;
case c of
{FIFI}
'X': if not robot[0]^.is_locked then
    begin
     if not graphix then toggle_graphix;
     if robot[0]^.armor>0 then
      begin
       if temp_mode>0 then step_mode:=temp_mode else step_mode:=1;
       step_count:=-1;
       init_debug_window;
      end;
    end;
{/FIFI}
'+','=':if game_delay<100 then
       case game_delay of
         000..004:game_delay:=005;
         005..009:game_delay:=010;
         010..014:game_delay:=015;
         015..019:game_delay:=020;
         020..029:game_delay:=030;
         030..039:game_delay:=040;
         040..049:game_delay:=050;
         050..059:game_delay:=060;
         060..074:game_delay:=075;
         075..100:game_delay:=100;
       end;
'-','_':if game_delay>0 then
       case game_delay of
         000..005:game_delay:=000;
         006..010:game_delay:=005;
         011..015:game_delay:=010;
         016..020:game_delay:=015;
         021..030:game_delay:=020;
         031..040:game_delay:=030;
         041..050:game_delay:=040;
         051..060:game_delay:=050;
         061..075:game_delay:=060;
         076..100:game_delay:=075;
       end;
'G':toggle_graphix;
else process_keypress(c);
end; flushkey;
if game_delay<0 then game_delay:=0;
if game_delay>100 then game_delay:=100;
case game_delay of
000..001:k:=100;
002..005:k:=50;
006..010:k:=25;
011..025:k:=20;
026..040:k:=10;
041..070:k:=5;
071..maxint:k:=1;
else k:=10;
end;
if not graphix then k:=100;
if graphix then
begin
if ((game_cycle mod k)=0) or (game_cycle=10) then update_cycle_window;
end else begin
if (update_timer<>mem[0:$46C] shr 1) then update_cycle_window;
update_timer:=mem[0:$46C] shr 1;
end;
until quit or gameover or bout_over;
update_cycle_window;
{if (not graphix) then writeln;}
score_robots;
show_statistics;
end;

procedure write_report;
var
i,j,k:integer;
f:text;
begin
assign(f,main_filename+report_ext);
rewrite(f);
writeln(f,num_robots+1);
for i:=0 to num_robots do
with robot[i]^ do
case report_type of
2:writeln(f,wins,' ',trials,' ',kills,' ',deaths,' ',fn,' ');
3:writeln(f,wins,' ',trials,' ',kills,' ',deaths,' ',armor,' ',
           heat,' ',shots_fired,' ',fn,' ');
4:writeln(f,wins,' ',trials,' ',kills,' ',deaths,' ',armor,' ',
           heat,' ',shots_fired,' ',hits,' ',damage_total,' ',
           cycles_lived,' ',error_count,' ',fn,' ');
else writeln(f,wins,' ',trials,' ',fn,' ');
end;
close(f);
end;

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
s:='Press any key to begin!';
outtextxy(320-((length(s) shl 3) shr 1),172,s);
flushkey; readkey;
setscreen;
end;

int main(void)
{
  int i, j, k, l, n, w;
  srand((unsigned) time(0)); //needed for random locations
  init();
  if (graphix) //GUI, just made it c++ code
    begin_window();
  if (matches > 0){
   for(i = 0; i < matches; i++)
    bout();
  }
  if (!graphix)
    cout<<endl; //no idea what this does
  if (quit)
    exit();
  if (matches > 1) { //Calculate overall statistics
    cout<<endl<<endl;
    graph_mode(false);
    textcolor(15); //GUI
    cout<<"Bout complete! ("<<matches<<" matches)")<<endl;
    cout<<endl;
    k = 0;
    w = 0;
    for(i = 0; i < num_robots; i++){
     //need robot[i]^
       if (packet_robot[i].wins = w)begin inc(k); end;
       if wins>w then begin k:=1; n:=i; w:=wins; end;
      end;
    cout<<"Robot           Wins  Matches  Kills  Deaths    Shots"<<endl;
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    for i:=0 to num_robots do
     with robot[i]^ do
      {
//       textcolor(robot_color(i)); //GUI
//       writeln(addfront(cstr(i+1),2)+' - '+addrear(fn,8)
//              +addfront(cstr(wins),7)+addfront(cstr(trials),8)
//              +addfront(cstr(kills),8)+addfront(cstr(deaths),8)
//              +addfront(cstr(shots_fired),9));
      }
//    textcolor(15); //GUI
    cout<<endl;
    if k=1 then writeln('Robot #',n+1,' (',robot[n]^.fn,') wins the bout! (score: ',w,'/',matches,')')
           else writeln('There is no clear victor!');
    writeln;
   end
  else if graphix then
   begin {matches=1 also}
    graph_mode(false);
    show_statistics;
   end;
  if report then write_report;
  shutdown();
}
