#ifndef COMP_H
#define COMP_H
#include "fOpen.h"
//Contributer(s): Mark Yetter & Nicole Maiello
//successfully formats code to be read correctly (ex. opo,17)
using namespace std;

class Comp
{
public:
    int fireCount;
    bool scanning;
    int scanarc;

    string compile(string);
    string trim(string); //can be useful in other files

private:
    void formatFile();
    string formatLine(string);
    string replaceSpace(string);
    string fText;
};
#endif // COMP
