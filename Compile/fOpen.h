#ifndef FOPEN_H
#define FOPEN_H
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <windows.h>
#include <Commdlg.h>

using namespace std;

/*
File opening class
Contributors: Mark Yetter, William Csont
Written in C++ (gcc compiler) in the spring of 2018
Purpose: to allow a program to open and read the contents of a file, as well as print into a different file.
*/

class fOpen //requires libcomdlg32.a
{
    public:

        fOpen(); //Constructor Opens up the windows prompt to take in a directory of a file and puts it in the inPath variable. If the directory isn't valid, then the program exits.
        string getBuffer (); //returns the variable buffer
        string getInPath (); //returns the variable inPath
        void writeLine (string); //Writes a line to the output file. If no file exists, it creates one


    private:

        char * inPath;
        char * buffer;
        FILE * roboFile;
        string inPathStr;

        bool isValidExt(char directory[]);
        void read_file_to_buffer(FILE *f);
        int findLine (int);
        string ucase(string s);
};

#endif
