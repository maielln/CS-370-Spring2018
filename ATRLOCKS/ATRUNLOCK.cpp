#include "ATRUNLOCK.h"

/*
Written by Will
Written in C++ (GNU GCC)
Last updated 2/15/18
A class that takes a string encrypted by the ATRLOCK program
and brings it back to the original
*/


//Constructor
ATRUNLOCK::ATRUNLOCK (void)
{

}

//Takes a valid string encrypted by ATRLOOCK otherwise it returns an empty string
string ATRUNLOCK::decipher(string buf)
{
    buffer = buf;

    s = getLine(0);

    if(s.find("This code was encoded by ATRLOCK. DO NOT REMOVE THIS STATEMENT") != 2)
    {
        cout<<"NOT VALID ENCODED FILE RETURNING EMPTY STRING\r"<<endl;
        return "";
    }
    o = s.length();
    lock_code = s[0];
    s = getLine(1);
    string decryption = "";
    for (i=2;s!="";i++)
    {
        s = ucase(s);
        s = decode(s);
        if (s[1] != '\0'&& s [0] !=13 && s [0] != 9 && s [0] !=10)
            decryption += s + "\n";
        s = getLine(i);
    }

    return decryption;
}

//Passed a line number in a file (starting at 0), then returns the location of the start of the line in the buffer
int ATRUNLOCK::findLine (int lNum)
{
    int index = 0, position = 0;

    if (lNum == 0)
        return 0;

    for (position=0; index<lNum; position++)
    {
        if (buffer[position] == '\0')
        {
            return -1;
        }
        else if (buffer[position] == '\r'&& buffer[position + 1] == '\n')
        {
            index++;
        }
    }

    return position+1;
}

//passed a line number in a file (starting at 0), and returns a string containing that line.
string ATRUNLOCK::getLine (int lNum)
{
    int index = 0, position = 0;
    string line = "";

    position = findLine(lNum);
    if (position < 0)
    {
        return "";
    }

    bool first = true;

    if(buffer[position]!=';'&& position != 0)
    {
        header = false;
    }

    for (index=0; index<1; position++)
    {
        if (buffer[position]=='\r' || buffer[position]=='\0')
        {
            line += buffer[position];
            index++;
        }

        else if(buffer[position] == ';' && header)
        {
            line += '\r';
            index++;
        }
        else if(buffer[position]!=9)
        {
           if(buffer[position] == ' ')
           {
               if(buffer[position-1]!=' '&& position != 0 && !first)
                {
                    line += buffer[position];
                }
           }
           else
           {
               line += buffer[position];
           }
           first = false;
        }
    }

    return line;
}

//Turns all characters in a string into uppercase letters
string ATRUNLOCK::ucase(string s)
{
    for(int i = 0; i < s.length(); i++)
    {
        if(s[i] >= 97 && s[i] <= 122)
        {
            s[i] = s[i] - 32;
        }
    }
    return s;
}

//decodes a string, removing white space and randomizing the letters to deter reading
//lock_dat was a failed way for mote encryption
string ATRUNLOCK::decode(string s)
{
    if (lock_code != "")
    {
        for (int i = 0; i < s.length()-1; i++)
        {
            lock_pos++;

            if (lock_pos > lock_code.length())
            {
                lock_pos = 1;
            }

            this_dat = (i && 15);
            if(s[i] == ' ')
            {

            }
            else if(s[i]!='\r'||s[i]!='\0'&& s[i] != ' ')
            {
                s[i] = decrypt(lock_code[lock_pos],s[i]) - 1;
            }



            lock_dat = (char)this_dat;
        }
    }
    return s;
}

//gets a character of ints that represent the bitstring of the character value c
int* ATRUNLOCK::getBitString(char c)
{
    bitString = new int[MAX_BIT];
    int num = (int)(c);
    for(int i = MAX_BIT - 1;i >= 0; i--)
    {
        int factor =(int)(pow(2.0,i));
        if((num != factor)&&(num%factor < num))
        {
            bitString[i] = 1;
            num -= factor;
        }
        else if(num == factor)
        {
            bitString[i] = 1;
            num -= factor;
        }
        else
        {
            bitString[i] = 0;
        }
    }

    return bitString;
}

//performs the opposite of exclusive or with the key and crypted being the
//crypted character from ATRLOCK this one takes two bitstrings
int* ATRUNLOCK::decrypt(int* key,int* crypted)
{
    bitString = new int[MAX_BIT];
    for(int i=0;i < MAX_BIT; i++)
    {
        if(crypted[i] != 0 && crypted[i] != 1)
        {
            cout<<"ERROR INVALID BITSTRING"<<endl;
            exit(-1);
        }
        else if(crypted[i] == 0)
        {
            bitString[i] = key[i];
        }
        else
        {
            if(key[i] == 0)
            {
                bitString[i] = 1;
            }
            else
            {
                bitString[i] = 0;
            }
        }
    }

    return bitString;
}

//calculates the integer equivalent of a given bitstring
int ATRUNLOCK::bitSTringToInt(int* bitS)
{
    int total = 0;
    for(int i = 0;i < MAX_BIT; i++)
    {
        if(bitS[i] != 0 && bitS[i] != 1)
        {
            cout<<"ERROR INVALID BITSTRING"<<endl;
            exit(-1);
        }
        int factor =(int)(pow(2.0,i));
        total += factor * bitS[i];
    }
    return total;
}

//performs the opposite of exclusive or with the key and crypted being the
//crypted character from ATRLOCK this one takes two characters
char ATRUNLOCK::decrypt(char key,char crypted)
{
    int* keyBit = getBitString(key);
    int* cryptBit = getBitString(crypted);
    bitString = new int[MAX_BIT];
    for(int i=0;i < MAX_BIT; i++)
    {
        if(cryptBit[i] != 0 && cryptBit[i] != 1)
        {
            cout<<"ERROR INVALID BITSTRING"<<endl;
            exit(-1);
        }
        else if(cryptBit[i] == 0)
        {
            bitString[i] = keyBit[i];
        }
        else
        {
            if(keyBit[i] == 0)
            {
                bitString[i] = 1;
            }
            else
            {
                bitString[i] = 0;
            }
        }
    }

    char val = (char)(bitSTringToInt(bitString));

    return val;
}
