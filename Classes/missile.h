/* 
Name: Stats.h	
Desc: The purpose of this class is to store and
		manage the stats of each robot. this means
		calculating kills, deaths, wins, matches,
		and shots.
	Coder(s): Eric & Tyler
	Date: 2/22/2018
	Last Update: 2/22/2018
*/

#include <iostream>
#include <stdio.h>
#include <string>
#include <iomanip>
#include <cstdint>
using namespace std;


const int MAX_MISSILES = 3; 
const int TOTAL_ROBOTS = 5; 
const int MAX_RANGE = 800; 

class Missile {
	int speed[TOTAL_ROBOTS], damage[TOTAL_ROBOTS], xt[TOTAL_ROBOTS], 
		yt[TOTAL_ROBOTS], xCor[MAX_RANGE], yCor[MAX_RANGE]; 
		
	int x, y; //our x and y position , assume 0,0 when 
			//it is immediatly fired 		
	
	
	public:
	void shoot(int); 
	bool hitD(int); 
	double damage(int); 
	void line(int, int); 
	
}
	//once the missle class is used, shouldn't we already know who shot last?
void shoot(r) {
	//r dictates robot

}

bool hitD(r) {	
	//pass in x and y of target from somewhere else
	//use x and y of missile from another func
	bool hit =  false; 
	int cnt = 0; 
	
	while (cnt < MAX_RANGE) {
		if ((xt[r] == xCor[cnt]) && (yt[r] == yCor[cnt]))
			hit = true;
		
		cnt++;
	}
	
	return hit; 
}


//damage assumes a hit has been confirmed, and we know
//what robot is doing the damage
double damage (r) {
	double dam; 

	
	//the robot doing the damage
	dam = robot[r].getdamageadj() * .80
	
	
	//i don't know what the method is called,
	//so correct this when available
	
	//dam gets deducted from the armor value of the robot
	
	return dam; 
	
}
	
//this is the line the missile will follow 	
void line(x, y) {
	//assume the robot is origin (0,0)
	int m, b; 
	
	//create slope
	m = y/x; 
	b = y / (m * x); 
	
	//creates the coordinates for the missile to follow, 
	for (int cnt = 0; cnt < MAX_RANGE; cnt++) { //wayy to large a range
		yCor[cnt] = (m * cnt) + b 
		xCor[cnt] = cnt; 
			
	} 
		
	
	//we can figure out what "quadrant" the
	//target is in, and change direction depending on it 

	//do we even have to worry about quadrants?
	
}	