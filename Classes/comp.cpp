#include "comp.h"

using namespace std;

comp::comp(char fPath[])
{
    fLine = 0;

    fOpen inFile(fPath);
    fText = inFile.getBuffer();

    formatF();
}

void comp::formatF(void)
{
    int i=0, length = fText.length();

    for (i=0;i<length;i++) //removes comments
    {
        if (fText[i] == ';')
        {
            while (fText[i] != '\r' && fText[i] != '\n' && fText[i] != '\0')
                {
                    fShift(i);
                }
        }
    }

    for (i=0;i<length;i++) //format blank space
    {
        if (fText[i] == '\t' || fText[i] == 9)
            fShift(i);
        else if (fText[i] == '\r')
            fText[i] = '\n';
        else if (fText[i] == ' ' && fText[i+1] == ' ')
            while (fText[i+1] == ' ')
                fShift(i);
        if (fText[i] == ' ' && fText[i-1] == '\n')
        {
            fShift(i);
            i--;
        }
    }

    for (i=0;i<length;i++) //removes most empty lines
    {
        if (fText[i] == '\n' && (fText[i+1] == '\n' || fText[i+1] == ' ' || fText[i+2] == '\n'))
            while (fText[i+1] == '\n' || fText[i+1] == ' ')
                fShift(i);
    }

    return;
}

void comp::fShift(int i)
{
    int length;

    for (length = fText.length();i<length;i++)
    {
        if (fText[i] == '\0')
            break;
        else
            fText[i] = fText[i+1];
    }

    return;
}


int comp::findLine(void)
{
    int cLine = 0, pos = 0;

    for(pos=0;cLine<fLine;pos++)
    {
        if (fText[pos] == '\n')
            cLine++;
        else if (fText[pos] == '\0')
            return -2;
    }

    return pos;
}


string comp::getLine(void)
{
    string ans;
    int i = findLine();

    if (fLine >= 0)
    {
        if (fText[i] == '\n')
            i++;

        for (ans="";fText[i]!='\n';i++)
        {
            ans+=fText[i];
        }
    }
    else
        return "";

    return ans;
}


string comp::getCommand(void)
{
    fLine++;
    cout << "Line " << fLine << endl;
    string line;
    line = getLine();

    if (line == "")
    {
        fLine = 0;
        return getCommand();
    }
    else
    {
        cout << line << endl;
        return line;
    }
}

void comp::setLine(int lNum)
{
    fLine = lNum;

    cout << "Set line to " << fLine << endl;

    return;
}
