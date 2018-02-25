#ifndef FOPEN_H
#define FOPEN_H
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <cstdlib>

using namespace std;

class fOpen
{
    public:
        fOpen(char[]);
        string getBuffer ();
        void read_file_to_buffer(FILE *f);
        void cleanup();
        string getLine (int);
        void writeLine (string, FILE *f);


    private:
        char inPath[80];
        char outPath[80];
        char * buffer;
        FILE * roboFile;
        bool isValidExt(char directory[]);
        int findLine (int);
        string ucase(string s);
};

#endif
