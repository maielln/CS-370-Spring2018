/* Name: Stats.h
	Desc: The purpose of this class is to store and
		manage the stats of each robot. this means
		calculating kills, deaths, wins, matches,
		and shots.
	Coder(s): Eric & Tyler
	Date: 2/13/2018
	Last Update: 2/15/2018
*/

#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

 //on the fly
const int TOTAL_ROBOTS = 5;

//make this MAX ROBOTS
//make new var for num robots
//0 bob
//1 steve
//2 becky

class Stats {
	int  wins[TOTAL_ROBOTS], kills[TOTAL_ROBOTS], deaths[TOTAL_ROBOTS],
		matches[TOTAL_ROBOTS], shots[TOTAL_ROBOTS];

	 public:

	 void init();
	 void winLossChk(int, int);
	 void displayHead();
	 void displayLine(int);
	 void shotCnt(int, int);
	 void killCnt(int); //need kill counter
	 int getWins(int);
	 int getKills(int);
	 int getDeaths(int);
	 int getMatches(int);
	 int getShots(int);

 };

void Stats::init()  {
	//Initializes the base stats for each rbt
	for (int cnt = 0; cnt < TOTAL_ROBOTS; cnt++) {
		wins[cnt] = 1;
		kills[cnt] = 2;
		deaths[cnt] = 1;
		matches[cnt] = 3;
		shots[cnt] = 0;

	}

 }

void Stats::winLossChk(int a, int r) {
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


 }

void Stats::displayHead() {
	printf("-------------------------------------------------------");
	printf("|Robot           Wins  Matches  Kills  Deaths    Shots|");
	printf("-------------------------------------------------------");
}

void Stats::displayLine(int r) {
	cout << r << " - " << wins[r] << " " << matches[r] << " " << kills[r] << " " << deaths[r] << " " << shots[r] << endl;
}

void Stats::shotCnt(int s, int r) {
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

