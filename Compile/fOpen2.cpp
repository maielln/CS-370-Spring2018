#include "fOpen.h"

using namespace std;

/*
File opening class
Contributors: Mark Yetter, William Csont
Written in C++ (gcc compiler) in the spring of 2018
Purpose: to allow a program to open and read the contents of a file, as well as print into a different file.
*/

fOpen::fOpen ()
{
    cout << "Please insert the path for a .txt or .at2 file: ";
    cin >> inPath;

    int i=0;
    inPathStr = "i";
    line_count = 0;

    FILE * roboFile = NULL;
    buffer = NULL;

    if (!isValidExt(inPath))
    {
        cout << "Invalid extension. Please use a .txt or .at2 file." << endl;
        inPathStr = "";
    }
    else
    {
        roboFile = fopen(inPath, "rb");
        if (roboFile == NULL)
        {
            cout << "Could not find file at " << inPath << endl;
            inPathStr = "";
        }
        else if (read_file_to_buffer(roboFile))
        {
            fText = buffer;

            fclose(roboFile);
            free(buffer);
            buffer = NULL;
        }
        else
        {
            inPathStr = "";
        }

    }

    if (inPathStr == "i")
        inPathStr = (string)inPath;

    for (i=0;fText[i]!='\0';i++)
    {
        if (fText[i] == '\n')
            line_count++;
    }


    return;
}


bool fOpen::read_file_to_buffer(FILE *f)
{
    long file_size = 0;

	if(buffer != NULL) {
		fprintf(stderr, "ERROR: Buffer in use\n");
		buffer = (char*)"";
		return 0;
	}

	rewind(f);
	if(fseek(f, 0, SEEK_END) != 0) {
		perror("ERROR: Couldn't seek to end of file");
		buffer = (char*)"";
		return 0;
	}

	file_size = ftell(f);
	if(file_size < 0) {
		perror("ERROR: Couldn't tell size");
		buffer = (char*)"";
		return 0;
	}
	rewind(f);

	buffer = (char *)malloc(sizeof(char) * (file_size + 1));
	if(buffer == NULL) {
		fprintf(stderr, "ERROR: Could not allocate buffer\n");
		buffer = (char*)"";
		return 0;
	}

	if((int)(fread(buffer, sizeof(char), (size_t)file_size, f)) != file_size) {
		fprintf(stderr, "ERROR: Couldn't read file\n");
		buffer = (char*)"";
		return 0;
	}
	buffer[file_size] = '\0';

	return 1;
}


int fOpen::findLine (int lNum)
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


bool fOpen::isValidExt(char inPath[])
{
    int i, j;
    char ext[4] = "";

    for (i=0;inPath[i]!='.';i++)
    {
        if (i > 80)
            return 0;
    }

    for(j=0;j<3;j++)
    {
        ext[j] += inPath[i+j+1];
    }

    if((ext[0] == 'a' || ext[0] == 'A') && (ext[1] == 't' || ext[1] == 'T') && (ext[2] == '2'))
        return 1;
    else if ((ext[0] == 't' || ext[0] == 'T') && (ext[1] == 'x' || ext[1] == 'X') && (ext[2] == 't' || ext[2] == 'T'))
        return 1;
    else
        return 0;

}


string fOpen::getBuffer (void)
{
    return fText;
}


string fOpen::getInPath(void)
{
    return inPathStr;
}


int fOpen::getLineCount(void)
{
    return line_count;
}


void fOpen::writeLine(string inString)
{
    string outPath = inPathStr.substr (0, (inPathStr.length()-4));

    outPath += "Output.txt";

    ofstream outFile;
    outFile.open(outPath, ios::out | ios::app | ios::binary);

    outFile << inString;

    outFile.close();
}

