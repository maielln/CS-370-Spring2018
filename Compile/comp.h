#ifndef COMP_H
#define COMP_H
#include <fOpen.h>
//Contributer(s): Mark Yetter & Nicole Maiello
//successfully formats code to be read correctly (ex. opo,17)
using namespace std;

class Comp
{
public:
    int scanner, armor, engines, mines, heatsinks, weapon, shield;

    Comp(string); //Constructor replaced compile()
    //string compile(string); //Formats the contents of the file and assigns configuration points. Replaced by constructor and getText()
    string getText(void); //replaces the return of compile()
    string trim(string); //can be useful in other files
    void dispConfig (void); //Displays the configuration values for debugging

private:
    void formatFile(void);
    string formatLine(string);
    string replaceSpace(string);
    void setConfig(void);
    void setVars(int *, char);

    string fText;
};
#endif
