#include "Stats.h"
using namespace std;

void Stats::init()  {
	//Initializes the base stats for each rbt
/*	for (int cnt = 0; cnt < TOTAL_ROBOTS; cnt++) {
		wins[cnt] = 1;
		kills[cnt] = 2;
		deaths[cnt] = 1;
		matches[cnt] = 3;
		shots[cnt] = 0;

	}
*/
 }

/*void Stats::winLossChk(int a, int r) {
	 //placeholder = 1
	 //r refers to robID#
	 //a refers to armor @ ID#
	 //assumes battle has completed

	 if (a <= 0) {
		 //loosers
		 deaths[r]++;
		 matches[r]++;

	 } else {
		 //winner
		 wins[r]++;
		 matches[r]++;
	 }


 }*/

void Stats::displayHead() {
	cout<<"-------------------------------------------------------"<<endl;
	cout<<"|Robot\t\tWins\tMatches\tKills\tDeaths\tShots|"<<endl;
	cout<<"-------------------------------------------------------"<<endl;
}

void Stats::displayLine(string n, int w, int k, int d, int s) {
	cout << n << " -\t" << w << "\t" << 1 << "\t" << k << "\t" << d << "\t" << s << endl;
}

/*void Stats::shotCnt(int s, int r) {
	//assumes the process for shooting is done elseware
	shots[r] += s;
}

void Stats::killCnt(int r) {
	//assumes hit detection will associate robots to kills
	kills[r]++;
}

int Stats::getWins(int r) {
	return wins[r];
}

int Stats::getKills(int r) {
	return kills[r];
}

int Stats::getDeaths(int r) {
	return deaths[r];
}

int Stats::getMatches(int r) {
	return matches[r];
}

int Stats::getShots(int r){
	return shots[r];
}
*/
