#ifndef COMSWITCH_H
#define COMSWITCH_H
#include "comp.h"

using namespace std;

class comSwitch
{
    public:
        comSwitch(char[]); //constructor. Calls init()
        void init(void); //initializes variables inside the robot
        void thisShouldntWork(void);// it doesn't work. Deleted because it doesn't work
        void nextCom(int); //a giant switch case. Gets passed the line number because it wasn't working. Still isn't working

    private:
        string line, com1, com2, com3;
        comp * inCode; //the compiler didn't like be accessing inFile, so i made a pointer to it
        comp inFile(char inDir); //used to be initialized in main, but changed because it wasn't working. Still doesn't work

        int stringToInt(string); //turns a string into an integer value. NOT WRITTEN YET
        void parseCommand(void); //takes a command and splits it up, puts parts into com1, com2, and com3
        void jmp(string); //used when the file wants to jump around lines. NOT WRITTEN YET
        void errMsg(int); //called by nextCom() when defaulting. Displays an error message
        void shR(int, int); //shift int num int bits right. Doesn't rotate bits. NOT WRITTEN YET
        void shL(int, int); //shifts int num int bits left. Doesn't rotate bits. NOT WRITTEN YET
        void roR(int, int); //rotates int num int bits right. Rotates bits. NOT WRITTEN YET
        void roL(int, int); //rotates int num int bits left. Rotates bits. NOT WRITTEN YET
        int toInt (string); //turns a string to an integer for the math stuff. Cleans white space and invalid characters. NOT WRITTEN YET
        void debug (string); //used during creation to debug strange characters and such. Call was deleted after I fucked up so bad it printed out 50 lines of integers all above 4 billion

};

#endif
