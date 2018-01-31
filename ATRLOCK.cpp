/*File Name: ATRLOCK.cpp
  Contributer(s): Will Csont
  Date: 1/31/18
  Language: C++ (gcc compiler)
  Version: 1.01
  Description: Currently unfinished and need to check a bunch of notes.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ctime>

using namespace std;

//uses functions from the stuff and filelib files. look into later

#define locktype 3


string fn1,fn2,s,s1,s2,lock_code;

int i,j,k,lock_pos,lock_dat;
bool this_dat;

//missing f1 and f2 of type text need to look into that later



    string encode(string s)
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

    int main(String[] argv, int argc)
    {
        randomize();        //find out where this function is declared
        lock_pos = 0;
        lock_dat = 0;
        if ((argc<1) || (argc>2))
        {
            cout<<"Usage: ATRLOCK <robot[.at2]> [locked[.atl]]"<<endl;
            return 1;         //check if works if not replace other returns with an exit command
        }

        fn1 = btrim(ucase(paramstr(1)));    //check where that is from and make ucase method if neccessary

        if (fn1 == base_name(fn1))
        {
            fn1 = fn1 + ".AT2";
        }
        if (!exist(fn1))
        {
            cout<<"Robot "<<fn1<<" not found!"<<endl;
            return 1;                                 //return
        }
        if (paramcount == 2)
        {
            fn2 = btrim(ucase(paramstr(2)));
        }
        else
        {
            fn2 = base_name(fn1) + ".ATL";
        }
        if (fn2 == base_name(fn2))
        {
            fn2 = fn2 + ".ATL";
        }
        if (!valid(fn2))                //find where this is from
        {
            cout<<"Output name '"<<fn1<<"' not valid!"<<endl;
            return 1;
        }

        if (fn1 == fn2)
        {
            cout<<"Filenames can not be the same!"<<endl;
            return;
        }

        //assign(f1,fn1); reset(f1);      //figure out: the following statement opens the file f1 for reading
        //assign(f2,fn2); rewrite(f2);    //figure out: the following statement opens the file f2 for writing

        /*--copy comment header--*/
        cout<<f2,";------------------------------------------------------------------------------"<<endl;
        s = "";
        while ((!eof(f1)) and (s=''))    //figure out: how to get this statement in C++
        {
            //readln(f1,s);              //figure out: the following statement a line from the file f1
            s = btrim(s);                //find where btrim is
            if (s[1] == ';')
            {
                cout<<f2<<s<<endl;
                s = '';
            }
        }



        /*--lock header--*/
        cout<<f2<<";------------------------------------------------------------------------------"<<endl;
        cout<<f2,"; "<<no_path(base_name(fn1))<<" Locked on "<<ctime(time(0))<<endl;
        cout<<f2,";------------------------------------------------------------------------------"<<endl;
        lock_code = "";
        k = rand()%21 + 20;
        for (i = 1; i < k; i++)
        {
            lock_code = lock_code + (char)(rand()%32 + 65);
        }

        cout<<f2<<"#LOCK"<<locktype<<" "<<lock_code<<endl;

        /*--decode lock-code--*/
        for (i:=1 i < lock_code.length(); i++)
        {
            lock_code[i] = (char)(i-65);
        }


        cout<<"Encoding '"<<fn1<<"'...");

        /*--encode robot--*/
        s = btrim(s);           //to reiterate look into this function
        if (s.length()>0)
        {
            write_line("",ucase(s));    //check where the uppercase function is
        }

        while( not eof(f1))         //figure out: how to get this statement in C++
        {
            /*--read line!--*/
            //readln(f1,s1);          //figure out: the following statement reads a line from the file f1
            s = '';
            s1 = btrim(ucase(s1));  //again check where these functions are

            /*--write line!--*/
            write_line(s,s1);
        }
        cout<<"Done. Used LOCK Format #"<<locktype,"."<<endl;
        cout<<"Only ATR2 v2.08 or later can decode."<<endl;
        cout<<"LOCKed robot saved as '"<<fn2<<"'"<<endl;

        close(f1);
        close(f2);

        /*encode(fn1);        Old tests to be certain it would compile
        prepare(fn1,fn2);
        write_line(fn1,fn2);  */
        return 0;
    }
