#include "fOpen.h"
//constructor

using namespace std;

//Constructor that takes in a character array of a valid Directory in the computer
//otherwise it throws an error.
fOpen::fOpen()
{
    OPENFILENAME ofn ;

    char szFile[100] ;
    buffer = "";

    cout<<"Please select an AT2 or text file."<<endl;
    for(int i=0; i<1000000000;i++)
    {

    }

    ZeroMemory( &ofn , sizeof( ofn));
	ofn.lStructSize = sizeof ( ofn );
	ofn.hwndOwner = NULL  ;
	ofn.lpstrFile = szFile ;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof( szFile );
	ofn.lpstrFilter = "ATR Files\0*.AT2\0Text\0*.TXT\0";
	ofn.nFilterIndex =1;
	ofn.lpstrFileTitle = NULL ;

	GetOpenFileName( &ofn );;

    inPath = ofn.lpstrFile;

    FILE * roboFile = NULL;

    if(!isValidExt(inPath))
    {
        cout << "ERROR: File extension not valid " << inPath << endl << "Please use a .AT2 or .TXT file";
        buffer = "";
        return;
    }

    roboFile = fopen(inPath, "rb");

	if (roboFile == NULL)
	{
        cout << "ERROR: Could not find input file at " << inPath;
        buffer = "";
        return;
	}

    buffer = "";
	read_file_to_buffer(roboFile);
	fclose (roboFile);
}

//The given file will read it into a buffer which is a string
void fOpen::read_file_to_buffer(FILE *f)
{
    long file_size = 0;
    buffer = NULL;

	if(buffer != NULL) {
		fprintf(stderr, "ERROR: Buffer in use\n");
		buffer = "";
		return;
	}

	rewind(f);
	if(fseek(f, 0, SEEK_END) != 0) {
		perror("ERROR: Couldn't seek to end of file");
		buffer = "";
		return;
	}

	file_size = ftell(f);
	if(file_size < 0) {
		perror("ERROR: Couldn't tell size");
		buffer = "";
		return;
	}
	rewind(f);

	buffer = (char *)malloc(sizeof(char) * (file_size + 1));
	if(buffer == NULL) {
		fprintf(stderr, "ERROR: Could not allocate buffer\n");
		buffer = "";
		return;
	}

	if(fread(buffer, sizeof(char), (size_t)file_size, f) != file_size) {
		fprintf(stderr, "ERROR: Couldn't read file\n");
		buffer = "";
		return;
	}
	buffer[file_size] = '\0';

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
        if(i>=80)
        {
            cout<<"Could not read file extension"<<endl;
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

    ext = ucase(ext);

    return ext == "AT2" ||ext == "TXT";
}

string fOpen::ucase(string s)
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

//returns buffer
string fOpen::getBuffer ()
{
    return buffer;
}

char* fOpen::getInPath ()
{
    return inPath;
}
