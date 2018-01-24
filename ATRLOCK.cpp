/*File Name: ATRLOCK.cpp
  Contributer(s): Will Csont
  Date: 1/24/18
  Language: C++ (gcc compiler)
  Version: 1.0
  Description: Currently just a shell with some variables declared and functions declared with no
  code in the function. The current build runs.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

//uses functions from the stuff and filelib files. Look into later.

#define locktype 3


string fn1,fn2,s,s1,s2,lock_code;

int i,j,k,lock_pos,lock_dat,this_dat;

//missing f1 and f2 of type text need to look into that later



    string encode(string s)                 //********THE FOLLOWING ARE ONLY JUST EMPTY FUNCTIONS TO BE CODED LATER******
    {                                       //**
        return s;                           //**
    }                                       //**
                                            //**
    string prepare(string s,string s1)      //**
    {                                       //**
        return s;                           //**
    }                                       //**
                                            //**
    void write_line(string s,string s1)     //**
    {                                       //**
        return;                             //**
    }                                       //**
                                            //**
    int main(void)                          //**
    {                                       //**
        encode(fn1);                        //**
        prepare(fn1,fn2);                   //**
        write_line(fn1,fn2);                //**
        return 0;                           //**
    }
