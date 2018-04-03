/*
Name: Missile.h
Desc: The purpose of this class is to store and
		manage the data of each missile, such as
		the path it will travel, the damage it deals
		and confirming a hit.
	Coder(s): Eric & Tyler
	Date: 2/22/2018
	Last Update: 2/23/2018
*/
/*
  This file needs to be updated, and directly ported from Confer's code
*/
#include <iostream>
#include <stdio.h>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <cmath>

using namespace std;

#ifndef MISSILE
#define MISSILE

const int MAX_MISSILES = 3;
const int TOTAL_ROBOTS = 5;
const int MAX_RANGE = 80;

class Missile {
    public:
	int speed, dam, lastShot, owner, tarx = 0, tary = 0;
	double xChange, yChange;
	bool does_Exist;

	double xCor[MAX_RANGE], yCor[MAX_RANGE];

    Missiles();
	void initMissile(int, int, int);
	void shoot();
	bool hitD();
	void damage(double);
	void setTar(int, int);
	void myMove();
//	void line(); //x and y are doubles?
    double x;
    double y;

	//getters
	double getDam();
	int getLastShot();


};

#endif
