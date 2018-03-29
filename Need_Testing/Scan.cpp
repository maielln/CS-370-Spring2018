#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "Robot.h"

using namespace std;

int scan(int n)
{
    double r,d,acc;
    int dir,range,i,j,k,l,nn,xx,yy,sign;
    nn = -1;
    range = maxint;
    if (!(n  >= 0 && n <=num_robots)
    {
        return -3;                      //This result of an invalid entry placeholder return
    }
    //with robot[n]^ do
    if( robot[n].scanarc<0)
    {
        scanarc=0;
    }

    robot[n].accuracy=0;
    nn=-1;
    dir=(shift+hd) && 255;      //possible robot class values
    if (debug_info)
    {
        cout<<"<SCAN Arc="<<scanarc<<", Dir="<<dir<<">";
    }
    for (i=0; i < num_robots;i++)
    {
        if (i!=n) && (robot[i].health>0)
        {
            j = find_anglei(robot[n].x,robot[n].y,robot[i].x,robot[i].y);
            d = distance(robot[n].x,robot[n].y,robot[i].x,robot[i].y);
            k = round(d);
            if ((k<range) && (k<=scanrange) && ((abs(j-dir)<=abs(scanarc)) || (abs(j-dir)>=256-abs(scanarc))))
            {
                dir = (dir+1024) && 255;
                xx = round( sint[dir]*d+robot[n].x);
                yy = round(-cost[dir]*d+robot[n].y);
                r = distance(xx,yy,robot[i].x,robot[i].y);
                if (debug_info)
                {
                    cout<<"SCAN HIT! Scan X,Y: "<<round(xx)<<","<<round(yy)<<"  Robot X,Y: "<<
                        round(robot[i].x)<<","<<round(robot[i].y)<<"  Dist="<<round(r);
                    //repeat until keypressed; flushkey;                //THIS WAS FOR DEBUG MODE!!!! UPDATE CODE LATER!!!!
                }
                if ((scanarc>0) || (r<hit_range-2))
                {
                    range = k;
                    accuracy = 0;
                    if (scanarc>0)
                    {
                        j=(j+1024) && 255;
                        dir=(dir+1024) && 255;
                        if (j<dir)
                        {
                            sign=-1;
                        }
                        if (j>dir)
                        {
                            sign= 1;
                        }
                        if ((j>190) && (dir<66))
                        {
                            dir=dir+256;
                            sign=-1;
                        }
                        if ((dir>190) && (j<66))
                        {
                            j = j+256;
                            sign = 1;
                        }
                        acc = abs(j-dir)/robot[n].scanarc*2;
                        if (sign<0)
                        {
                            robot[n].accuracy=-abs(round(acc))
                        }

                        else
                        {
                            accuracy = abs(round(acc));
                        }

                        if (accuracy>2)
                        {
                            accuracy = 2;
                        }
                        if (accuracy<-2)
                        {
                            accuracy=-2;
                        }
                    }
                    nn = i;
                    if (debug_info)
                    {
                        cout<<#13<<zero_pad(game_cycle,5)<<" S "<<n<<": nn="<<nn<<", range="<<range<<", acc="<<accuracy<<endl;           ');
                        //repeat until keypressed; flushkey;            //AGAIN DEBUG MENU
                    }
                }
            }

            if (nn  >= 0 && nn <=num_robots)
            {
                ram[5] = robot[nn].transponder;
                ram[6] = (robot[nn].hd-(hd+shift)+1024) && 255;
                ram[7] = robot[nn].spd;
                ram[13] = round(robot[nn].speed*100);
            }
        }
    }
    return range;
}

int find_anglei(double xx,double yy,double tx,double ty)
{
    int i;
    i=round(find_angle(xx,yy,tx,ty)/pi*128+256);
    while (i<0)
    {
        i += 256);
    }

    i = i && 255;
    return i;
}

double find_angle(double xx,double yy,double tx,double ty)
{
    int i,j,k,v,z;
    double q;
    q=0;
    v=abs(round(tx-xx));
    if (v=0)
    {
        //{v=0.001;} commented out in original
        if (tx=xx) && (ty>yy)
        {
            q = pi;
        }

        if (tx=xx) && (ty<yy)
        {
            q = 0;
        }
    }
    else
    {
        z=abs(round(ty-yy));
        q=abs(atan(z/v));
        if ((tx>xx) && (ty>yy))
        {
            q=pi/2+q;
        }
        if ((tx>xx) && (ty<yy))
        {
            q = pi/2-q;
        }
        if ((tx<xx) && (ty<yy))
        {
            q=pi+pi/2+q;
        }
        if ((tx<xx) && (ty>yy))
        {
            q=pi+pi/2-q;
        }
        if ((tx=xx) && (ty>yy))
        {
            q=pi/2;
        }
        if ((tx=xx) && (ty<yy))
        {
            q=0;
        }
        if ((tx<xx) && (ty=yy))
        {
            q=pi+pi/2;
        }
        if ((tx>xx) && (ty=yy))
        {
            q=pi/2;
        }
    }
    return q;
}

double distance(int x1, int y1, int x2, int y2)
{
    return pow((pow(x2-x1,2) + pow(y2-y1,2)),.5);
}
