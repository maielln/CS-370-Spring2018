#ifndef SCAN_H
#define SCAN_H
#include <robot.h>

using namespace std;

class Scan
{

    public:
        Scan(void); //initializer, passed the robot's scan range
        void setRange (int);
        void setArc (int); //sets scan arc
        int getArc (void); //returns scan arc
        int doScan (int); //passed turret angle, returns a value from -3 to 2 (-3 means no robot found, all others are scan sectors)

    private:
        int range, arc;

};

#endif
