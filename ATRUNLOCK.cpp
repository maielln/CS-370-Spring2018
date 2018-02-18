#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define MAX_BIT 8

using namespace std;

void read_file_to_buffer(FILE *);
char *buffer = NULL;
void cleanup();
int findLine (int);
string getLine (int);
void writeLine (string, FILE *);
string ucase(string s);
string decode(string s);
char decrypt(char key,char crypted);
int bitSTringToInt(int* bitS);
int* decrypt(int* key,int* crypted);
int* getBitString(char c);

string fn1,fn2,s,s1,s2,lock_code;

int i,j,k,lock_pos,lock_dat;

int* bitString;

bool this_dat;

int main (void)
{
    char inPath[80] = "C:/Users/wildo/Desktop/output.txt";

    char justAnIfStatement = 'J';
    FILE * roboFile = NULL;
    FILE * outFile = NULL;
    int o = 0, i = 0;

    atexit(cleanup);

    cout<<"Please enter the directory of the file encrypted by ATRLOCK: ";
    cin>>inPath;

    for (i=0;i<80;i++)
    {
        if (inPath[i] == 92)
            inPath[i] = '/';
    }

	roboFile = fopen(inPath, "rb");
	if (roboFile == NULL)
	{
        cout << "Could not find input file at " << inPath;
        exit(EXIT_FAILURE);
	}

    read_file_to_buffer(roboFile);
	fclose (roboFile);

	while (justAnIfStatement != 'y' && justAnIfStatement != 'Y' && justAnIfStatement != 'N' && justAnIfStatement != 'n')
    {

        cout << "Would you like to output in the same file? All existing data will be overwritten (Y/N): ";
        cin >> justAnIfStatement;

        if (justAnIfStatement == 'y' || justAnIfStatement == 'Y')
        {
            outFile = fopen(inPath, "wb");
        }
        else if (justAnIfStatement == 'n' || justAnIfStatement == 'N')
        {
            char inPath[80] = "C:/Users/wildo/Desktop/output.txt";
            cout<<"Please enter the directory of the file you want the output to go to: ";
            cin>>inPath;

            for (i=0;i<80;i++)
            {
                if (inPath[i] == 92)
                    inPath[i] = '/';
            }

            outFile = fopen(inPath, "wb");
        }
        else
        {
            cout << "Invalid input. Please enter 'Y' for yes or 'N' for no" << endl;
        }
    }

	if (outFile == NULL)
    {
        cout << "Error opening output file at " << inPath;
        exit(EXIT_FAILURE);
    }

    cout << endl;

    s = getLine(0);
    o = s.length();
    lock_code = s[0];
    s = getLine(1);
    for (i=1;s!="";i++)
    {
        s = ucase(s);
        s = decode(s);
        if (s[1] != '\0'&& s [0] !=13 && s [0] != 9)
            writeLine (s, outFile);
        s = getLine(i);
    }

    fclose (outFile);
	free(buffer);
	buffer = NULL;

	cout << "File decrypted! Used lock_code :"<<lock_code<< endl;
	cout << "Press any character and enter to exit." << endl;
	cin >> justAnIfStatement;

    exit(EXIT_SUCCESS);
}

//checks the validity of a file, and reads it to a buffer. Since the buffer is a global variable, the function returns nothing.
void read_file_to_buffer(FILE *f) {
	long file_size = 0;

	if(buffer != NULL) {
		fprintf(stderr, "Buffer in use\n");
		exit(EXIT_FAILURE);
	}

	rewind(f);
	if(fseek(f, 0, SEEK_END) != 0) {
		perror("Couldn't seek to end of file");
		exit(EXIT_FAILURE);
	}

	file_size = ftell(f);
	if(file_size < 0) {
		perror("Couldn't tell size");
		exit(EXIT_FAILURE);
	}
	rewind(f);

	buffer = (char *)malloc(sizeof(char) * (file_size + 1));
	if(buffer == NULL) {
		fprintf(stderr, "Could not allocate buffer\n");
		exit(EXIT_FAILURE);
	}

	if(fread(buffer, sizeof(char), (size_t)file_size, f) != file_size) {
		fprintf(stderr, "Couldn't read file\n");
		exit(EXIT_FAILURE);
	}
	buffer[file_size] = '\0';

	return;
}

//Appears to clean stuff up. Was a part of Confer's original code, so I ain't touching shit
void cleanup()
{
	return;
}

//Passed a line number in a file (starting at 0), then returns the location of the start of the line in the buffer
int findLine (int lNum)
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
        else if (buffer[position] == '\r')
        {
            index++;
        }
    }

    return position+1;
}

//passed a line number in a file (starting at 0), and returns a string containing that line.
string getLine (int lNum)
{
    int index = 0, position = 0;
    string line = "";

    position = findLine(lNum);
    if (position < 0)
    {
        return "";
    }

    for (index=0; index<1; position++)
    {
        if (buffer[position]=='\r' || buffer[position]=='\0')
        {
            line += buffer[position];
            index++;
        }

        else if(buffer[position] == ';')
        {
            line += '\r';
            index++;
        }
        else if(buffer[position]!=' ' && buffer[position]!=9)
        {
            line += buffer[position];
        }
    }
    return line;
}

//Casts a string to a constant char *, outputs it to a file, and ends the line. Made for windows.
void writeLine (string line, FILE *f)
{
    const char * cLine = line.c_str();

    fputs(cLine, f);
    fputs("\r\n", f);

    return;
}

//Turns all characters in a string into uppercase letters
string ucase(string s)
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
string decode(string s)
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

            if(s[i]!='\r'||s[i]!='\0')
            {
                s[i] = decrypt(lock_code[lock_pos],s[i]) - 1;
            }

            lock_dat = (char)this_dat;
        }
    }
    return s;
}

int* getBitString(char c)
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

int* decrypt(int* key,int* crypted)
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

int bitSTringToInt(int* bitS)
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

char decrypt(char key,char crypted)
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
