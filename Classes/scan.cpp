#include "scan.h"
#include <robot.h>

using namespace std;


Scan::Scan(void)
{
    return;
}


void Scan::setArc(int inA)
{
    if (inA > 0)
        arc = inA;
    else
        arc = 0;

    return;
}


int Scan::getArc(void)
{
    return arc;
}


int Scan::doScan(int turAng)
{
    int i=0, x1, y1, x2, y2;
    int arrSize = (sizeof(Robot.robot_arr)/sizeof(Robot.robot_arr[0]));

    for (i=0;i<arrSize;i++)
        i++;
}

void Scan::setRange(int sRange)
{
    range = sRange;

    return;
}
