#include <iostream>
#include <ctime>
#include <stdlib.h>
#include "Missile.h"

using namespace std;

Missile::Missiles(){}

void Missile::initMissile(int r, int xx, int yy) {
    owner = r;
	dam = 0;
	speed = 1;
    x = xx;
    y = yy; //giving missile the same origin as its robot
    tarx = 0;
    tary = 0; //initializing target coordinates
    does_Exist = true;

   // cout<<x<<", "<<y<<endl;

//	for (int cnt = 0; (cnt * speed) < MAX_RANGE; cnt++) { //wayy to large a range
	//	yCor[cnt] = 0;
    //  xCor[cnt] = 0;

	}

void Missile::setTar(int tx, int ty)
{
    tarx = tx;
    tary = ty;
}

void Missile::shoot() {
    if(tarx == 0 && tary == 0) //if there is no set target, fire randomly
    {
        tarx = rand() % 1000;
        tary = rand() % 1000;
    }
    double dx = 0;
    double dy = 0;
    xChange = tarx-x;
    yChange = tary-y;
    if(abs(xChange) > abs(yChange))
    {
        dx = xChange/abs(xChange); //dividing by abs to preserve possible negative values
        dy = yChange/abs(xChange);
    }
    else if(abs(yChange) > abs(xChange))
    {
        dx = xChange/abs(yChange);
        dy = yChange/abs(yChange);
    }
    else
    {
        dx = xChange/abs(xChange);
        dy = yChange/abs(yChange);
    }
    xChange = dx;
    yChange = dy; //putting both variables back into global

}

void Missile::myMove()
{
    x += xChange;
    y += yChange;
}

bool Missile::hitD() {
	bool hit = false;
//	int cnt = 0;

	//while (cnt < MAX_RANGE) {
		if (((int)x == (int)tarx) && ((int)y == (int)tary))
			hit = true;

		//cnt++;
	//}

	return hit;
}


void Missile::damage (double shotStrength) {

	dam = shotStrength * .80 * 10;

}

//this is the line the missile will follow
//void Missile::line() {
	//assume the robot is origin (0,0)
	//double m, b;

	//create slope


	//creates the coordinates for the missile to follow,
	//for (int cnt = 0; (cnt * speed) < MAX_RANGE; cnt++) { //wayy to large a range
		//yCor[cnt] = (m * cnt) + b;
		//xCor[cnt] = cnt;

	//}
//	x+=xChange;
//	y+=yChange;
//	cout<<x<<", "<<y<<endl;

	//we can figure out what "quadrant" the
	//target is in, and change direction depending on it

	//do we even have to worry about quadrants?

//}

double Missile::getDam() {
	return 20;
}

int Missile::getLastShot() {
	return lastShot;
}
