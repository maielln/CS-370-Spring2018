#include "fOpen.h"
//constructor

using namespace std;


//No Argument constructor (Will remove in future updates)
fOpen::fOpen()
{
    char inPath[80] = "C:/Users/wildo/Desktop/AT_Robots/atrobots/atrobots/CIRCLES.AT2";
    FILE * roboFile = NULL;
    FILE * outFile = NULL;
    roboFile = fopen(inPath, "rb");
	if (roboFile == NULL)
	{
        cout << "Could not find input file at " << inPath;
        exit(EXIT_FAILURE);
	}

	read_file_to_buffer(roboFile);
	fclose (roboFile);
}

//Constructor that takes in a character array of a valid Directory in the computer
//otherwise it throws an error.
fOpen::fOpen(char inDirectory[])
{
    int i = 0;
    while(inDirectory[i]!='\0')
    {
        inPath[i] = inDirectory[i];
        i++;
    }
    inPath[i] = '\0';

    FILE * roboFile = NULL;

    if(!isValidExt(inDirectory))
    {
        cout << "ERROR Non-Valid Extension " << inPath;
        exit(EXIT_FAILURE);
    }

    roboFile = fopen(inPath, "rb");
    cout<<inPath<<endl;
	if (roboFile == NULL)
	{
        cout << "Could not find input file at " << inPath;
        exit(EXIT_FAILURE);
	}

    buffer = NULL;
	read_file_to_buffer(roboFile);
	fclose (roboFile);
}

//The given file will read it into a buffer which is a string
void fOpen::read_file_to_buffer(FILE *f)
{
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

//does nothing
void fOpen::cleanup()
{
    return;
}

//finds the position of the line
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

//gets the string representing the line that is currently being selected
string fOpen::getLine (int lNum)
{
    int index = 0, position = 0;
    string line = "";

    position = findLine(lNum);
    if (position < 0)
        return "";

    for (index=0; index<1; position++)
    {
        if (buffer[position]=='\r' || buffer[position]=='\0')
        {
            line += buffer[position];
            index++;
        }
        else
            line += buffer[position];
    }

    return line;
}

//writes a line from the file
void fOpen::writeLine (string line, FILE *f)
{
    const char * cLine = line.c_str();

    fputs(cLine, f);
    fputs("\r\n", f);

    return;
}

//Checks if the given array of characters is a valid extension
bool fOpen::isValidExt(char directory[])
{
    int directSize = 0,i = 0;
    while(directory[i]!='\0')
    {
        directSize++;
        i++;
        if(i>=999999999999)
        {
            cout<<"ERROR TRYING TO READ DIRECTORY EXTENSION "<<endl;
            exit(EXIT_FAILURE);
        }
    }

    string ext = "";

    i--;

    while(directory[i]!='.'&&i>=0)
    {
        ext += directory[i];
        i--;
    }
    if(ext.length()!=3)
    {
        return false;
    }
    else
    {
        char temp;
        temp = ext[0];
        ext[0] = ext[2];
        ext[2] = temp;
    }
    return ext == "AT2" ||ext == "txt";
}

//returns buffer
string fOpen::getBuffer ()
{
    return buffer;
}
