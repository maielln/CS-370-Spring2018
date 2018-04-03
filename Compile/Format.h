#ifndef FORMAT_H
#define FORMAT_H

//Contributer(s): Mark Yetter & Nicole Maiello
//successfully formats code to be read correctly (ex. opo,17)
using namespace std;

class Format
{
public:
    int fireCount;
    bool scanning;
    int scanarc;

    string format(string);
    string trim(string); //can be useful in other files

private:
    void formatFile();
    string formatLine(string);
    string replaceSpace(string);
    string fText;
};
#endif // FORMAT
