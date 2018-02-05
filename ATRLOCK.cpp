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
string lstr(string s1,int l);
string ltrim(string s1 );
string rtrim(string s1);
string btrim(string s1);
string base_name(string name);
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
    char outPath[80]; //"C:/Users/MCYet/Desktop/School/Y2S2/CS 370/fOpen/fOut.txt"
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

    cout << "Please enter the path of the output file: ";
    cin >> outPath;

	outFile = fopen(outPath, "wb");
	if (outFile == NULL)
    {
        cout << "Error opening output file at " << outPath;
        exit(EXIT_FAILURE);
    }

	read_file_to_buffer(roboFile);
	fclose (roboFile);

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

void cleanup()
{

	return;
}

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

void writeLine (string line, FILE *f)
{
    const char * cLine = line.c_str();

    fputs(cLine, f);
    fputs("\r\n", f);

    return;
}


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


    string lstr(string s1,int l)
    {
        if (s1.length()<=l)
        {
            return s1;
        }
        else
        {
            return s1.substr(0,l);
        }
    }


    string ltrim(string s1 )
    {
        int i;
        while ((s1.length() > 0) && ((s1[0] == ' ' ) || (s1[0] == 8) || (s1[0] == 9)))
        {
            s1 = s1.substr(0,s1.length()-1);
        }

        return s1;
    }


string rtrim(string s1)
    {
        while ((s1.length() > 0) && ((s1.substr(s1.length()-1,1) == " " ) || (s1[s1.length()-1] == 8)
            || (s1[s1.length()-1] == 9)))
        {
            s1 = s1.substr(0,s1.length()-1);
        }

        return s1;
    }


    string btrim(string s1)
    {
        return ltrim(rtrim(s1));
    }

    string base_name(string name)
    {
        int k;
        string s1,s2;
        s1 = "";
        s2 = "";
        k = 1;
        while (k <= name.length() && (name[k] != '.'))
        {
            s1 = s1+name[k];
            k++;
        }
        return s1;
    }


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
