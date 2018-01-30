/*File Name: ATRLOCK.cpp
  Contributer(s): Will Csont
  Date: 1/24/18
  Language: C++ (gcc compiler)
  Version: 1.01
  Description: Currently mostly just a shell with some variables declared and functions declared with no
  code in most functions except for encode. The current build doesn't run.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

//uses functions from the stuff and filelib files. look into later

#define locktype 3


string fn1,fn2,s,s1,s2,lock_code;

int i,j,k,lock_pos,lock_dat;
bool this_dat;

//missing f1 and f2 of type text need to look into that later



    string encode(string s)                   //********THE FOLLOWING ARE ONLY JUST EMPTY FUNCTIONS TO BE CODED LATER******
    {       /*k:=0;*/

        if (lock_code!='')
        {
            for( i = 1; i < s.length())
            {
                lock_pos++;
                if (lock_pos>lock_code.length())
                {
                    lock_pos = 1;
                }

                if ((i>=0 && i<=31)||(i>=128 && i<=255))
                {
                    s[i] = ' ';
                }

                this_dat = i && 15;
                s[i] = ((i ^ lock_pos ^ lock_dat) + 1);
                lock_dat = (char) (this_dat);
            }
        }

        return s;
    }

    string prepare(string s,string s1)
    {
        int i,j,k,l;
        string s2;
        /*--remove comments--*/
        if (length(s1) == 0) || (s1[1] == ';')
        {
            s1 = "";
        }
        else
        {
            k = 0;
            for (i = s1.length(); i < 1; i--)
            {
                if (s1[i] == ';')
                {
                    k = i;
                }
                if (k>0)
                {
                    s1 = lstr(s1,k-1);      //check what this is
                }
            }
        }
        /*--remove excess spaces--*/
        s2 = '';

        for( i = 1; s1.length(); i++)
        {
            //CHECK WHAT #8 - 10 WAS SET TO IN ORIGIONAL CODE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            if ( (s1[i] != ' ') || (s1[i] != #8) || (s1[i] != #9) || (s1[i] != #10) || (s1[i] != ','))
            {
                s2 = s2 + s1[i];
            }

            else if (s2 != "")
            {
                s = s + s2 + ' ';
                s2 = "";
            }

        }
        if ( s2 != "" )
        {
            s = s + s2;
        }
        return s;
    }

    void write_line(string s,string s1)
    {
        s = prepare(s,s1);

        /*{--write line!--}*/
        if length(s)>0 then
        {
            s = encode(s);
            cout<<f2<<s<<endl;
        }
    }
   //The rest is to be coded it is unknown as to what the code is for
   //I speculate it is a main function
   /*begin
 randomize;
 lock_pos:=0;
 lock_dat:=0;
 if (paramcount<1) or (paramcount>2) then
  begin
   writeln('Usage: ATRLOCK <robot[.at2]> [locked[.atl]]');
   halt;
  end;
 fn1:=btrim(ucase(paramstr(1)));
 if fn1=base_name(fn1) then fn1:=fn1+'.AT2';
 if not exist(fn1) then
  begin writeln('Robot "',fn1,'" not found!'); halt; end;
 if paramcount=2 then
    fn2:=btrim(ucase(paramstr(2))) else fn2:=base_name(fn1)+'.ATL';
 if fn2=base_name(fn2) then fn2:=fn2+'.ATL';
 if not valid(fn2) then
  begin writeln('Output name "',fn1,'" not valid!'); halt; end;
 if fn1=fn2 then
  begin writeln('Filenames can not be the same!'); halt; end;
 assign(f1,fn1); reset(f1);
 assign(f2,fn2); rewrite(f2);

 {--copy comment header--}
 writeln(f2,';------------------------------------------------------------------------------');
 s:='';
 while (not eof(f1)) and (s='') do
  begin
   readln(f1,s);
   s:=btrim(s);
   if s[1]=';' then begin writeln(f2,s); s:=''; end;
  end;

 {--lock header--}
 writeln(f2,';------------------------------------------------------------------------------');
 writeln(f2,'; ',no_path(base_name(fn1)),' Locked on ',date);
 writeln(f2,';------------------------------------------------------------------------------');
 lock_code:='';
 k:=random(21)+20;
 for i:=1 to k do
  lock_code:=lock_code+char(random(32)+65);
 writeln(f2,'#LOCK',locktype,' ',lock_code);

 {--decode lock-code--}
 for i:=1 to length(lock_code) do
  lock_code[i]:=char(ord(lock_code[i])-65);

 write('Encoding "',fn1,'"...');

 {--encode robot--}
 s:=btrim(s);
 if length(s)>0 then
  write_line('',ucase(s));
 while not eof(f1) do
  begin
   {--read line!--}
   readln(f1,s1); s:='';
   s1:=btrim(ucase(s1));

   {--write line!--}
   write_line(s,s1);
  end;
 writeln('Done. Used LOCK Format #',locktype,'.');
 writeln('Only ATR2 v2.08 or later can decode.');
 writeln('LOCKed robot saved as "',fn2,'"');

 close(f1); close(f2);
end.*/
    int main(void)                          //**
    {                                       //**
        encode(fn1);                        //**
        prepare(fn1,fn2);                   //**
        write_line(fn1,fn2);                //**
        return 0;                           //**
    }
