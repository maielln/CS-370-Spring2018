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

  procedure robot_error(n,i:integer;ov:string); //occurs with any errors
  begin
   if graph_check(n) {FIFI} and (step_mode<=0) {/FIFI} then
   with robot[n]^ do
    begin
     if (stats_mode=0) then
      begin
       robot_graph(n);
       setfillstyle(1,0);
       bar(66,56,154,64);
       setcolor(robot_color(n));
       outtextxy(66,56,addrear(cstr(i),7)+hex(i));
       chirp;
      end;
     if logging_errors then log_error(n,i,ov);
     inc(error_count);
    end;
  end;

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

  procedure prog_error(n:integer; ss:string); //error proofing, will probably need something similar with how confer is
  var //n is the int of what kin of error, ss is the string that caused the error
   s:string;
  begin
   graph_mode(false);
   textcolor(15);
   write('Error #',n,': ');
   case n of //these errors are errors from reading in the files, if the assembly code is wrong, these errors are shown respectively
    00:s:=ss;(*user error*)
    01:s:='Invalid :label - "'+ss+'", silly mortal.';
    02:s:='Undefined identifier - "'+ss+'". A typo perhaps?';
    03:s:='Memory access out of range - "'+ss+'"';
    04:s:='Not enough robots for combat. Maybe we should just drive in circles.';
    05:s:='Robot names and settings must be specified. An empty arena is no fun.';
    06:s:='Config file not found - "'+ss+'"';
    07:s:='Cannot access a config file from a config file - "'+ss+'"';
    08:s:='Robot not found "'+ss+'". Perhaps you mistyped it?';
    09:s:='Insufficient RAM to load robot: "'+ss+'"... This is not good.';
    10:s:='Too many robots! We can only handle '+cstr(max_robots+1)+'! Blah.. limits are limits.';
    11:s:='You already have a perfectly good #def for "'+ss+'", silly.';
    12:s:='Variable name too long! (Max:'+cstr(max_var_len)+') "'+ss+'"';
    13:s:='!Label already defined "'+ss+'", silly.';
    14:s:='Too many variables! (Var Limit: '+cstr(max_vars)+')';
    15:s:='Too many !labels! (!Label Limit: '+cstr(max_labels)+')';
    16:s:='Robot program too long! Boldly we simplify, simplify along...'+ss;
    17:s:='!Label missing error. !Label #'+ss+'.';
    18:s:='!Label out of range: '+ss;
    19:s:='!Label not found. '+ss;
    20:s:='Invalid config option: "'+ss+'". Inventing a new device?';
    21:s:='Robot is attempting to cheat; Too many config points ('+ss+')';
    22:s:='Insufficient data in data statement: "'+ss+'"';
    23:s:='Too many asterisks: "'+ss+'"';
    {FIFI}
    24:s:='Invalid step count: "'+ss+'". 1-9 are valid conditions.';
    {/FIFI}
    25:s:='"'+ss+'"';
    else s:=ss;
   end;
   writeln(s);
   writeln;
   halt;
  end;

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

procedure parse1(n,p:integer; s:parsetype); //it seems that this procedure will be very important to look at, as it
  //is dealing with the robot's code, we may be able to simplify it
  var
   i,j,k,opcode,microcode:integer;
   found,indirect:boolean;
   ss:string;
  begin
   with robot[n]^ do
    begin
     for i:=0 to max_op-1 do
      begin
       k:=0; found:=false;
       opcode:=0;    {nop}
       microcode:=0; {instruction/constant}
       s[i]:=btrim(ucase(s[i]));
       indirect:=false;
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
       if s[i]='' then begin opcode:=0; microcode:=0; found:=true; end;

       if (lstr(s[i],1)='[') and (rstr(s[i],1)=']') then
         begin
          s[i]:=copy(s[i],2,length(s[i])-2);
          indirect:=true;
         end;

       {!labels}
       if (not found) and (s[i][1]='!') then
        begin
         ss:=s[i];
         ss:=btrim(rstr(ss,length(ss)-1));
         if numlabels>0 then
          for j:=1 to numlabels do
           if ss=labelname[j] then
            begin
             found:=true;
             if labelnum[j]>=0 then
               begin opcode:=labelnum[j]; microcode:=4; {resolved !label} end
              else
               begin opcode:=j; microcode:=3; {unresolved !label} end;
            end;
         if not found then
          begin
           inc(numlabels);
           if numlabels>max_labels then prog_error(15,'')
            else
             begin
              labelname[numlabels]:=ss;
              labelnum[numlabels]:=-1;
              opcode:=numlabels;
              microcode:=3; {unresolved !label}
              found:=true;
             end;
          end;
        end;

       {variables}
       if (numvars>0) and (not found) then
        for j:=1 to numvars do
         if s[i]=varname[j] then
          begin
           opcode:=varloc[j];
           microcode:=1; {variable}
           found:=true;
          end;
        //instructions
        if (s == "NOP")
        {
          opcode = 0;
          found = true;
        }
        else if(s == ("ADD"))
        {
          opcode = 1;
          found = true;
        }
        else if(s == ("SUB"))
        {
          opcode = 2;
          found = true;
        }
        else if(s == ("OR"))
        {
          opcode = 3;
          found = true;
        }
        else if(s == ("AND"))
        {
          opcode = 4;
          found = true;
        }
        else if(s == ("XOR"))
        {
          opcode = 5;
          found = true;
        }
        else if(s == ("NOT"))
        {
          opcode = 6;
          found = true;
        }
        else if(s == ("MPY"))
        {
          opcode = 7;
          found = true;
        }
        else if(s == ("DIV"))
        {
          opcode = 8;
          found = true;
        }
        else if(s == ("MOD"))
        {
          opcode = 9;
          found = true;
        }
        else if(s == ("RET") || s == ("RETURN"))
        {
          opcode = 10;
          found = true;
        }
        else if(s == ("GSB") || s == ("GOSUB") || s == ("CALL"))
        {
          opcode = 11;
          found = true;
        }
        else if(s == ("JMP") || s == ("JUMP") || s == ("GOTO"))
        {
          opcode = 12;
          found = true;
        }
        else if(s == ("JLS") || s == ("JB"))
        {
          opcode = 13;
          found = true;
        }
        else if(s == ("JGR") || s == ("JA"))
        {
          opcode = 14;
          found = true;
        }
        else if(s == ("JNE"))
        {
          opcode = 15;
          found = true;
        }
        else if(s == ("JEQ") || s == ("JE"))
        {
          opcode = 16;
          found = true;
        }
        else if(s == ("XCHG") || s == ("SWAP"))
        {
          opcode = 17;
          found = true;
        }
        else if(s == ("DO"))
        {
          opcode = 18;
          found = true;
        }
        else if(s == ("LOOP"))
        {
          opcode = 19;
          found = true;
        }
        else if(s == ("CMP"))
        {
          opcode = 20;
          found = true;
        }
        else if(s == ("TEST"))
        {
          opcode = 21;
          found = true;
        }
        else if(s == ("SET") || s == ("MOV"))
        {
          opcode = 22;
          found = true;
        }
        else if(s == ("LOC") || s == ("ADDR"))
        {
          opcode = 23;
          found = true;
        }
        else if(s == ("GET"))
        {
          opcode = 24;
          found = true;
        }
        else if(s == ("PUT"))
        {
          opcode = 25;
          found = true;
        }
        else if(s == ("INT"))
        {
          opcode = 26;
          found = true;
        }
        else if(s == ("IPO") || s == ("IN"))
        {
          opcode = 27;
          found = true;
        }
        else if(s == ("OPO") || s == ("OUT"))
        {
          opcode = 28;
          found = true;
        }
        else if(s == ("DEL") || s == ("DELAY"))
        {
          opcode = 29;
          found = true;
        }
        else if(s == ("PUSH"))
        {
          opcode = 30;
          found = true;
        }
        else if(s == ("POP"))
        {
          opcode = 31;
          found = true;
        }
        else if(s == ("ERR") || s == ("ERROR"))
        {
          opcode = 32;
          found = true;
        }
        else if(s == ("INC"))
        {
          opcode = 33;
          found = true;
        }
        else if(s == ("DEC"))
        {
          opcode = 34;
          found = true;
        }
        else if(s == ("SHL"))
        {
          opcode = 35;
          found = true;
        }
        else if(s == ("SHR"))
        {
          opcode = 36;
          found = true;
        }
        else if(s == ("ROL"))
        {
          opcode = 37;
          found = true;
        }
        else if(s == ("ROR"))
        {
          opcode = 38;
          found = true;
        }
        else if(s == ("JZ"))
        {
          opcode = 39;
          found = true;
        }
        else if(s == ("JNZ"))
        {
          opcode = 40;
          found = true;
        }
        else if(s == ("JAE") || s == ("JGE"))
        {
          opcode = 41;
          found = true;
        }
        else if(s == ("JLE") || s == ("JBE"))
        {
          opcode = 42;
          found = true;
        }
        else if(s == ("SAL"))
        {
          opcode = 43;
          found = true;
        }
        else if(s == ("SAR"))
        {
          opcode = 44;
          found = true;
        }
        else if(s == ("NEG"))
        {
          opcode = 45;
          found = true;
        }
        else if(s == ("JTL"))
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
          if(s == ("COLCNT"))
          {
            opcode = 8;
            microcode = 1;
            found = true;
          }
          else if(s == ("METERS"))
          {
            opcode = 9;
            microcode = 1;
            found = true;
          }
          else if(s == ("COMBASE"))
          {
            opcode = 10;
            microcode = 1;
            found = true;
          }
          else if(s == ("COMEND"))
          {
            opcode = 11;
            microcode = 1;
            found = true;
          }
          else if(s == ("FLAGS"))
          {
            opcode = 64;
            microcode = 1;
            found = true;
          }
          else if(s == ("AX"))
          {
            opcode = 65;
            microcode = 1;
            found = true;
          }
          else if(s == ("BX"))
          {
            opcode = 66;
            microcode = 1;
            found = true;
          }
          else if(s == ("CX"))
          {
            opcode = 67;
            microcode = 1;
            found = true;
          }
          else if(s == ("DX"))
          {
            opcode = 68;
            microcode = 1;
            found = true;
          }
          else if(s == ("EX"))
          {
            opcode = 69;
            microcode = 1;
            found = true;
          }
          else if(s == ("FX"))
          {
            opcode = 70;
            microcode = 1;
            found = true;
          }
          else if(s == ("SP"))
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
          if(s == ("MAXINT"))
          {
            opcode = 32767;
            microcode = 0;
            found = true;
          }
          else if(s == ("MININT"))
          {
            opcode = 32768;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_SPEDOMETER"))
          {
            opcode = 1;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_HEAT"))
          {
            opcode = 2;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_COMPASS"))
          {
            opcode = 3;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_TANGLE") || s == ("P_TURRET_OFS"))
          {
            opcode = 4;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_THEADING") || s == ("P_TURRET_ABS"))
          {
            opcode = 5;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_ARMOR") || s == ("P_DAMAGE"))
          {
            opcode = 6;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_SCAN"))
          {
            opcode = 7;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_ACCURACY"))
          {
            opcode = 8;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_RADAR"))
          {
            opcode = 9;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_RANDOM") || s == ("P_RAND"))
          {
            opcode = 10;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_THROTTLE"))
          {
            opcode = 11;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_TROTATE") || s == ("P_OFS_TURRET"))
          {
            opcode = 12;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_TAIM") || s == ("P_ABS_TURRET"))
          {
            opcode = 13;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_STEERING"))
          {
            opcode = 14;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_WEAP") || s == ("P_WEAPON") || s == ("P_FIRE"))
          {
            opcode = 15;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_SONAR"))
          {
            opcode = 16;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_ARC") || s == ("P_SCANARC"))
          {
            opcode = 17;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_OVERBURN"))
          {
            opcode = 18;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_TRANSPONDER"))
          {
            opcode = 19;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_SHUTDOWN"))
          {
            opcode = 20;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_CHANNEL"))
          {
            opcode = 21;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_MINELAYER"))
          {
            opcode = 22;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_MINETRIGGER"))
          {
            opcode = 23;
            microcode = 0;
            found = true;
          }
          else if(s == ("P_SHIELD") || s == ("P_SHIELDS"))
          {
            opcode = 24;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_DESTRUCT"))
          {
            opcode = 0;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_RESET"))
          {
            opcode = 1;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_LOCATE"))
          {
            opcode = 2;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_KEEPSHIFT"))
          {
            opcode = 3;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_OVERBURN"))
          {
            opcode = 4;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_ID"))
          {
            opcode = 5;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_TIMER"))
          {
            opcode = 6;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_ANGLE"))
          {
            opcode = 7;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_TID") || s == ("I_TARGETID"))
          {
            opcode = 8;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_TINFO") || s == ("I_TARGETINFO"))
          {
            opcode = 9;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_GINFO") || s == ("I_GAMEINFO"))
          {
            opcode = 10;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_RINFO") || s == ("I_ROBOTINFO"))
          {
            opcode = 11;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_COLLISIONS"))
          {
            opcode = 12;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_RESETCOLCNT"))
          {
            opcode = 13;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_TRANSMIT"))
          {
            opcode = 14;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_RECEIVE"))
          {
            opcode = 15;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_DATAREADY"))
          {
            opcode = 16;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_CLEARCOM"))
          {
            opcode = 17;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_KILLS") || s == ("I_DEATHS"))
          {
            opcode = 18;
            microcode = 0;
            found = true;
          }
          else if(s == ("I_CLEARMETERS"))
          {
            opcode = 19;
            microcode = 0;
            found = true;
          }
        }

{memory addresses} //this if exists to check different areas for what the assembly code wants if it is searching for a memory address
if (not found) and (s[i][1]='@') and (s[i][2] in ['0'..'9'])  then
 begin
  opcode:=str2int(rstr(s[i],length(s[i])-1));
  if (opcode<0) or (opcode>(max_ram+1)+(((max_code+1) shl 3)-1)) then
     prog_error(3,s[i]);
  microcode:=1; {variable}
  found:=true;
 end;

{numbers}
if (not found) and (s[i][1] in ['0'..'9','-'])  then //this is if the code has a number to indicate what they want to change something to
 begin
  opcode:=str2int(s[i]);
  found:=true;
 end;

if found then //when the code is found for what the assembly is doing, then it begins to start the process of the desired operation
 begin
  code[p].op[i]:=opcode;
  if indirect then microcode:=microcode or 8;
  code[p].op[max_op]:=code[p].op[max_op] or (microcode shl (i*4));
 end
else if s[i]<>'' then prog_error(2,s[i]);
end;
end;
if show_code then print_code(n,p);
if compile_by_line then readkey;
end;

procedure check_plen(plen:integer); //will need this if we have a max amount of code
begin
if plen>maxcode then
prog_error(16,#13#10+'Maximum program length exceeded, (Limit: '+
                      cstr(maxcode+1)+' compiled lines)');
end;

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

procedure reset_software(n:integer);
var
i:integer;
begin
with robot[n]^ do
begin
for i:=0 to max_ram do ram[i]:=0;
ram[71] := 768;
thd:=hd;  tspd:=0;
scanarc:=8; shift:=0; err:=0;
overburn:=false; keepshift:=false;
ip:=0; accuracy:=0;
meters:=0;
delay_left:=0; time_left:=0;
shields_up:=false;
end;
end;

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

function get_from_ram(n,i,j:integer):integer;
var k,l:integer;
begin
with robot[n]^ do
begin
if (i<0) or (i>(max_ram+1)+(((max_code+1) shl 3)-1)) then
begin k:=0; robot_error(n,4,cstr(i)); end
else
begin
 if i<=max_ram then k:=ram[i]
  else
   begin
    l:=i-max_ram-1;
    k:=code[l shr 2].op[l and 3];
   end;
end;
end;
get_from_ram:=k;
end;

function get_val(n,c,o:integer):integer;
var
i,j,k,l:integer;
begin
k:=0;
with robot[n]^ do
begin
j:=(code[c].op[max_op] shr (4*o)) and 15;
i:=code[c].op[o];
if (j and 7)=1 then
begin
k:=get_from_ram(n,i,j);
end else k:=i;
if (j and 8)>0 then
k:=get_from_ram(n,k,j);
end;
get_val:=k;
end;

procedure put_val(n,c,o,v:integer);
var
i,j,k:integer;
begin
k:=0; i:=0; j:=0;
with robot[n]^ do
begin
j:=(code[c].op[max_op] shr (4*o)) and 15;
i:=code[c].op[o];
if (j and 7)=1 then
begin
if (i<0) or (i>max_ram) then
  robot_error(n,4,cstr(i))
 else
  if (j and 8)>0 then
   begin
    i:=ram[i];
    if (i<0) or (i>max_ram) then
     robot_error(n,4,cstr(i))
    else ram[i]:=v;
   end
  else ram[i]:=v;
end
else
robot_error(n,3,'');
end;
end;

procedure push(n,v:integer);
begin
with robot[n]^ do
begin
if (ram[71] >= stack_base) and (ram[71] < (stack_base + stack_size)) then
begin
ram[ram[71]] := v;
inc(ram[71]);
end
else robot_error(n,1,cstr(ram[71]));
end;
end;

function pop(n:integer):integer;
var
k:integer;
begin
with robot[n]^ do
begin
if (ram[71] > stack_base) and (ram[71] <= (stack_base + stack_size)) then
begin
dec(ram[71]);
k := ram[ram[71]];
end
else robot_error(n,5,cstr(ram[71]));
end;
pop:=k;
end;

function find_label(n,l,m:integer):integer;
var
i,j,k:integer;
begin
k:=-1;
with robot[n]^ do
begin
if m=3 then robot_error(n,9,'')
else
if m=4 then k:=l
else
for i:=plen downto 0 do
begin
j:=code[i].op[max_op] and 15;
if (j=2) and (code[i].op[0]=l) then k:=i;
end;
end;
find_label:=k;
end;


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

procedure init_missile(xx,yy,xxv,yyv:real; dir,s,blast:integer; ob:boolean);
var
i,j,k:integer;
m:real;
sound:boolean;
begin
k:=-1; click;
for i:=max_missiles downto 0 do
if missile[i].a=0 then k:=i;
if k>=0 then
with missile[k] do
begin
source:=s;
x:=xx; lx:=x;
y:=yy; ly:=y;
rad:=0; lrad:=0;
if ob then mult:=1.25 else mult:=1;
if blast>0 then
 begin max_rad:=blast; a:=2; end
else
 begin
  if (s>=0) and (s<=num_robots) then
     mult:=mult*(robot[s]^.shotstrength);
  m:=mult;
  if ob then m:=m+0.25;
  mspd:=missile_spd*mult;
  if insane_missiles then mspd:=100+(50*insanity)*mult;
  if (s>=0) and (s<=num_robots) then
    begin
     inc(robot[s]^.heat,round(20*m));
     inc(robot[s]^.shots_fired);
     inc(robot[s]^.match_shots);
    end;
  a:=1; hd:=dir;
  max_rad:=mis_radius;
  if debug_info then
    begin writeln(#13,zero_pad(game_cycle,5),' F ',s,': hd=',hd,'           ');
    repeat until keypressed; flushkey; end;
 end;
end;
(*The following was to see if teh missile array is big enough*)
{else
begin
sound:=sound_on;
sound_on:=true;
chirp;
sound_on:=sound;
end;
setfillstyle(1,0);
setcolor(robot_color(k));
bar(5,5,37,12);
outtextxy(5,5,cstr(count_missiles));}
end;

procedure damage(n,d:integer; physical:boolean);
var
i,k,h,dd:integer;
m:real;
begin
if (n<0) or (n>num_robots) or (robot[n]^.armor<=0) then exit;
if robot[n]^.config.shield<3 then robot[n]^.shields_up:=false;
with robot[n]^ do
begin
h:=0;
if (shields_up) and (not physical) then
begin
dd:=d;
if (old_shields) and (config.shield>=3) then begin d:=0; h:=0; end
 else case config.shield of
  3:begin d:=round(dd*2/3); if d<1 then d:=1; h:=round(dd*2/3); end;
  4:begin h:=trunc(dd/2); d:=dd-h; end;
  5:begin d:=round(dd*1/3); if d<1 then d:=1;
          h:=round(dd*1/3); if h<1 then h:=1; end;
 end;
end;
if d<0 then d:=0;
if debug_info then
begin writeln(#13,zero_pad(game_cycle,5),' D ',n,': ',armor,'-',d,'=',armor-d,'           ');
repeat until keypressed; flushkey; end;
if d>0 then
begin d:=round(d*damageadj); if d<1 then d:=1; end;
dec(armor,d);
inc(heat,h);
last_damage:=0;
if armor<=0 then
begin
armor:=0;
update_armor(n);
heat:=500;
update_heat(n);
armor:=0;
inc(kill_count);
inc(deaths);
update_lives(n);
if graphix and timing then time_delay(10);
draw_robot(n);
heat:=0;
update_heat(n);
init_missile(x,y,0,0,0,n,blast_circle,false);
if overburn then m:=1.3 else m:=1;
for i:=0 to num_robots do
 if (i<>n) and (robot[i]^.armor>0) then
  begin
   k:=round(distance(x,y,robot[i]^.x,robot[i]^.y));
   if k<blast_radius then
    damage(i,round(abs(blast_radius-k)*m),false);
  end;
end;
end;
end;

function scan(n:integer):integer;
var
r,d,acc:real;
dir,range,i,j,k,l,nn,xx,yy,sign:integer;
begin
nn:=-1;
range:=maxint;
if not (n in [0..num_robots]) then exit;
with robot[n]^ do
begin
if scanarc<0 then scanarc:=0;
accuracy:=0; nn:=-1;
dir:=(shift+hd) and 255;
if debug_info then writeln('<SCAN Arc=',scanarc,', Dir=',dir,'>');
for i:=0 to num_robots do
if (i<>n) and (robot[i]^.armor>0) then
begin
 j:=find_anglei(x,y,robot[i]^.x,robot[i]^.y);
 d:=distance(x,y,robot[i]^.x,robot[i]^.y);
 k:=round(d);
 if (k<range) and (k<=scanrange) and ((abs(j-dir)<=abs(scanarc)) or (abs(j-dir)>=256-abs(scanarc))) then
  begin
   dir:=(dir+1024) and 255;
   xx:=round( sint[dir]*d+x);
   yy:=round(-cost[dir]*d+y);
   r:=distance(xx,yy,robot[i]^.x,robot[i]^.y);
   if debug_info then begin
     writeln('SCAN HIT! Scan X,Y: ',round(xx),',',round(yy),'  Robot X,Y: ',
              round(robot[i]^.x),',',round(robot[i]^.y),'  Dist=',round(r));
     repeat until keypressed; flushkey; end;
   if (scanarc>0) or (r<hit_range-2) then
    begin
     range:=k;
     accuracy:=0;
     if scanarc>0 then
      begin
       j:=(j+1024) and 255; dir:=(dir+1024) and 255;
       if (j<dir) then sign:=-1;
       if (j>dir) then sign:= 1;
       if (j>190) and (dir<66) then begin dir:=dir+256; sign:=-1; end;
       if (dir>190) and (j<66) then begin   j:=  j+256; sign:= 1; end;
       acc:=abs(j-dir)/scanarc*2;
       if sign<0 then accuracy:=-abs(round(acc))
                 else accuracy:=abs(round(acc));
       if accuracy>2 then accuracy:=2;
       if accuracy<-2 then accuracy:=-2;
      end;
     nn:=i;
     if debug_info then
      begin writeln(#13,zero_pad(game_cycle,5),' S ',n,': nn=',nn,', range=',range,', acc=',accuracy,'           ');
      repeat until keypressed; flushkey; end;
    end;
  end;
end;
if nn in [0..num_robots] then
begin
ram[5]:=robot[nn]^.transponder;
ram[6]:=(robot[nn]^.hd-(hd+shift)+1024) and 255;
ram[7]:=robot[nn]^.spd;
ram[13]:=round(robot[nn]^.speed*100);
end;
end;
scan:=range;
end;

procedure com_transmit(n,chan,data:integer);
var
i,j,k:integer;
begin
for i:=0 to num_robots do
with robot[i]^ do
if (armor>0) and (i<>n) and (channel=chan) then
begin
if (ram[10]<0) or (ram[10]>max_queue) then ram[10]:=0;
if (ram[11]<0) or (ram[11]>max_queue) then ram[11]:=0;
ram[ram[11]+com_queue]:=data;
inc(ram[11]);
if (ram[11]>max_queue) then ram[11]:=0;
if (ram[11]=ram[10]) then inc(ram[10]);
if (ram[10]>max_queue) then ram[10]:=0;
end;
end;

function com_receive(n:integer):integer;
var
i,j,k:integer;
begin
k:=0;
with robot[n]^ do
begin
if (ram[10]<>ram[11]) then
begin
if (ram[10]<0) or (ram[10]>max_queue) then ram[10]:=0;
if (ram[11]<0) or (ram[11]>max_queue) then ram[11]:=0;
k:=ram[ram[10]+com_queue];
inc(ram[10]);
if (ram[10]>max_queue) then ram[10]:=0;
end
else robot_error(n,12,'');
end;
com_receive:=k;
end;

function in_port(n,p:integer; var time_used:integer):integer;
var
v,i,j,k,l,nn:integer;
begin
v:=0;
with robot[n]^ do
begin
case p of
01:v:=spd;
02:v:=heat;
03:v:=hd;
04:v:=shift;
05:v:=(shift+hd) and 255;
06:v:=armor;
07:begin v:=scan(n); inc(time_used); if show_arcs then arc_count:=2; end;
08:begin v:=accuracy; inc(time_used); end;
09:begin
   nn:=-1;
   inc(time_used,3); k:=maxint; nn:=-1;
   for i:=0 to num_robots do
    begin
     j:=round(distance(x,y,robot[i]^.x,robot[i]^.y));
     if (n<>i) and (j<k) and (robot[i]^.armor>0) then begin k:=j; nn:=i; end;
    end;
   v:=k;
   if nn in [0..num_robots] then
      ram[5]:=robot[nn]^.transponder;
  end;
10:v:=random(65535)+random(2);
16:begin
   nn:=-1;
   if show_arcs then sonar_count:=2;
   inc(time_used,40);
   l:=-1; k:=maxint; nn:=-1;
   for i:=0 to num_robots do
    begin
     j:=round(distance(x,y,robot[i]^.x,robot[i]^.y));
     if (n<>i) and (j<k) and (j<max_sonar) and (robot[i]^.armor>0) then
        begin k:=j; l:=i; nn:=i; end;
    end;
   if l>=0 then v:=(round(find_angle(x,y,robot[l]^.x,robot[l]^.y)/pi*128+
                         1024+random(65)-32) and 255)
           else v:=minint;
   if nn in [0..num_robots] then
      ram[5]:=robot[nn]^.transponder;
  end;
17:v:=scanarc;
18:if overburn then v:=1 else v:=0;
19:v:=transponder;
20:v:=shutdown;
21:v:=channel;
22:v:=mines;
23:if config.mines>=0 then begin
   k:=0;
   for i:=0 to max_mines do
    with mine[i] do
     if (x>=0) and (x<=1000) and (y>=0) and (y<=1000) and (yield>0) then inc(k);
   v:=k;
  end else v:=0;
24:if config.shield>0 then
   begin if shields_up then v:=1 else v:=0;
   end else begin v:=0; shields_up:=false; end;
else robot_error(n,11,cstr(p));
end;
end;
in_port:=v;
end;

procedure out_port(n,p,v:integer; var time_used:integer);
var
i:integer;
begin
with robot[n]^ do
begin
case p of
11:tspd:=v;
12:shift:=(shift+v+1024) and 255;
13:shift:=(v+1024) and 255;
14:thd:=(thd+v+1024) and 255;
15:begin
   inc(time_used,3);
   if v>4 then v:=4;
   if v<-4 then v:=-4;
   init_missile(x,y,xv,yv,(hd+shift+v) and 255,n,0,overburn);
  end;
17:scanarc:=v;
18:if v=0 then overburn:=false else overburn:=true;
19:transponder:=v;
20:shutdown:=v;
21:channel:=v;
22:if config.mines>=0 then
   begin if mines>0 then begin init_mine(n,v,mine_blast); dec(mines); end else robot_error(n,14,'');
   end else robot_error(n,13,'');
23:if config.mines>=0 then
   begin for i:=0 to max_mines do mine[i].detonate:=true;
   end else robot_error(n,13,'');
24:if config.shield>=3 then
   begin if v=0 then shields_up:=false else shields_up:=true;
   end else begin shields_up:=false; robot_error(n,15,''); end;
else robot_error(n,11,cstr(p));
end;
if scanarc>64 then scanarc:=64; if scanarc<0 then scanarc:=0;
end;
end;

procedure call_int(n,int_num:integer; var time_used:integer);
var
i,j,k:integer;
begin
with robot[n]^ do
begin
case int_num of
00:damage(n,1000,true);
01:begin
   reset_software(n);
   time_used:=10;
  end;
02:begin
   time_used:=5;
   ram[69]:=round(x);
   ram[70]:=round(y);
  end;
03:begin
   time_used:=2;
   if ram[65]=0 then keepshift:=false else keepshift:=true;
   ram[70]:=shift and 255;
  end;
04:begin
   if ram[65]=0 then overburn:=false else overburn:=true;
  end;
05:begin
   time_used:=2;
   ram[70]:=transponder;
  end;
06:begin
   time_used:=2;
   ram[69]:=game_cycle shr 16;
   ram[70]:=game_cycle and 65535;
  end;
07:begin
   j:=ram[69]; k:=ram[70];
   if j<0 then j:=0; if j>1000 then j:=1000;
   if k<0 then k:=0; if k>1000 then k:=1000;
   ram[65]:=round(find_angle(round(x),round(y),j,k)/pi*128+256) and 255;
   time_used:=32;
  end;
08:begin
   ram[70]:=ram[5];
   time_used:=1;
  end;
09:begin
   ram[69]:=ram[6];
   ram[70]:=ram[7];
   time_used:=2;
  end;
10:begin
   k:=0;
   for i:=0 to num_robots do
    if robot[i]^.armor>0 then inc(k);
   ram[68]:=k;
   ram[69]:=played;
   ram[70]:=matches;
   time_used:=4;
  end;
11:begin
   ram[68]:=round(speed*100);
   ram[69]:=last_damage;
   ram[70]:=last_hit;
   time_used:=5;
  end;
12:begin
   ram[70]:=ram[8];
   time_used:=1;
  end;
13:begin
   ram[8]:=0;
   time_used:=1;
  end;
14:begin
   com_transmit(n,channel,ram[65]);
   time_used:=1;
  end;
15:begin
   if (ram[10]<>ram[11]) then
    ram[70]:=com_receive(n)
   else
    robot_error(n,12,'');
   time_used:=1;
  end;
16:begin
   if (ram[11]>=ram[10]) then
    ram[70]:=ram[11]-ram[10]
   else
    begin
     ram[70]:=max_queue+1-ram[10]+ram[11];
    end;
   time_used:=1;
  end;
17:begin
   ram[10]:=0;
   ram[11]:=0;
   time_used:=1;
  end;
18:begin
   ram[68]:=kills;
   ram[69]:=kills-startkills;
   ram[70]:=deaths;
   time_used:=3;
  end;
19:begin ram[9]:=0; meters:=0; end;
else robot_error(n,10,cstr(int_num));
end;
end;
end;

procedure jump(n,o:integer; var inc_ip:boolean);
var
i,j,k,l,loc:integer;
begin
with robot[n]^ do
begin
loc:=find_label(n,get_val(n,ip,o),code[ip].op[max_op] shr (o*4));
if (loc>=0) and (loc<=plen) then begin inc_ip:=false; ip:=loc; end
                          else robot_error(n,2,cstr(loc));
end;
end;

{FIFI}
procedure update_debug_bars;
begin
if graphix and (step_mode>0) then with robot[0]^ do
begin
{debugger window}
viewport(480,4,635,410);

{armour}
setfillstyle(1,robot_color(0));
if armor>0 then
bar(88,03,87+(armor shr 2),08);
setfillstyle(1,8);
if armor<100 then
bar(88+(armor shr 2),03,111,08);

{heat}
setfillstyle(1,robot_color(0));
if heat>5 then
bar(127,03,126+(heat div 20),08);
setfillstyle(1,8);
if heat<500 then
bar(127+(heat div 20),03,151,08);
end;
end;
{/FIFI}

{FIFI}
procedure update_debug_system;
var
i:integer;
begin
if graphix and (step_mode>0) then with robot[0]^ do
begin
{debugger window}
viewport(480,4,635,410);
setfillstyle(1,0);
{for i:=0 to 6 do
begin
bar(039,32+(i*10),70,39+(i*10));
bar(110,32+(i*10),141,39+(i*10));
end;} {Commented out the black bars, and switched to 'textxy' -Bones}
setcolor(robot_color(0));
textxy(039,32,hex(ram[0])); textxy(110,32,hex(ram[7]));
textxy(039,42,hex(ram[1])); textxy(110,42,hex(ram[8]));
textxy(039,52,hex(ram[2])); textxy(110,52,hex(ram[9]));
textxy(039,62,hex(ram[3])); textxy(110,62,hex(ram[10]));
textxy(039,72,hex(ram[4])); textxy(110,72,hex(ram[11]));
textxy(039,82,hex(ram[5])); textxy(110,82,hex(ram[12]));
textxy(039,92,hex(ram[6])); textxy(110,92,hex(ram[13]));
end;
end;
{/FIFI}

{FIFI}
procedure update_debug_registers;
var
i:integer;
begin
if graphix and (step_mode>0) then with robot[0]^ do
begin
{debugger window}
viewport(480,4,635,410);
setfillstyle(1,0);
{for i:=0 to 4 do
begin
bar(039,122+(i*10),70,129+(i*10));
bar(110,122+(i*10),141,129+(i*10));
end;} {Commented out the black bars, and switched to 'textxy' -Bones}
setcolor(robot_color(0));
textxy(039,122,hex(ram[65])); textxy(110,122,hex(ram[69]));
textxy(039,132,hex(ram[66])); textxy(110,132,hex(ram[70]));
textxy(039,142,hex(ram[67]));
textxy(039,152,hex(ram[68])); textxy(110,152,hex(ram[71]));
end;
end;
{/FIFI}


{FIFI}
procedure update_debug_flags;
var
bin_string:string;
begin
if graphix and (step_mode>0) then with robot[0]^ do
begin
{debugger window}
viewport(480,4,635,410);
bin_string:=bin(ram[64]);
if bin_string[13]='1' then setcolor(robot_color(0)) else setcolor(robot_color(8));
outtextxy(111,172,'Z');
if bin_string[14]='1' then setcolor(robot_color(0)) else setcolor(robot_color(8));
outtextxy(119,172,'G');
if bin_string[15]='1' then setcolor(robot_color(0)) else setcolor(robot_color(8));
outtextxy(127,172,'L');
if bin_string[16]='1' then setcolor(robot_color(0)) else setcolor(robot_color(8));
outtextxy(135,172,'E');
{setfillstyle(1,0);
bar(015,182,142,189);
setcolor(robot_color(0));}
{Commented out the black bars, and switched to 'textxy' -Bones}
textxy(015,182,bin(ram[64]));
end;
end;
{/FIFI}

{FIFI}
procedure update_debug_memory;
var
i:integer;
begin
if graphix and (step_mode>0) then with robot[0]^ do
begin
{debugger window}
viewport(480,4,635,410);
{setfillstyle(1,0);
for i:=0 to 9 do
bar(091,212+(i*10),123,222+(i*10));}
{Commented out the black bars, and switched to 'textxy' -Bones}
setcolor(robot_color(0));
for i:=0 to 9 do
if ((mem_watch+i)>=0) and ((mem_watch+i)<=1023) then
begin
 textxy(035,212+(10*i),decimal((mem_watch+i),4) + ' :');
 textxy(091,212+(10*i),hex(ram[mem_watch+i]));
end;
end;
end;
{/FIFI}

{FIFI}
procedure update_debug_code;
var
i:integer;
begin
if graphix and (step_mode>0) then with robot[0]^ do
begin
{debugger window}
viewport(480,4,635,410);
{code}
setfillstyle(1,0);
for i:=0 to 6 do bar(003,332+(i*10),152,340+(i*10));
setcolor(7);
for i:=0 to 6 do
if (ip+i-3)>=0 then
begin
 if i=3 then setcolor(robot_color(0));
 textxy(003,332+(i*10),decimal(ip+i-3,4)+': '
       +mnemonic(code[ip+i-3].op[0],code[ip+i-3].op[3] and 15)+' '
       +operand(code[ip+i-3].op[1],(code[ip+i-3].op[3] shr 4) and 15)+','
       +operand(code[ip+i-3].op[2],(code[ip+i-3].op[3] shr 8) and 15));
 if i=3 then setcolor(7);
end;
end;
end;
{/FIFI}


{FIFI}
procedure update_debug_window;
begin
if graphix and (step_mode>0) then
begin

{armour + heat}
update_debug_bars;

{system variables}
update_debug_system;

{registers}
update_debug_registers;

{flag register}
update_debug_flags;

{memory}
update_debug_memory;

{code}
update_debug_code;

end;
end;
{/FIFI}

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

function gameover:boolean;
var
n,i,j,k,l:integer;
begin
if (game_cycle>=game_limit) and (game_limit>0) then
begin gameover:=true; exit; end;
if game_cycle and 31=0 then
begin
k:=0;
for n:=0 to num_robots do
if robot[n]^.armor>0 then inc(k);
if k<=1 then gameover:=true
      else gameover:=false;
end else gameover:=false;
end;

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

function invalid_microcode(n,ip:integer):boolean;
var
invalid:boolean;
i,j,k:integer;
begin
invalid:=false;
for i:=0 to 2 do
begin
k:=(robot[n]^.code[ip].op[max_op] shr (i shl 2)) and 7;
if not (k in [0,1,2,4]) then invalid:=true;
end;
invalid_microcode:=invalid;
end;

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

procedure execute_instruction(n:integer);
var
i,j,k:integer;
time_used,loc:integer;
inc_ip:boolean;
{FIFI}
c:char;
{/FIFI}
begin
with robot[n]^ do
begin
{--update system variables--}
ram[000]:=tspd;
ram[001]:=thd;
ram[002]:=shift;
ram[003]:=accuracy;

time_used:=1; inc_ip:=true; loc:=0;
if (ip>plen) or (ip<0) then ip:=0;
if invalid_microcode(n,ip) then
 begin time_used:=1; robot_error(n,16,hex(code[ip].op[max_op])); end
else

{FIFI}
if graphix and (step_mode>0) and (n=0) then  {if stepping enabled...}
begin
inc(step_count);
update_cycle_window;
update_debug_window;
if (step_count mod step_mode)=0 then step_loop:=true else step_loop:=false;
while step_loop and (not(quit or gameover or bout_over)) do
if keypressed then with robot[0]^ do
 begin
  c:=upcase(readkey);
  case c of
   'X':begin
        temp_mode:=step_mode;
        step_mode:=0;
        step_loop:=false;
        close_debug_window;
       end;
   ' ':begin step_count:=0; step_loop:=false; end;
   '1'..'9':begin step_mode:=value(c); step_count:=0; step_loop:=false; end;
   '0':begin step_mode:=10; step_count:=0; step_loop:=false; end;
   '-','_':if mem_watch>0 then
            begin
             setcolor(0);
             for i:=0 to 9 do
              outtextxy(035,212+(10*i),decimal((mem_watch+i),4) + ' :');
             dec(mem_watch);
             update_debug_memory;
            end;
   '+','=':if mem_watch<1014 then
            begin
             setcolor(0);
             for i:=0 to 9 do
              outtextxy(035,212+(10*i),decimal((mem_watch+i),4) + ' :');
             inc(mem_watch);
             update_debug_memory;
            end;
   '[','{':if mem_watch>0 then
            begin
             setcolor(0);
             for i:=0 to 9 do
              outtextxy(035,212+(10*i),decimal((mem_watch+i),4) + ' :');
             dec(mem_watch,10);
             if mem_watch<0 then mem_watch:=0;
             update_debug_memory;
            end;
   ']','}':if mem_watch<1014 then
            begin
             setcolor(0);
             for i:=0 to 9 do
              outtextxy(035,212+(10*i),decimal((mem_watch+i),4) + ' :');
             inc(mem_watch,10);
             if mem_watch>1014 then mem_watch:=1014;
             update_debug_memory;
            end;
   'G':begin toggle_graphix; temp_mode:=step_mode; step_mode:=0; step_loop:=false; end;
   else process_keypress(c);
  end;
 end;
end;
{/FIFI}

if (not ((code[ip].op[max_op] and 7) in [0,1])) then
 begin time_used:=0; end
else
case get_val(n,ip,0) of
00:begin (*NOP*)
   inc(executed);
  end;
01:begin (*ADD*)
   put_val(n,ip,1,get_val(n,ip,1)+get_val(n,ip,2));
   inc(executed);
  end;
02:begin (*SUB*)
   put_val(n,ip,1,get_val(n,ip,1)-get_val(n,ip,2));
   inc(executed);
  end;
03:begin (*OR*)
   put_val(n,ip,1,get_val(n,ip,1) or get_val(n,ip,2));
   inc(executed);
  end;
04:begin (*AND*)
   put_val(n,ip,1,get_val(n,ip,1) and get_val(n,ip,2));
   inc(executed);
  end;
05:begin (*XOR*)
   put_val(n,ip,1,get_val(n,ip,1) xor get_val(n,ip,2));
   inc(executed);
  end;
06:begin (*NOT*)
   put_val(n,ip,1,not(get_val(n,ip,1)));
   inc(executed);
  end;
07:begin (*MPY*)
   put_val(n,ip,1,get_val(n,ip,1)*get_val(n,ip,2));
   time_used:=10;
   inc(executed);
  end;
08:begin (*DIV*)
   j:=get_val(n,ip,2);
   if j<>0 then put_val(n,ip,1,get_val(n,ip,1) div j)
           else robot_error(n,8,'');
   time_used:=10;
   inc(executed);
  end;
09:begin (*MOD*)
   j:=get_val(n,ip,2);
   if j<>0 then put_val(n,ip,1,get_val(n,ip,1) mod j)
           else robot_error(n,8,'');
   time_used:=10;
   inc(executed);
  end;
10:begin (*RET*)
   ip:=pop(n);
   if (ip<0) or (ip>plen) then robot_error(n,7,cstr(ip));
   inc(executed);
  end;
11:begin (*GSB*)
   loc:=find_label(n,get_val(n,ip,1),code[ip].op[max_op] shr (1*4));
   if loc>=0 then begin push(n,ip); inc_ip:=false; ip:=loc; end
             else robot_error(n,2,cstr(get_val(n,ip,1)));
   inc(executed);
  end;
12:begin (*JMP*)
   jump(n,1,inc_ip);
   inc(executed);
  end;
13:begin (*JLS,JB*)
   if ram[64] and 2>0 then
      jump(n,1,inc_ip);
   time_used:=0;
   inc(executed);
  end;
14:begin (*JGR,JA*)
   if ram[64] and 4>0 then
      jump(n,1,inc_ip);
   time_used:=0;
   inc(executed);
  end;
15:begin (*JNE*)
   if ram[64] and 1=0 then
      jump(n,1,inc_ip);
   time_used:=0;
   inc(executed);
  end;
16:begin (*JEQ,JE*)
   if ram[64] and 1>0 then
      jump(n,1,inc_ip);
   time_used:=0;
   inc(executed);
  end;
17:begin (*SWAP, XCHG*)
   ram[4]:=get_val(n,ip,1);
   put_val(n,ip,1,get_val(n,ip,2));
   put_val(n,ip,2,ram[4]);
   time_used:=3;
   inc(executed);
  end;
18:begin (*DO*)
   ram[67]:=get_val(n,ip,1);
   inc(executed);
  end;
19:begin (*LOOP*)
   dec(ram[67]);
   if ram[67]>0 then
      jump(n,1,inc_ip);
   inc(executed);
  end;
20:begin (*CMP*)
   k:=get_val(n,ip,1)-get_val(n,ip,2);
   ram[64]:=ram[64] and $FFF0;
   if k=0 then ram[64]:=ram[64] or 1;
   if k<0 then ram[64]:=ram[64] or 2;
   if k>0 then ram[64]:=ram[64] or 4;
   if (get_val(n,ip,2)=0) and (k=0) then
               ram[64]:=ram[64] or 8;
   inc(executed);
  end;
21:begin (*TEST*)
   k:=get_val(n,ip,1) and get_val(n,ip,2);
   ram[64]:=ram[64] and $FFF0;
   if k=get_val(n,ip,2) then ram[64]:=ram[64] or 1;
   if k=0               then ram[64]:=ram[64] or 8;
   inc(executed);
  end;
22:begin (*MOV, SET*)
   put_val(n,ip,1,get_val(n,ip,2));
   inc(executed);
  end;
23:begin (*LOC*)
   put_val(n,ip,1,code[ip].op[2]);
   time_used:=2;
   inc(executed);
  end;
24:begin (*GET*)
   k:=get_val(n,ip,2);
   if (k>=0) and (k<=max_ram) then
       put_val(n,ip,1,ram[k])
     else if (k>max_ram) and (k<=(Max_ram+1)+(((max_code+1) shl 3)-1)) then
      begin
       j:=k-max_ram-1;
       put_val(n,ip,1,code[j shr 2].op[j and 3]);
      end
     else robot_error(n,4,cstr(k));
   time_used:=2;
   inc(executed);
  end;
25:begin (*PUT*)
   k:=get_val(n,ip,2);
   if (k>=0) and (k<=max_ram) then
       ram[k]:=get_val(n,ip,1)
     else robot_error(n,4,cstr(k));
   time_used:=2;
   inc(executed);
  end;
26:begin (*INT*)
   call_int(n,get_val(n,ip,1),time_used);
   inc(executed);
  end;
27:begin (*IPO,IN*)
   time_used:=4;
   put_val(n,ip,2,in_port(n,get_val(n,ip,1),time_used));
   inc(executed);
  end;
28:begin (*OPO,OUT*)
   time_used:=4;
   out_port(n,get_val(n,ip,1),get_val(n,ip,2),time_used);
   inc(executed);
  end;
29:begin (*DEL,DELAY*)
   time_used:=get_val(n,ip,1);
   inc(executed);
  end;
30:begin (*PUSH*)
   push(n,get_val(n,ip,1));
   inc(executed);
  end;
31:begin (*POP*)
   put_val(n,ip,1,pop(n));
   inc(executed);
  end;
32:begin (*ERR*)
   robot_error(n,get_val(n,ip,1),'');
   time_used:=0;
   inc(executed);
  end;
33:begin (*INC*)
   put_val(n,ip,1,get_val(n,ip,1)+1);
   inc(executed);
  end;
34:begin (*DEC*)
   put_val(n,ip,1,get_val(n,ip,1)-1);
   inc(executed);
  end;
35:begin (*SHL*)
   put_val(n,ip,1,get_val(n,ip,1) shl get_val(n,ip,2));
   inc(executed);
  end;
36:begin (*SHR*)
   put_val(n,ip,1,get_val(n,ip,1) shr get_val(n,ip,2));
   inc(executed);
  end;
37:begin (*ROL*)
   put_val(n,ip,1,rol(get_val(n,ip,1),get_val(n,ip,2)));
   inc(executed);
  end;
38:begin (*ROR*)
   put_val(n,ip,1,ror(get_val(n,ip,1),get_val(n,ip,2)));
   inc(executed);
  end;
39:begin (*JZ*)
   time_used:=0;
   if ram[64] and 8>0 then
      jump(n,1,inc_ip);
   inc(executed);
  end;
40:begin (*JNZ*)
   time_used:=0;
   if ram[64] and 8=0 then
      jump(n,1,inc_ip);
   inc(executed);
  end;
41:begin (*JAE,JGE*)
   if (ram[64] and 1>0) or (ram[64] and 4>0) then
      jump(n,1,inc_ip);
   time_used:=0;
   inc(executed);
  end;
42:begin (*JBE,JLE*)
   if (ram[64] and 1>0) or (ram[64] and 2>0) then
      jump(n,1,inc_ip);
   time_used:=0;
   inc(executed);
  end;
43:begin (*SAL*)
   put_val(n,ip,1,sal(get_val(n,ip,1),get_val(n,ip,2)));
   inc(executed);
  end;
44:begin (*SAR*)
   put_val(n,ip,1,sar(get_val(n,ip,1),get_val(n,ip,2)));
   inc(executed);
  end;
45:begin (*NEG*)
   put_val(n,ip,1,0-get_val(n,ip,1));
   inc(executed);
  end;
46:begin (*JTL*)
   loc:=get_val(n,ip,1);
   if (loc>=0) and (loc<=plen) then begin inc_ip:=false; ip:=loc; end
                               else robot_error(n,2,cstr(loc));
  end;
else begin robot_error(n,6,''); end;
end;
inc(delay_left,time_used);
if inc_ip then inc(ip);
{FIFI}
if graphix and (n=0) and (step_mode>0) then update_debug_window;
{/FIFI}
end;
end;

procedure do_robot(n:integer);
var
i,j,k,l,tthd:integer;
heat_mult,ttx,tty:real;
begin
if (n<0) or (n>num_robots) then exit; //if there are no robots then don't do this
with robot[n]^ do
begin
if armor<=0 then exit;

{--time slice--}
time_left:=time_slice; //this seems to be checking the time left in the match compaired to the current time
if (time_left>robot_time_limit) and (robot_time_limit>0) then
 time_left:=robot_time_limit;
if (time_left>max_time) and (max_time>0) then time_left:=max_time;
executed:=0;

{--execute timeslice--} //this checks the heat to lower the shields if the robot is over heating and make the robot take damage over time
while (time_left>0) and (not cooling) and (executed<20+time_slice) and (armor>0) do
begin
 if delay_left<0 then delay_left:=0;
 if (delay_left>0) then begin dec(delay_left); dec(time_left); end;
 if (time_left>=0) and (delay_left=0) then
   execute_instruction(n);
 if heat>=shutdown then begin cooling:=true; shields_up:=false; end;
 if heat>=500 then damage(n,1000,true);
end;

{--fix up variables--} //this is figuring out speeds, and when the robot was last hit and damaged
thd:=(thd+1024) and 255;
hd:=(hd+1024) and 255;
shift:=(shift+1024) and 255;
if tspd<-75 then tspd:=-75;
if tspd>100 then tspd:=100;
if spd<-75 then spd:=-75;
if spd>100 then spd:=100;
if heat<0 then heat:=0;
if last_damage<maxint then inc(last_damage);
if last_hit<maxint then inc(last_hit);

{--update heat--} //going to be very necessary, can probably be written better though, will need to look at this for physics update as well
if shields_up and (game_cycle and 3=0) then inc(heat);
if not shields_up then
begin
if heat>0 then
 case config.heatsinks of  (*heat adjustments*)
  5:if game_cycle and 1=0 then dec(heat);
  4:if game_cycle mod 3=0 then dec(heat);
  3:if game_cycle and 3=0 then dec(heat);
  2:if game_cycle and 7=0 then dec(heat);
  1:begin (*Do nothing, no adjustment needed*) end;
  else if game_cycle and 3=0 then inc(heat);
 end;
if overburn and (game_cycle mod 3=0) then inc(heat);
if (heat>0) then dec(heat);
if (heat>0) and (game_cycle and 7=0) and (abs(tspd)<=25) then dec(heat);
if (heat<=shutdown-50) or (heat<=0) then cooling:=false;
end;
if cooling then begin tspd:=0; end;
heat_mult:=1;
case heat of
080..099:heat_mult:=0.98;
100..149:heat_mult:=0.95;
150..199:heat_mult:=0.85;
200..249:heat_mult:=0.75;
250..maxint:heat_mult:=0.50;
end;
if overburn then heat_mult:=heat_mult*1.30;
if (heat>=475) and (game_cycle and 03=0) then damage(n,1,true) else
if (heat>=450) and (game_cycle and 07=0) then damage(n,1,true) else
if (heat>=400) and (game_cycle and 15=0) then damage(n,1,true) else
if (heat>=350) and (game_cycle and 31=0) then damage(n,1,true) else
if (heat>=300) and (game_cycle and 63=0) then damage(n,1,true);

{--update robot in physical world--}
{-acceleration-} //checking speed with the current acceleration of the robot, going to be important
if abs(spd-tspd)<=acceleration then spd:=tspd
 else begin
       if tspd>spd then inc(spd,acceleration)
                   else dec(spd,acceleration);
      end;
{-turning-} //will be important for turning the robots
tthd:=hd+shift;
if (abs(hd-thd)<=turn_rate) or (abs(hd-thd)>=256-turn_rate) then
 hd:=thd
else if hd<>thd then
begin
 k:=0;
 if ((thd>hd) and (abs(hd-thd)<=128)) orj
    ((thd<hd) and (abs(hd-thd)>=128)) then k:=1;
 if k=1 then hd:=(hd+turn_rate) and 255
        else hd:=(hd+256-turn_rate) and 255;
end;
hd:=hd and 255;
if keepshift then shift:=(tthd-hd+1024) and 255;
{-moving-} //VITAL
speed:=spd/100*(max_vel*heat_mult*speedadj);
xv:= sint[hd]*speed;
yv:=-cost[hd]*speed;
if (hd=0) or (hd=128) then xv:=0;
if (hd=64) or (hd=192) then yv:=0;
if xv<>0 then ttx:=x+xv else ttx:=x;
if yv<>0 then tty:=y+yv else tty:=y;
if (ttx<0) or (tty<0) or (ttx>1000) or (tty>1000) then
 begin inc(ram[8]); tspd:=0; if abs(speed)>=max_vel/2 then damage(n,1,true); spd:=0; {ttx:=x; tty:=y;} end;
for i:=0 to num_robots do
if (i<>n) and (robot[i]^.armor>0) and (distance(ttx,tty,robot[i]^.x,robot[i]^.y)<crash_range)
  then begin
        tspd:=0; spd:=0; ttx:=x; tty:=y;
        robot[i]^.tspd:=0; robot[i]^.spd:=0;
        inc(ram[8]); inc(robot[i]^.ram[8]);
        if abs(speed)>=max_vel/2 then begin damage(n,1,true); damage(i,1,true); end;
       end;
if ttx<0 then ttx:=0;
if tty<0 then tty:=0;
if ttx>1000 then ttx:=1000;
if tty>1000 then tty:=1000;
meters:=meters+distance(x,y,ttx,tty);
if meters>=maxint then meters:=meters-maxint;
ram[9]:=trunc(meters);
x:=ttx; y:=tty;


{--draw robot--} //more graphics, could probably do without these
if armor<0 then armor:=0;
if heat<0 then heat:=0;
if graphix then
begin
if armor<>larmor then update_armor(n);
if heat div 5<>lheat div 5 then update_heat(n);
draw_robot(n);
end;
lheat:=heat; larmor:=armor;

{--Cycle complete, inc counter--} //may need a counter for cycles
inc(cycles_lived);
end;
end;

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
