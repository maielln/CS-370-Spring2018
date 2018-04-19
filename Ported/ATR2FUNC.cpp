#include <iostream>
#include <ctime>
#include <string>
#include <math.h>
#include <chrono>
#include <thread>

#define pi 3.1415926535897932384626433

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

long delay_per_sec;
bool registered,graphix,sound_on;
string reg_name;
uint16_t reg_num;            //ORIGIONALLY TYPE :word!!!!!!!!!!!
double sint[256],cost[256];

string addrear(string b,int l);
double distance(int x1, int y1, int x2, int y2);
int rol(int n,int k);
int ror(int n,int k);
int sal(int n,int k);
int sar(int n,int k);
double abs(double x);
void textxy(int x,int y, string s);
void coltextxy(int x,int y,string s,uint8_t c);
char hexnum(uint8_t num);
string hexb(uint8_t num);
string hex(uint16_t num );
double valuer(string i);
long value(string i);
string cstrr(double i);
string cstr(long i);
string zero_pad(long n,long l);
string zero_pads(string s ,long l);
string addfront(string b,int l);
string ucase(string s);
string lcase(string s);
string space(uint8_t i);
string repchar(char c,uint8_t i);
string ltrim(string s1);
string rtrim(string s1);
string btrim(string s1);
string lstr(string s1,int  l);
string rstr(string s1,int l);
void FlushKey();
void calibrate_timing();
void time_delay(int n); //{n=milliseconds}
void check_registration();
void viewport(int x1,int y1,int x2,int y2);
void main_viewport();
void make_tables();
//FINISH FUNCTIONS BELOW HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!
int robot_color(int n);
void box(int x1,int y1,int x2,int y2);
void hole(int x1,int y1,int x2,int y2);
void chirp();
void click();
int hex2int(string s);
int str2int(string s);
string bin(int n);
string decimal(int num,int length );

void textxy(int x,int y, string s)  //GUI PEOPLE NEED TO DO THIS (prints string to screen at cord. x & y)
{
/*    setfillstyle(1,0);
    bar(x,y,x+length(s)*8,y+7);
    outtextxy(x,y,s);
*/
}


void coltextxy(int x,int y,string s,uint8_t c)//GUI PEOPLE NEED TO DO THIS(changes color of the text and displays string)
{
/*    setcolor(c);
    textxy(x,y,s);
*/
}

char hexnum(uint8_t num)
{
    switch(num)
    {
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return 'A';
        case 11: return 'B';
        case 12: return 'C';
        case 13: return 'D';
        case 14: return 'E';
        case 15: return 'F';
        default: return 'X';
    }
}

string hexb(uint8_t num)
{
    return ((hexnum(num >> 4) + hexnum(num & 15))+"");
}

string hex(uint16_t num )
{
    return ((hexb(num >> 8) + hexb(num & 255))+"");
}

double valuer(string i)
{
    double s;
    int n;
 //   val(i,s,n);
    if (n>0)
    {
        s = 0;
    }
    return s;
}

 long value(string i)
 {
    long s;
    int n;
  //  val(i,s,n);
    if (n>0)
    {
        s = 0;
    }
    return s;
 }

 string zero_pads(string s ,long l)
 {
    string s1;
    s1=s;
    while (s1.length()<l)
    {
        s1 = '0'+s1;
    }

    return s1;
 }

 string addfront(string b,int l)
 {
    while (b.length()< l)
    {
        b = ' '+b;
    }
    return b;
 }

string ucase(string s)
{
    for(int i = 0; i < s.length(); i++)
    {
        if(s[i] >= 97 && s[i] <= 122)
        {
            s[i] = s[i] - 32;
        }
    }
    return s;
}

string lcase(string s)
{
    int i;
    if (s.length()>=1)
    {
        for (i = 0; i< s.length();i++)
        {
            if ((s[i]>=65) && (s[i]<=90))
            {
                s[i] = s[i]+32;
            }
        }

    }
    return s;
}

string space(uint8_t i)
{
    string s;
    int k ;
    s = "";
    if (i>0)
    {
        for (k = 1; k <= i; k++)
        {
            s = s+' ';
        }
    }
    return s;
}

string repchar(char c,uint8_t i)
{
    string s;
    int k;

    s = "";
    if (i>0)
    {
        for (k = 1; k <= i; k++)
        {
            s = s + c;
        }
    }
    return s;
}


string addrear(string b,int l)
{
    while ((int)(b.length())< l)
    {
        b = b+' ';
    }
    return b;
}

string zero_pad(long n,long l)
{
    string s;

    s = n + "";
    while (s.length()<l)
    {
        s ='0'+s;
    }

    return s;
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

void FlushKey()                 //{ Clears any key strokes in the key-  }
                                //{ board buffer so a couple of key     }
{                               //{ presses don't race you through program. }
/*    Regs : Registers;
    Regs.AH := $01;  { AH=1: Check for keystroke }
    Intr($16,regs); { Interupt $16: Keyboard services}
    IF (regs.Flags and $0040) = 0 then { if chars in buffer }
        REPEAT
        Regs.AH := 0;
        Intr($16,Regs);
        Regs.AH := $01;
        Intr($16,Regs);
    Until (regs.flags and $0040) <> 0;
    */
}

void calibrate_timing()
{
    long i,k;

    delay_per_sec = 0;
/*    k=mem[0:$46C];
    repeat until (k!=mem)[0:$46C];
    k=mem[0:$46C];
    repeat delay(1);
    delay_per_sec++;
    until k<>mem[0:$46C];
    delay_per_sec = round(delay_per_sec*18.2);
*/
}

void time_delay(int n)
{
     long i,l;
     if (delay_per_sec==0)
     {
         calibrate_timing();
     }
     l = round(n/1000*delay_per_sec);

     for (i = 1; i<= l; i++)
     {
         sleep_for(nanoseconds(10));
     }
}

void check_registration()
{
    uint8_t w;
    int i;
    //f:text;
    string s;

    registered = false;
    if (/*exist('ATR2.REG')*/true)
    {
 /*       assign(f,'ATR2.REG');
        reset(f);
        readln(f,reg_name);
        readln(f,reg_num);
        close(f);
        w = 0;
        s = btrim(ucase(reg_name));
*/        for (i=0; i < s.length();i++)
        {
            w+=(int)(s[i]);
        }

        w = w ^ 23205;
        if (w==reg_num)
        {
            registered = true;
        }
    }
}

void viewport(int x1,int y1,int x2,int y2)      //GUI CODE!!!!!!!!!!!!!!
{
 /*   if (!graphix)
    {
        return;
    }
    setviewport(x1,y1,x2,y2,true);
*/
}

void main_viewport()    //MORE CODE FOR GUI!!!!!!!!!
{
    //viewport(5,5,474,474); //{470x470}
}

void make_tables()
{
    int i,j,k;
    for (i = 0; i<= 255;i++)
    {
        sint[i] = sin(i/128*pi);
        cost[i] = cos(i/128*pi);
    }
}

int robot_color(int n)
{
    int k;
    k = 7;
    switch (n % 14)
    {
        case 0:k = 10;
            break;
        case 1:k = 12;
            break;
        case 2:k = 9;
            break;
        case 3:k = 11;
            break;
        case 4:k = 13;
            break;
        case 5:k = 14;
            break;
        case 6:k = 7;
            break;
        case 7:k = 6;
            break;
        case 8:k = 2;
            break;
        case 9:k = 4;
            break;
        case 10:k = 1;
            break;
        case 11:k = 3;
            break;
        case 12:k = 5;
            break;
        case 13:k = 15;
            break;
        default: k = 15;
            break;
    }
    return k;
}

double abs(double x)
{
    if(x<0)
    {
        x*=-1;
    }
    return x;
}

double find_angle(double xx,double yy,double tx,double ty)
{
    int v,z;
    double q;
    q=0;
    v=(int)(abs(round(tx-xx)));
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

void box(int x1,int y1,int x2,int y2)    //GUI PEOPLE (Draws a box at coordinates)
{

}

void hole(int x1,int y1,int x2,int y2)    //GUI PEOPLE (Don't know exactly what this does )
{
/*    i:integer;
    begin
    if not graphix then exit;
    if x2<x1 then begin i:=x1; x1:=x2; x2:=i; end;
    if y2<y1 then begin i:=y1; y1:=y2; y2:=i; end;
    setfillstyle(1,0);
    setcolor(0);
    bar(x1,y1,x2,y2);
    setcolor(8);
    line(x1,y1,x2-1,y1);
    line(x1,y1,x1,y2-1);
    setcolor(15);
    line(x1+1,y2,x2,y2);
    line(x2,y1+1,x2,y2);
    putpixel(x1,y2,7);
    putpixel(x2,y1,7);
    */
}

void chirp()        //GUI PEOPLE!!!! (sound)
{

}

void click()        //GUI PEOPLE!!!! (shooting/explosion sound)
{

}

int hex2int(string s)
{
    uint16_t w;
    int i,j;

    i = -1; w = 0;
    while (i<s.length())
    {
        i++;
        switch ((int)(s[i]))
        {
            case 48: w = (w << 4) | 0;
                break;
            case 49: w = (w << 4) | 1;
                break;
            case 50: w = (w << 4) | 2;
                break;
            case 51: w = (w << 4) | 3;
                break;
            case 52: w = (w << 4) | 4;
                break;
            case 53: w = (w << 4) | 5;
                break;
            case 54: w = (w << 4) | 6;
                break;
            case 55: w = (w << 4) | 7;
                break;
            case 56: w = (w << 4) | 8;
                break;
            case 57: w = (w << 4) | 9;
                break;
            case 65: w = (w << 4) | 10;
                break;
            case 66: w = (w << 4) | 11;
                break;
            case 67: w = (w << 4) | 12;
                break;
            case 68: w = (w << 4) | 13;
                break;
            case 69: w = (w << 4) | 14;
                break;
            case 70: w = (w << 4) | 15;
                break;
            default: i = s.length();
        }
    }

    return w;
}

int str2int(string s)
{
    long i,j,k;
    bool neg;
    neg = false;
    s = btrim(ucase(s));
    if (s == "")
    {
        k = 0;
    }
    else
    {
        if (s[1]='-')
        {
            neg = true;
            s = rstr(s,s.length()-1);
        }
        k = 0;
        if (lstr(s,2)=="0X")
        {
            k = hex2int(rstr(s,s.length()-2));
        }
        else if (rstr(s,1)=="H")
        {
            k = hex2int(lstr(s,s.length()-1));
        }
        else
        {
            k = value(s);
        }
        if (neg)
        {
            k = 0-k;
        }
    }
    return k;
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

string bin(int n)
{
    int i;
    string bin_string;

    bin_string = "";
    for (i = 0; i <= 15; i++)
    {
        if ((n % 2) == 0)
        {
            bin_string =  '0' + bin_string;
        }
        else
        {
            bin_string =  '1' + bin_string;
        }

        n = n/2;
    }
    return bin_string;
}

string decimal(int num,int length )
{
    string dec_string;
    int i;
    dec_string = "";
    for (i = 1; i <=length; i++)
    {
        dec_string = (char)((num % 10)+48) + dec_string;
        num = num / 10;
    }
    return dec_string;
}

int main(void)
{
    return 0;
}
