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
//const int TOTAL_ROBOTS = 5;
#ifndef STATS
#define STATS
//make this MAX ROBOTS
//make new var for num robots
//0 bob
//1 steve
//2 becky

class Stats {
//	int  wins[TOTAL_ROBOTS], kills[TOTAL_ROBOTS], deaths[TOTAL_ROBOTS],
	//	matches[TOTAL_ROBOTS], shots[TOTAL_ROBOTS];

	 public:
	 void init();
	 void winLossChk(int, int);
	 void displayHead();
	 void displayLine(string, int, int, int, int);
	 void shotCnt(int, int);
	 void killCnt(int); //need kill counter
	 int getWins(int);
	 int getKills(int);
	 int getDeaths(int);
	 int getMatches(int);
	 int getShots(int);

 };

#endif
