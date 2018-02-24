#ifndef ATRUNLOCK_H_INCLUDED
#define ATRUNLOCK_H_INCLUDED
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define MAX_BIT 8

using namespace std;

class ATRUNLOCK
{
    public:

        ATRUNLOCK();
        string decipher(string buf);

    private:

        int findLine (int);
        string ucase(string s);
        int* decrypt(int* key,int* crypted);
        int* getBitString(char c);
        int bitSTringToInt(int* bitS);
        char decrypt(char key,char crypted);
        string decode(string s);
        string getLine (int);

        string lock_code, s, buffer;
        int i,j,k,lock_pos,o,lock_dat;
        int* bitString;
        bool this_dat,header = true;
};


#endif // ATRUNLOCK_H_INCLUDED
