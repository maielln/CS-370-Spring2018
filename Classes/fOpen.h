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
        fOpen();
        string getBuffer ();
        void read_file_to_buffer(FILE *f);
        void cleanup();
        string getLine (int);
        void writeLine (string, FILE *f);


    private:
        char * inPath;
        char * buffer;
        FILE * roboFile;
        bool isValidExt(char directory[]);
        int findLine (int);
        string ucase(string s);
};

#endif
