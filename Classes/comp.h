#ifndef COMP_H
#define COMP_H
#include "fOpen.h"

using namespace std;

class comp
{
    public:
        comp(char*); //passed a file path. Opens the file using fOpen, and sets variables. Calls formatF()
        string getCommand(); //returns the next command. Calls getLine(). The two could be combined, but that's just how I wrote it
        void setLine(int); //sets fLine

    private:
        void formatF(); //removes comments and tabs, formats newline feeds, removes blank lines. Outputs in fText
        string getLine(); //calls findLine(). Returns a string of the line, or "" if the line doesn't exist
        void fShift(int); //used in formatF() to remove unwanted characters. Leaves a lot of blank space at the bottom, and can be cleaned in the future.
        int findLine (); //finds line fLine. returns it's position in the string, or -2 if the line doesn't exist.

        string fText;
        int fLine;
};

#endif
