#include <iostream>
#include <string>
#include <algorithm>
#include "Format.h"
/*
    This is the formatter.
    It checks every character in a string.
    Successfully removes all white space
    This file formats code to be worked with in the switch case
    Receives file from fOpen
*/

using namespace std;

string Format::format(string SSPath)
{
    fOpen inFile(SSPath);
    fireCount = 0;
    scanning = false;
    scanarc = 1; //some variables for robot

    fText = inFile.getBuffer();

    formatFile();

    return fText; //returning the address of position 0
}

void Format::formatFile()
{
    string Line = "";
    string clearFile = "";
    int i = 0;

    while(fText[i] != '\0') //going through whole file
    {
        if(fText[i] != '\r' && fText[i + 1] != '\n')//if not empty line
        {
            if(fText[i] == ';') //checking for comments
            {
                while(fText[i] != '\r' && fText[i + 1] != '\n') //ignores comment only lines
                    i++;
            }
            else
            {
                while(fText[i] != '\r' && fText[i + 1] != '\n') //gets whole line
                {
                    if(fText[i] != ';') //ignoring comments after lines of code
                    {
                        Line += fText[i];
                        i++;
                    }
                    else
                    {
                        while(fText[i] != '\r' && fText[i + 1] != '\n') //goes to end of line if a comment is found
                            i++;
                    }

                }
                Line = trim(Line);
                if(Line[0] >= 33 && Line[0] <= 126) //if what was trimmed is an accurate ascii value, then continue with filling clearFile
                {
                    Line = formatLine(replaceSpace(Line));//completely removes all useless white space
                    clearFile += Line + '\n';
                }
            }
            i++;
            Line = ""; //resetting line
        }
        else
            fText.erase(i, 1); //deletes lines that are empty
        i++;
    }

    fText = clearFile;

}

string Format::trim(string l) //trimming front and back
{
    int i = 0; //keep track of position

    while(!l.empty() && std::isspace(l.back())) //getting rid of back first
        l.pop_back();

    while(i < l.size() && std::isspace(l[i])) //taking care of front now
        i++;

    return l.substr(i);
}

string Format::replaceSpace(string l) //puts commas in place of lone characters, using later for switch case
{
    int i = 1; //pos 0 is confirmed not white space
    while(i < l.size())
    {
        if((l[i] >= 33 && l[i] <= 126) && (l[i] != 44 || l[i] != ',')) //checks ascii table to make sure the right things (and not commas) are getting a comma behind them
        {
            i++;//character found, so if next character is white space, the next character will be changed
            if(l[i] == ' ' || l[i] == 32 || l[i] == 9 || l[i] == '\t')
            {
                l[i] = ','; //next character dealt with
                i++;//inc i to check for more white space
                while(l[i] == ' ') //skipping the rest of the white space
                    i++;
            }
        }
        else
            i++;
    }
    return l;
}

string Comp::formatLine(string l) //erases ALL forms of white space
{
    l.erase(std::remove_if(l.begin(), l.end(), ::isspace), l.end()); //removes remaining white space
    return l;
}
