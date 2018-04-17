string addrear(string b,int l);
double distance(int x1, int y1, int x2, int y2);
int rol(int n,int k);
int ror(int n,int k);
int sal(int n,int k);
int sar(int n,int k);

string addrear(string b,int l)
{
    while ((int)(b.length())< l)
    {
        b = b+' ';
    }
    return b;
}

double distance(int x1, int y1, int x2, int y2)
{
    return pow((pow(x2-x1,2) + pow(y2-y1,2)),.5);
}

int rol(int n,int k)        
{
/*  asm                     //THE NEXT FEW FUNCTIONS NEED TO KNOW HOW TO WORK WITH ASSEMBLY!!!!!
    cld
  mov   cx,     k
  rep   rol     n,      1
*/
 return n;
}

int ror(int n,int k)
{
/*    asm
    cld
    mov   cx,     k
    rep   ror     n,      1
    end;
*/
    return n;
}

int sal(int n,int k)
{
/*    asm
    cld
    mov   cx,     k
    cmp   cx,     0
    jle   @2
    @1:
    sal   n,      1
    loop  @1
    @2:
*/
    return n;
}

int sar(int n,int k)
{
/*    asm
    cld
    mov   cx,     k
    cmp   cx,     0
    jle   @2
    @1:
    sar   n,      1
    loop  @1
    @2:
*/
    return n;
}

double find_angle(double xx,double yy,double tx,double ty)
{
    int v,z;
    double q;
    q=0;
    v=abs(round(tx-xx));
    if (v==0)
    {
        //{v=0.001;} commented out in original
        if ((tx==xx) && (ty>yy))
        {
            q = pi;
        }
        if ((tx==xx) && (ty<yy))
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

int find_anglei(double xx,double yy,double tx,double ty)
{
    int i;
    i=round(find_angle(xx,yy,tx,ty)/pi*128+256);
    while (i<0)
    {
        i += 256;
    }
    i = i & 255;
    return i;
}
