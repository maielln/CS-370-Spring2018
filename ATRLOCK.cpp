#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <cstdlib>

using namespace std;

void read_file_to_buffer(FILE *);
char *buffer = NULL;
void cleanup();
int findLine (int);
string getLine (int);
void writeLine (string, FILE *);
string ucase(string s);
string encode(string s);

/*
Written by Mark Nicole, and Will
Written in C++ (GNU GCC)
Last updated 2/5/18
opens a file, removes blank space, and encrypts it
*/

string fn1,fn2,s,s1,s2,lock_code;

int i,j,k,lock_pos,lock_dat;
bool this_dat;

int main (void)
{
    char inPath[80]; //"C:/Users/MCYet/Desktop/School/Y2S2/CS 370/fOpen/f.txt"
    char justAnIfStatement = 'J';
    FILE * roboFile = NULL;
    FILE * outFile = NULL;
    int o = 0;

    cout << "Please enter the path of the input file: ";
    cin >> inPath;

	atexit(cleanup);

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
            cout << "Please enter the path of the output file: ";
            cin >> inPath;

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
    int i;
    lock_code = lock_code + (char)(rand()%32 + 65);
    for (i=1;s!="";i++)
    {
        s = ucase(s);
        s = encode(s);
        if (s[1] != '\0')
            writeLine (s, outFile);
        s = getLine(i);
    }

    fclose (outFile);
	free(buffer);
	buffer = NULL;

	cout << "File encrypted!";

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
        return "";

    for (index=0; index<1; position++)
    {

        if(buffer[position] == ';')
        {
            line += '\r';
            index++;
        }
        else if (buffer[position]=='\r' || buffer[position]=='\0')
        {
            line += buffer[position];
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

//encodes a string, removing white space and randomizing the letters to deter reading
string encode(string s)
{
    if (lock_code != ""){
        for (int i = 1; i < s.length(); i++)
        {
            lock_pos++;

            if (lock_pos > lock_code.length())
            {
                lock_pos = 1;
            }

            if ((s[i] >= 0 && s[i] <= 31) || (s[i] >= 128 && s[i] <= 255)) {
                s[i] = ' ';
            }

            this_dat = (i && 15);

            if(s[i] != '\0' && s[i] != ' ' && (s[i] != '\r' || s[i+1] != '\n'))
                s[i] = ((s[i] ^ lock_code[lock_pos] ^ lock_dat) + 1);
            else
                s[i] = '\0';
            lock_dat = (char)this_dat;
        }
    }
    return s;
}

