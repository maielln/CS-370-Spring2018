/*File Name: ATRLOCK.cpp
  Contributer(s): Will Csont
  Date: 1/31/18
  Language: C++ (gcc compiler)
  Version: 1.01
  Description: Currently unfinished and need to check a bunch of notes. Doesn't compile.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ctime>
#include <iostream>
using namespace std;

//uses functions from the stuff and filelib files. look into later

#define locktype 3


string fn1,fn2,s,s1,s2,lock_code;

int i,j,k,lock_pos,lock_dat;
bool this_dat;

//missing f1 and f2 of type text need to look into that later


    string ucase(string s)
    {
        for(int i = 0; i < s.length(); i++)
        {
            if(s[i] >= 97 && s[i] <= 122)
            {
                s[i] = s[i] - 32;
            }
        }
        return s;
    }

    string lstr(string s1,int l)
    {
        if (s1.length()<=l)
        {
            return s1;
        }
        else
        {
            return s1.substr(0,l);
        }
    }

    string ltrim(string s1 )
    {
        int i;
        while ((s1.length() > 0) && ((s1[0] == ' ' ) || (s1[0] == 8) || (s1[0] == 9)))
        {
            s1 = s1.substr(0,s1.length()-1);
        }

        return s1;
    }


    string rtrim(string s1)
    {
        while ((s1.length() > 0) && ((s1.substr(s1.length()-1,1) == " " ) || (s1[s1.length()-1] == 8)
            || (s1[s1.length()-1] == 9)))
        {
            s1 = s1.substr(0,s1.length()-1);
        }

        return s1;
    }


    string btrim(string s1)
    {
        return ltrim(rtrim(s1));
    }

    string base_name(string name)
    {
        int k;
        string s1,s2;
        s1 = "";
        s2 = "";
        k = 1;
        while (k <= name.length() && (name[k] != '.'))
        {
            s1 = s1+name[k];
            k++;
        }
        return s1;
    }


    string encode(string s)
    {
        /*k:=0;*/

        if (lock_code!="")
        {
            for( i = 1; i < s.length(); i++)
            {
cout<<s<<endl;
                lock_pos++;
                if (lock_pos>lock_code.length())
                {
                    lock_pos = 1;
                }

                if ((s[i]>=0 && s[i]<=31)||(s[i]>=128 && s[i]<=255))
                {
                    s[i] = ' ';
                }

                this_dat = i && 15;
                s[i] = ((s[i] ^ lock_code[lock_pos] ^ lock_dat) + 1);
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


        if ((s1.length() == 0) || (s1[1] == ';'))
        {
            s1 = "";
        }
        else
        {
            k = 0;
            for (i = s1.length(); i > 1; i--)
            {
                if (s1[i] == ';')
                {
                    k = i + 1;
                }
                if (k>0)
                {
                    s1 = lstr(s1,k-1);      //check what this is
                }
            }
        }
        /*--remove excess spaces--*/
        s2 = "";

        for( i = 1; i < s1.length(); i++)
        {
            if ( (s1[i] != ' ') || (s1[i] != 8) || (s1[i] != 9) || (s1[i] != 10) || (s1[i] != ','))
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
        if (s.length() > 0)
        {
            s = encode(s);
//            cout<<f2<<s<<endl;
        }
    }

    int main(void)
    {
        //randomize();        //find out where this function is declared
/*        lock_pos = 0;
        lock_dat = 0;


        //replace with file system code

        fn1 = //sets the file

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
            return 1;
        }

        //assign(f1,fn1); reset(f1);      //figure out: the following statement opens the file f1 for reading
        //assign(f2,fn2); rewrite(f2);    //figure out: the following statement opens the file f2 for writing
*/
        /*--copy comment header--*/
/*        cout<<f2<<";------------------------------------------------------------------------------"<<endl;
        s = "";
        while ((!eof(f1)) && (s==""))    //figure out: how to get this statement in C++
        {
            //readln(f1,s);              //figure out: the following statement a line from the file f1
            s = btrim(s);                //find where btrim is (remove the spaces at the end)
            if (s[1] == ';')
            {
                cout<<f2<<s<<endl;
                s = "";
            }
        }
*/


        /*--lock header--*/
/*        cout<<f2<<";------------------------------------------------------------------------------"<<endl;
        cout<<f2,"; "<<no_path(base_name(fn1))<<" Locked on "<<ctime(time(0))<<endl;
        cout<<f2,";------------------------------------------------------------------------------"<<endl;
        lock_code = "";
        k = rand()%21 + 20;
        for (i = 1; i < k; i++)
        {
            lock_code = lock_code + (char)(rand()%32 + 65);
        }

        cout<<f2<<"#LOCK"<<locktype<<" "<<lock_code<<endl;
*/
        /*--decode lock-code--*/
/*        for (i = 1; i < lock_code.length(); i++)
        {
            lock_code[i] = (char)(lock_code[i]-65);
        }


        cout<<"Encoding '"<<fn1<<"'...";
*/
        /*--encode robot--*/
/*        s = btrim(s);           //to reiterate look into this function
        if (s.length()>0)
        {
            write_line("",ucase(s));    //check where the uppercase function is
        }

        while( not eof(f1))         //figure out: how to get this statement in C++
        {
*/
            /*--read line!--*/
            //readln(f1,s1);          //figure out: the following statement reads a line from the file f1
 /*           s = "";
            s1 = btrim(ucase(s1));  //again check where these functions are
*/
            /*--write line!--*/
/*            write_line(s,s1);
        }
        cout<<"Done. Used LOCK Format #"<<locktype<<"."<<endl;
        cout<<"Only ATR2 v2.08 or later can decode."<<endl;
        cout<<"LOCKed robot saved as '"<<fn2<<"'"<<endl;

        close(f1);
        close(f2);
*/
        //encode(fn1);
       // prepare(fn1,fn2);
        //write_line(fn1,fn2);

        cout<<ucase("thIs will BE ees Test 32@%^%$34256425^$%^$*$")<<endl;
        cout<<lstr(" I don't know ", 7)<<endl;
        lock_code = lock_code + (char)(rand()%32 + 65);
        cout<<encode("Could I get this to work at all?")<<endl;
        return 0;
    }
