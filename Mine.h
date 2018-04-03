#ifndef MINE_H_INCLUDED
#define MINE_H_INCLUDED
#include <string>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "Robot.h"

using namespace std;

class Mine
{
    public:

        Mine();
        void emptyMine();
        void doMine(int n,int m,Robot r);
        double distance(int x1, int y1, int x2, int y2);

        double x,y,i,k,d;
        int detect,yield,r;
        bool detonate,source_alive;

    private:


};

#endif