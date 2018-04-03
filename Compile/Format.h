#ifndef FORMAT_H
#define FORMAT_H

//Contributer(s): Mark Yetter & Nicole Maiello
//successfully formats code to be read correctly (ex. opo,17)
using namespace std;

class Format
{
public:
    string format(string);
    /*
        This funciton takes in a string, then puts it through the rest of the code
        so it can be sucessfully formatted.
        EX.
           Input:    OPO            8
           Output:OPO,8
    */
    string trim(string);
    /*
        This funciton takes in a string, then removes whitespace from the front
        and back of the text.
        EX.
            Input:        I want some food.         
            Output:I want some food.
    */

private:
    void formatFile();
    string formatLine(string);
    string replaceSpace(string);
    string fText;
};
#endif // FORMAT
