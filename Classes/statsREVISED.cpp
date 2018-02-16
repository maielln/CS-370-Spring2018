#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include "stats.h"
using namespace std;

const int MAX_ROBOTS = 5;

int main (void) {
	//all of the following is for testing purposes only
	Stats stats;
	int r[MAX_ROBOTS], a[MAX_ROBOTS];

	for (int cnt = 0; cnt < MAX_ROBOTS; cnt++) {
		a[cnt] = rand() % 5;
	}


	//initialize
	stats.init();

	stats.shotCnt(10, 4);
	stats.shotCnt(5, 3);
	stats.killCnt(1);


	//check wins and losses
	for (int cnt = 0; cnt < MAX_ROBOTS; cnt++) {
		stats.winLossChk(a[cnt], cnt);
	}

	cout << endl;

	for (int cnt = 0; cnt < MAX_ROBOTS; cnt++) {
        cout << a[cnt] << endl;


	}



	//display header
	stats.displayHead();
	//display each line
	for (int cnt = 0; cnt < MAX_ROBOTS; cnt++) {
		stats.displayLine(cnt);
	}







	return 0;
}
