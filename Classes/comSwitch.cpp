#include "comSwitch.h"

using namespace std;


comSwitch::comSwitch(char path[])
{
    comp inFile(path);
    inCode = &inFile;
    init();
}


void comSwitch::init(void)
{

    for(line=inCode->getCommand();line[0]=='#';line=inCode->getCommand())
    {
        parseCommand();

        if (com1[0] == '#') //all the initialization commands start with a #
        {
            switch(com1[1]) //This is the part when I learned switches in c++ could only take ints. You could pinpoint the spot where my heart broke.
            {
            case 100: //D
            case 68: //DEF
                cout << "making variable named " << com2 << endl;
                break;
            case 99: //C
            case 67: //CONFIG
                cout << "assigning points: " << com2 << endl;
                break;
            case 109: //M
            case 77: //MSG
                cout << "displaying message: " << com2 << " " << com3 << endl;
                break;
            }
        }
    }
    //everything below this to the end of the function isn't necessary. It's either outdated, for my convenience, or used in debugging
    inCode->setLine(0);
    cout << "All set!" << endl;

    for (int i=0;i<10;i++)
    {
        nextCom(i);
    }

    cout << "\n\n\n\n";
    inCode->setLine(0);

    return;
} //---------------------------


void comSwitch::parseCommand(void)
{
    int temp=line.find(' '), length = line.length();

    com1 = line.substr(0, temp);
    com2 = line.substr(temp+1, length);

    if (com2.find(' ')!=string::npos)
    {
        temp = com2.find(' ');
        length = com2.length();

        com3 = com2.substr(temp+1, length);
        com2 = com2.substr(0, temp);
    }
}


void comSwitch::nextCom(int lNum)
{
    inCode->setLine(lNum);
    line = inCode->getCommand();
    parseCommand();

    switch(com1[0]) //for the record, this switch case is 292 lines long.
    {
    case 78: //N
    case 110: //NOP, NEG, NOT
        switch (com1[2])
        {
        case 80: //NOP
        case 112:
            cout << "nop" << endl;
            break;
        case 71: //NEG
        case 103:
            cout << "neg " << com2 << endl;
            cout << toInt(com2)*-1 << endl;
            break;
        case 84: //NOT
        case 116:
            cout << "not " << com2 << endl;
            break;
        default:
            errMsg(0);
        }
        break;
    case 65: //A
    case 97: //ADD, AND, ADDR
        switch (com1[1])
        {
        case 68: //ADD, ADDR
        case 100:
            if (com1[3] == 82 || com1[3] == 114) //ADDR
                cout << "addr " << com2 << " " << com3 << endl;
            else //ADD
            {
                cout << "add " << com2 << " " << com3 << endl;
                cout << toInt(com2)+toInt(com3) << endl;
            }
            break;
        case 78: //AND
        case 110:
            cout << "and " << com2 << " " << com3 << endl;
            break;
        default:
            errMsg(0);
        }
        break;
    case 83: //S
    case 115: //SUB, SHL, SHR, SAL, SAR
        switch (com1[2])
        {
        case 66: //SUB
        case 98:
            cout << "sub " << com2 << " " << com3 << endl;
            break;
        case 76: //SHL, SAL
        case 108: //both are the same
            //shL(com2[1], com2[2]);
            cout << "shl " << com2 << " " << com3 << endl;
            break;
        case 82: //SHR, SAR
        case 114: //shR() will split them up
            //shL(com2[1], com2[2], com1[1]);
            cout << "shr " << com2 << " " << com3 << endl;
            break;
        default:
            errMsg(0);
        }
        break;
    case 73: //I
    case 105: //INC, INT, IPO/IN
        switch (com1[2])
        {
        case 67: //INC
        case 99:
            cout << "inc " << com2 << endl;
            break;
        case 84: //INT
        case 116:
            cout << "int " << com2 << endl;
            break;
        default: //IPO/IN
            cout << "ipo " << com2 << " " << com3 << endl;
        }
        break;
    case 68: //D
    case 100: //DEC, DO, DEL, DIV
        switch (com1[2])
        {
        case 67: //DEC
        case 99:
            cout << "dec " << com2 << endl;
            break;
        case 76: //DEL
        case 108:
            cout << "del " << com2 << endl;
            break;
        case 86: //DIV
        case 118:
            cout << "div " << com2 << " " << com3 << endl;
            break;
        default: //DO
            cout << "do " << com2 << endl;
        }
        break;
    case 82: //R
    case 114: //ROL, ROR, RET
        switch (com1[2])
        {
        case 76: //ROL
        case 108:
            cout << "rol " << com2 << " " << com3 << endl;
            break;
        case 82: //ROR
        case 114:
            cout << "ror " << com2 << " " << com3 << endl;
            break;
        case 84: //RET
        case 116:
            cout << "ret" << endl;
            break;
        default:
            errMsg(0);
        }
        break;
    case 79: //O
    case 111: //OR, OPO/OUT
        switch (com1[1])
        {
        case 82: //OR
        case 114:
            cout << "or " << com2 << " " << com3 << endl;
            break;
        case 80: //OPO
        case 112:
        case 85: //OUT
        case 117:
            cout << "opo " << com2 << " " << com3 << endl;
            break;
        default:
            errMsg(0);
        }
        break;
    case 88: //X
    case 120: //XOR, XCHG
        switch (com1[1])
        {
        case 79: //XOR
        case 111:
            cout << "xor " << com2 << " " << com3 << endl;
            break;
        case 76: //XCHG
        case 99:
            cout << "xchg " << com2 << " " << com3 << endl;
            break;
        default:
            errMsg(0);
        }
        break;
    case 77: //M
    case 109: //MPY, MOV, MOD
        switch (com1[2])
        {
        case 89: //MPY
        case 121:
            cout << "mpy " << com2 << " " << com3 << endl;
            break;
        case 86: //MOV
        case 118:
            cout << "mov " << com2 << " " << com3 << endl;
            break;
        case 68: //MOD
        case 100:
            cout << "mod " << com2 << " " << com3 << endl;
            break;
        default:
            errMsg(0);
        }
        break;
    case 67: //C
    case 99: //CALL, CMP
        switch (com1[1])
        {
        case 65: //CALL
        case 97:
            cout << "call " << com2 << endl;
            break;
        case 77: //CMP
        case 109:
            cout << "cmp " << com2 << " " << com3 << endl;
            /*
            register0 = (com2 == com3);
            register1 = (com2 < com3);
            register2 = (com2 > com3);
            register3 = (com2 == 0 && com3 == 0);
            */
            break;
        default:
            errMsg(0);
        }
        break;
    case 74: //J
    case 106: //JMP, JLS, JGR, JNE, JE, JGE, JLE, JZ, JNZ, JTL
        switch (com1[1])
        {
        case 77: //JMP
        case 109:
        case 84: //JTL
        case 116:
            cout << "jmp " << com2 << endl;
            inCode->setLine(toInt(com2));
            break;
        case 76: //L
        case 108: //JLS, JLE
            /*
            if (register1 || (register 0 && (com1[2] == 'e' || com1[2] == 'E')))
                inCode->setLine(toInt(com2));
            */
            cout << "jls or jle " << com2 << endl;
            break;
        case 71: //G
        case 103: //JGR, JGE
            /*
            if (register2 || (register 0 && (com1[2] == 'e' || com1[2] == 'E')))
                inCode->setLine(toInt(com2));
            */
            cout << "jgr or jge " << com2 << endl;
            break;
        case 78: //N
        case 110: //JNE, JNZ
            /*
            if (!register0)
                inCode->setLine(com2);
            else if (register3 && (com1[2] == 'z' || com1[2] == 'Z'))
                inCode->setLine(toInt(com2));
            */
            cout << "jne " << com2 << endl;
            break;
        case 69: //E
        case 101: //JE
            /*
            if (register0)
                inCode->setLine(toInt(com2));
            */
            cout << "jz " << com2 << endl;
            break;
        default:
            errMsg(0);
        }
        break;
    case 76: //L
    case 108: //LOOP
        cout << "loop " << com2 << endl;
        break;
    case 84: //T
    case 116: //Test
        cout << "test " << com2 << endl;
        break;
    case 71: //G
    case 103: //GET
        cout << "get " << com2 << " " << com3 << endl;
        break;
    case 80: //P
    case 112: //PUT, PUSH, POP
        switch (com1[2])
        {
        case 84: //PUT
        case 116:
            cout << "put " << com2 << " " << com3 << endl;
            break;
        case 83: //PUSH
        case 115:
            cout << "push " << com2 << endl;
            break;
        case 80: //POP
        case 103:
            cout << "pop " << com2 << endl;
            break;
        default:
            errMsg(0);
        }
        break;
    case 69: //E
    case 101: //ERR
        cout << "err " << com2 << endl;
        break;
    case 35: //#
    case 33: // !
    case 58: //:
        cout << "#:!" << endl;
        nextCom(lNum+1);
        break;
    default:
        errMsg(0);
    }

    return;
}

void comSwitch::errMsg (int er)
{
    cout << "Error: ";

    switch (er)
    {
    case 0:
        cout << "Invalid command: " << com1 << endl;
        break;
    default:
        cout <<  "We don't even know what you did wrong" << endl;
    }
}

int comSwitch::toInt (string in)
{
    int ans = 0, length = in.length(), i=length-1, base=1;

    for (i=length-1;i!=-1;i--)
    {
        if (in[i] >= 48 && in[i] <= 57)
        {
            ans += (in[i]-48)*base;
            base *= 10;
        }
    }

    return ans;
}

void comSwitch::debug(string in)
{
    cout << "\n\n\n ENTERING DEBUG \n";
    int i=0, length = in.length();

    unsigned int temp;

    for (i=0;i<length;i++)
    {
        temp = in[i];
        cout << temp << " ";
    }

    cout << "\n LEAVING DEBUG \n\n\n";
}
