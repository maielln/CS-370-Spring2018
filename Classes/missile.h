/* 
Name: Stats.h	
Desc: The purpose of this class is to store and
		manage the data of each missile, such as
		the path it will travel, the damage it deals
		and confirming a hit. 
	Coder(s): Eric & Tyler
	Date: 2/22/2018
	Last Update: 2/23/2018
*/

#include <iostream>
#include <stdio.h>
#include <string>
#include <iomanip>
#include "Robot.h"
using namespace std;


const int MAX_MISSILES = 3; 
const int TOTAL_ROBOTS = 5; 
const int MAX_RANGE = 80; 

class Missile {
	int speed, dam, lastShot; 
		
	double xCor[MAX_RANGE], yCor[MAX_RANGE]; 
	
	
	public:
	void missileInit(); 
	void shoot(int); 
	bool hitD(int); 
	double damage(int); 
	void line(int, int); //x and y are doubles?
	
	//getters 
	double getDam(); 
	int getLastShot(); 
	
	
}

void Missile::missileInit() {
	dam = 0; 
	speed = 1; 
	
	for (int cnt = 0; (cnt * speed) < MAX_RANGE; cnt++) { //wayy to large a range
		yCor[cnt] = 0; 
		xCor[cnt] = 0; 
			
	} 
	
}

void Missile::shoot(r) {
	lastShot = r; 
}

bool Missile::hitD(r, x, y) {	
	bool hit =  false; 
	int cnt = 0; 
	
	while (cnt < MAX_RANGE) {
		if ((x == xCor[cnt]) && (y == yCor[cnt]))
			hit = true;
		
		cnt++;
	}
	
	return hit; 
}


double Missile::damage (shotStrength) { 

	dam = shotStrength * .80; 
	
	return dam; 
	
}
	
//this is the line the missile will follow 	
void Missile::line(x, y) {
	//assume the robot is origin (0,0)
	double m, b; 
	
	//create slope
	m = y/x; 
	b = y / (m * x); 
	
	//creates the coordinates for the missile to follow, 
	for (int cnt = 0; (cnt * speed) < MAX_RANGE; cnt++) { //wayy to large a range
		yCor[cnt] = (m * cnt) + b; 
		xCor[cnt] = cnt; 
			
	} 
	
	//we can figure out what "quadrant" the
	//target is in, and change direction depending on it 

	//do we even have to worry about quadrants?
	
}	

double Missile::getDam() {
	return dam; 
}

int Missile::getLastShot() {
	return lastShot; 
}
