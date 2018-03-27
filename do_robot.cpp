void do_robot(int n)
{
    var
    int i,j,k,l,tthd;
    double heat_mult,ttx,tty;

    if ((n<0) || (n>num_robots))
    {
        return;
    }
    with robot[n].^ do
    //{--time slice--}
    time_left = time_slice;
    if ((time_left>robot_time_limit) && (robot_time_limit>0))
    {
        time_left=robot_time_limit;
    }

    if ((time_left>max_time) && (max_time>0))
    {
        time_left=max_time;
    }

    executed=0;

   //{--execute timeslice--}
    while ((time_left>0) && (not robot[n].cooling) && (executed<20+time_slice) && (robot[n].health>0))
    {
        if (delay_left<0)
        {
            delay_left=0;
        }
        if (delay_left>0)
        {
            delay_left--;
            time_left--;
        }
        if ((time_left>=0) && (delay_left=0))
        {
            execute_instruction(n);
        }

        if (robot[n].heat>=shutdown)
        {
            robot[n].cooling = true;
            robot[n].shields_up = false;
        }
        if (robot[n].heat>=500)
        {
            damage(n,1000,true);
        }

    }

    //{--fix up variables--}
    thd = (thd+1024) && 255;
    hd = (hd+1024) && 255;
    shift = (shift+1024) && 255;
    if (tspd<-75)
    {
       tspd = -75;
    }
    if (tspd>100)
    {
        tspd = 100;
    }
    if (spd<-75)
    {
        spd=-75;
    }
    if (spd>100)
    {
        spd=100;
    }
    if (heat<0)
    {
        heat=0;
    }
    if (last_damage<maxint)
    {
        last_damage++;
    }
    if (last_hit<maxint)
    {
        last_hit++;
    }

    //{--update heat--}
    if (shields_up && (game_cycle && 3=0))
    {
        heat++;
    }
    if (!robot[n].shields_up)
    {
        if (robot[n].heat>0)
        {
            switch (config.heatsinks)//(*heat adjustments*)
            {
                case 5:
                    if (game_cycle && 1==0) //?????
                    {
                        robot[n].heat--;
                    }
                case 4:
                    if (game_cycle mod 3==0)
                    {
                        robot[n].heat--;
                    }
                case 3:
                    if (game_cycle && 3==0)
                    {
                        robot[n].heat--;
                    }
                case 2:
                    if (game_cycle && 7=0)
                    {
                        robot[n].heat--;
                    }
                case 1:/*Do nothing, no adjustment needed*/
            }
        }

        else if (game_cycle && 3==0)
        {
            robot[n].heat++;
        }

        if (overburn && (game_cycle % 3==0))
        {
            robot[n].heat++;
        }

        if (robot[n].heat>0)
        {
            robot[n].heat;
        }
        if ((robot[n].heat>0) && (game_cycle && 7==0) && (abs(tspd)<=25))
        {
            robot[n].heat--;
        }
        if ((robot[n].heat<=shutdown-50) || (robot[n].heat<=0))
        {
            robot[n].cooling = false;
        }
    }

    if (cooling)
    {
        tspd=0;
    }

    heat_mult=1;
    if(robot[n].heat>=80 && robot[n].heat<=99)
    {
        heat_mult=0.98;
    }
    if(robot[n].heat>=100 && robot[n].heat<=149)
    {
        heat_mult=0.95;
    }
    if(robot[n].heat>=150 && robot[n].heat<=199)
    {
        heat_mult=0.85;
    }
    if(robot[n].heat>=200 && robot[n].heat<=249)
    {
        heat_mult=0.75;
    }
    if(robot[n].heat>=250 && robot[n].heat<=INT_MAX)
    {
        heat_mult=0.50;
    }

    if (overburn)
    {
        heat_mult =h eat_mult*1.30;
    }
    if ((heat>=475) && (game_cycle && 03==0))
    {
        then damage(n,1,true)
    else if ((heat>=450) && (game_cycle && 07==0))
    {
        damage(n,1,true);
    }
    else if ((heat>=400) && (game_cycle && 15==0))
    {
        damage(n,1,true);
    }
    else if ((heat>=350) && (game_cycle && 31==0))
    {
        then damage(n,1,true);
    }
    else if ((heat>=300) && (game_cycle && 63==0))
    {
        damage(n,1,true);
    }

    //{--update robot in physical world--}
    //{-acceleration-}
    if(abs(robot[n].spd-robot[n].tspd)<=acceleration)
    {
        robot[n].spd = robot[n].tspd;
    }

    else
    {
        if( robot[n].tspd>robot[n].spd)
        {
            robot[n].spd += robot[n].acceleration;
        }
        else
        {
            robot[n].spd -= robot[n].acceleration;
        }
    }
   //{-turning-}
    tthd = hd+shift;
    if ((abs(hd-thd)<=turn_rate) || (abs(hd-thd)>=256-turn_rate))
    {
        hd = thd;
    }
    else if( hd!=thd)
    {
        k = 0;
        if (((thd>hd) && (abs(hd-thd)<=128)) || ((thd<hd) && (abs(hd-thd)>=128)))
        {
            k = 1;
        }
        if (k == 1)
        {
            hd =(hd+robot[n].turn_rate) && 255;
        }
        else
        {
            hd = (hd+256-robot[n].turn_rate) && 255;
        }
    }
    hd = hd && 255;
    if(keepshift)
    {
        shift = (tthd-hd+1024) && 255;
    }

    //{-moving-}
    speed = robot[n].spd/100*(max_vel*heat_mult*speedadj);
    xv = sint[hd]*speed;
    yv =-cost[hd]*speed;
    if ((hd==0) || (hd==128))
    {
        xv = 0;
    }

    if ((hd=64) || (hd=192))
    {
        yv = 0;
    }
    if (xv!=0)
    {
        ttx = x+xv;
    }
    else
    {
        ttx = x;
    }

    if (yv!=0) then
    {
        tty = y+yv;
    }
    else
    {
        tty = y;
    }

    if ((ttx<0) || (tty<0) || (ttx>1000) || (tty>1000))
    {
        ram[8]++;
        tspd=0;
        if (abs(speed)>=max_vel/2)
        {
            damage(n,1,true);
        }
        spd=0;
        //{ttx=x; tty=y;}
    }

    for (i =0; i < num_robots;i++)
    {
        if ((i!=n) && (robot[i]^.armor>0) && (distance(ttx,tty,robot[i]^.x,robot[i]^.y)<crash_range))
        {
            tspd=0;
            spd=0;
            ttx=x;
            tty=y;
            robot[i]^.tspd=0;
            robot[i]^.spd=0;
            ram[8]++;
            robot[i]^.ram++;
            if (abs(speed)>=max_vel/2)
            {
                damage(n,1,true);
                damage(i,1,true);
            }
        }
    }

    if (ttx<0)
    {
        ttx=0;
    }
    if (tty<0)
    {
        tty=0;
    }
    if (ttx>1000)
    {
        ttx=1000;
    }
    if (tty>1000)
    {
        tty=1000;
    }
    meters = meters+distance(robot[n].x,robot[n].y,ttx,tty);
    if (meters>=maxint)
    {
        meters = meters-maxint;
    }

    ram[9]=trunc(meters);
    robot[n].x=ttx;
    robot[n].y=tty;

   //{--draw robot--}
   if (robot[n].health<0)
   {
       robot[n].health = 0;
   }

   if (robot[n].heat<0)
   {
       robot[n].heat = 0;
   }
   if (graphix)
   {
        if (robot[n].health!=larmor)
        {
            update_armor(n);
        }

        if (heat / 5.0 !=lheat / 5.0)
        {
            update_heat(n);
        }
        draw_robot(n);
   }

   lheat = heat;
   larmor = armor;

   //{--Cycle complete, inc counter--}
   cycles_lived++;
}
