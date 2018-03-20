#include "Mine.h"



//sets mine's value to default
Mine::Mine (void)
{
    x = -1;
    y = -1;
    yield = 0;
    detonate = false;
    detect = 0;
}

void Mine::emptyMine (void)
{
    x = -1;
    y = -1;
    yield = 0;
    detonate = false;
    detect = 0;
}

void Mine::doMine(int n,int m,Robot r)
{
    if ((x>=0) && (x<=1000) && (y>=0) && (y<=1000) && (yield>0))
    {

        if ((r.health>0) && (i!=n))
        {
            d = distance(x,y,r.x,r.y);
            if (d<=detect)
            {
                detonate = true;
            }
        }


        if (detonate)
        {
            //init_missile(x,y,0,0,0,n,mine_circle,false);
            //kill_count = 0;
            if (r.health>0)
            {
                source_alive = true;
            }
            else
            {
                source_alive = false;
            }

            if (r.health>0) /*original comment placed before boolean expression(i!=n) &&*/
            {
                k = (int)(round(distance(x,y,r.x,r.y)));
                if (k<yield)
                {
//                    damage(i,round(abs(yield-k)),false);        //come up with own function????
                    if (i!=n)
                    {
//                        r.damage_total +=round(abs(yield-k));
                    }
                }
            }
            if (/*(kill_count>0) &&*/ (source_alive) && (r.health<=0))
            {
//                kill_count--;
            }
            if (/*kill_count>0*/ true)
            {
//                r.kills += kill_count;
//                update_lives(n);
            }
            /*if graphix then                                                       Graphics stuff idk what to do with it now
            putpixel(round(x*screen_scale)+screen_x,round(y*screen_scale)+screen_y,0);
            yield:=0; x:=-1; y:=-1;*/

            else            //continue from here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            {
                /* Draw mine */
                /*if graphix and (game_cycle and 1=0) then      more graphical stuff from the old code
                {
                    main_viewport;
                    setcolor(robot_color(n));
                    line(round(x*screen_scale)+screen_x,round(y*screen_scale)+screen_y-1,
                    round(x*screen_scale)+screen_x,round(y*screen_scale)+screen_y+1);
                    line(round(x*screen_scale)+screen_x+1,round(y*screen_scale)+screen_y,
                    round(x*screen_scale)+screen_x-1,round(y*screen_scale)+screen_y);
                }*/
            }
        }
    }

}

double Mine::distance(int x1, int y1, int x2, int y2)
{
    return pow((pow(x2-x1,2) + pow(y2-y1,2)),.5);
}
