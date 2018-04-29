#include "Includer.h"

using namespace std;

string getDirect()
{
    char* direct;

    direct = getenv("ATSD");

    if(direct == NULL)
    {
        cout<<"An error ocurred. NULL directory."<<endl<<endl<<endl;


        cout<<"************************************************************************************"<<endl<<endl;

        cout<<"Please check to be sure that there was an environment variable set to the name ATSD."<<endl;
        cout<<"Also make sure that it is assigned properly as well. ie. If the folder is in the desktop"<<endl;
        cout<<"it may look something like this C:/Users/Public/Desktop."<<endl<<endl;

        cout<<"If you are running this on Windows go to the search bar and search environment "<<endl;
        cout<<"variables and select edit environment variables, then choose environment variables "<<endl;
        cout<<"and check if the variable ATSD is properly set."<<endl<<endl<<endl<<endl;



        cout<<"Please enter any character to exit the program: ";
        string placeholder;
        cin>>placeholder;

        exit(1);
    }

    string directs = direct;

    for(int i=0; i<(int)(directs.length());i++)
    {
        if(directs[i]=='\\')
        {
            directs[i] = '/';
        }
    }

    directs += "/assets/";

    return directs;
}
