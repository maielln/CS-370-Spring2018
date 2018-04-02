#ifndef FOPEN_H
#define FOPEN_H
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <windows.h>
#include <Commdlg.h>

using namespace std;

class fOpen
{
    public:
        //Constructor Opens up the windows prompt to take in a directory of a file
        //and puts it in the inPath variable. If the directory isn't valid, then the
        //program exits.
        fOpen();
        //returns the variable buffer
        string getBuffer ();
        //returns the variable inPath
        char* getInPath ();



    private:

        char * inPath;
        char * buffer;
        FILE * roboFile;
        bool isValidExt(char directory[]);
        void read_file_to_buffer(FILE *f);
        void writeLine (string, FILE *f);
        int findLine (int);
        string ucase(string s);
};

#endif
