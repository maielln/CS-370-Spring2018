#include <iostream>
#include "Mine.h"
#include <windows.h>
#include <Commdlg.h>


using namespace std;


//TO PROPERLY BUILD THIS YOU MUST HAVE Libcomdlg32.a added in your linker settings.
//Go to Project build options and in your linker settings search for the library and add it
int main()
{
    Mine m;
    Robot r1();
    cout << m.x << endl;

    OPENFILENAME ofn ;

    char szFile[100] ;

    ZeroMemory( &ofn , sizeof( ofn));
	ofn.lStructSize = sizeof ( ofn );
	ofn.hwndOwner = NULL  ;
	ofn.lpstrFile = szFile ;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof( szFile );
	ofn.lpstrFilter = "All\0*.*\0Pascal .PAS\0*.PAS\0";
	ofn.nFilterIndex =1;
	ofn.lpstrFileTitle = NULL ;

	GetOpenFileName( &ofn );
	// Now simpley display the file name

    cout<<ofn.lpstrFile<<endl;
    return 0;
}
