#include "fOpen.h"
#include <iostream>

using namespace std;

void display (void);

fOpen * fileArr[32];

/*
file selection program, used to select and store the filepaths of multiple files.
Written by Mark Yetter in Spring 2018
Written in c++ (gcc compiler)
*/

int main (void)
{
	string input = "";
	int i, numRobots = 0;

	while (input != "exit")
	{
		display();

		cout << "Please insert add, edit, delete, continue, or exit: ";
		cin >> input;

		if (input == "add")
		{
			for (i=0;i<32;i++)
			{
				if (fileArr[i] == NULL)
				{
					cout << "Please select a file to add in position " << i+1 << endl;
					fileArr[i] = new fOpen();
					break;
				}
			}

			if (i < 32)
				cout << "File added in position " << i+1 << endl;
			else
				cout << "No open slots to add a file. You have to delete a file to add a new one " << endl;
		}
		else if (input == "edit")
        {
            cout << "Which file position would you like to edit: ";
            cin >> i;
            i--;
            if (i < 32 && fileArr[i] != NULL)
            {
                cout << "Please select a file to replace " << fileArr[i] -> getInPath() << " : ";
                delete fileArr[i];
                fileArr[i] = new fOpen();
                cout << "File successfully replaced!" << endl;
            }
            else if (i < 32)
            {
                cout << "Please select a file to add in position " << i+1 << ": ";
                fileArr[i] = new fOpen ();
                cout << "File successfully added!" << endl;
            }
            else
            {
                cout << "Invalid entry. Please pick a number between 1 and 32" << endl;
            }
        }
		else if (input == "delete")
		{
			cout << "Please insert the position of the file you wish to delete: ";
			cin >> i;
			i--;
			if (fileArr[i] == NULL)
			{
				cout << "There is no file in that position to delete!" << endl;
			}
			else
			{
				cout << "Are you sure you would like to delete " << fileArr[i] -> getInPath() << "? (y/n): ";
				cin >> input;
				if (input == "y")
				{
					delete fileArr[i];
					fileArr[i] = NULL;
					cout << "File " << i+1 << " deleted!" << endl;
				}
				else
				{
					cout << "File " << i+1 << " has not been deleted." << endl;
				}
				input = "delete";
			}
		}
		else if (input == "continue")
		{
			cout << "Are you sure you would like to start with the selected robots? (y/n): ";
			cin >> input;
			if (input == "y")
            {
                for (i=0;i<32;i++)
                {
                    if (fileArr[i] != NULL)
                    {
                        cout << "Loading in " << fileArr[i] -> getInPath() << endl;
                        numRobots++;
                    }
                }
                cout << "Starting the game with these " << numRobots << " robots." << endl;
				return 1;
            }

		}
		else if (input != "exit")
		{
			cout << "Invalid comand: " << input << endl;
		}
	}

	return 0;
}


void display (void)
{
	int i = 0;

	cout << "--------------------------------------------------" << endl;

	for (i=0;i<32;i++)
	{
		cout << i+1 << ": ";

		if (fileArr[i] == NULL)
		{
			cout << "no file selected" << endl;
		}
		else
		{
			cout << fileArr[i] -> getInPath() << endl;
		}
	}

	cout << "--------------------------------------------------" << endl;

	return;
}

