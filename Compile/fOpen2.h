#ifndef FOPEN_H
#define FOPEN_H
#include <iostream>
#include <fstream>


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

        fOpen(string); //Constructor Opens up the windows prompt to take in a directory of a file and puts it in the inPath variable. If the directory isn't valid, then the program exits.
        string getBuffer (); //returns the variable buffer
        string getInPath (); //returns the variable inPath
        void writeLine (string); //Writes a line to the output file. If no file exists, it creates one
        int getLineCount (); //Returns how many lines are in the file


    private:

        char * buffer;
        FILE * roboFile;
        string inPathStr;
        string fText;
        int line_count;

        bool isValidExt(char[]);
        bool read_file_to_buffer(FILE *f);
        int findLine (int);
        string ucase(string);
};

#endif

