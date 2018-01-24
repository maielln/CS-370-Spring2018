/*File Name: ATRLOCK.c
  Contributer(s): Will Csont
  Date: 1/24/18
  Language: C (gcc compiler)
  Version: 1.0
  Description: Currently just a shell with some variables declared and functions declared with no
  code in the function. The current build runs.
*/
#include <stdio.h>
#include <stdlib.h>

//uses functions from the stuff and filelib files. look into later

#define locktype 3

char fn1[999999],fn2[999999],s[999999],s1[999999],s2[999999],lock_code[999999];     //placeholder size

int i,j,k,lock_pos,lock_dat,this_dat;

//missing f1 and f2 of type text need to look into that later



    char* encode(char *s)                   //********THE FOLLOWING ARE ONLY JUST EMPTY FUNCTIONS TO BE CODED LATER******
    {                                       //**
        return s;                           //**
    }                                       //**
                                            //**
    char* prepare(char *s,char *s1)         //**
    {                                       //**
        return s;                           //**
    }                                       //**
    //check what a procedure is for this    //**
    void write_line(char *s,char *s1)       //**
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
