  found = false;
  opcode = -1;
  microcode = -1;

  //instructions
  if (s == "NOP")
  {
    opcode = 0;
    found = true;
  }
  else if(s == ("ADD"))
  {
    opcode = 1;
    found = true;
  }
  else if(s == ("SUB"))
  {
    opcode = 2;
    found = true;
  }
  else if(s == ("OR"))
  {
    opcode = 3;
    found = true;
  }
  else if(s == ("AND"))
  {
    opcode = 4;
    found = true;
  }
  else if(s == ("XOR"))
  {
    opcode = 5;
    found = true;
  }
  else if(s == ("NOT"))
  {
    opcode = 6;
    found = true;
  }
  else if(s == ("MPY"))
  {
    opcode = 7;
    found = true;
  }
  else if(s == ("DIV"))
  {
    opcode = 8;
    found = true;
  }
  else if(s == ("MOD"))
  {
    opcode = 9;
    found = true;
  }
  else if(s == ("RET") || s == ("RETURN"))
  {
    opcode = 10;
    found = true;
  }
  else if(s == ("GSB") || s == ("GOSUB") || s == ("CALL"))
  {
    opcode = 11;
    found = true;
  }
  else if(s == ("JMP") || s == ("JUMP") || s == ("GOTO"))
  {
    opcode = 12;
    found = true;
  }
  else if(s == ("JLS") || s == ("JB"))
  {
    opcode = 13;
    found = true;
  }
  else if(s == ("JGR") || s == ("JA"))
  {
    opcode = 14;
    found = true;
  }
  else if(s == ("JNE"))
  {
    opcode = 15;
    found = true;
  }
  else if(s == ("JEQ") || s == ("JE"))
  {
    opcode = 16;
    found = true;
  }
  else if(s == ("XCHG") || s == ("SWAP"))
  {
    opcode = 17;
    found = true;
  }
  else if(s == ("DO"))
  {
    opcode = 18;
    found = true;
  }
  else if(s == ("LOOP"))
  {
    opcode = 19;
    found = true;
  }
  else if(s == ("CMP"))
  {
    opcode = 20;
    found = true;
  }
  else if(s == ("TEST"))
  {
    opcode = 21;
    found = true;
  }
  else if(s == ("SET") || s == ("MOV"))
  {
    opcode = 22;
    found = true;
  }
  else if(s == ("LOC") || s == ("ADDR"))
  {
    opcode = 23;
    found = true;
  }
  else if(s == ("GET"))
  {
    opcode = 24;
    found = true;
  }
  else if(s == ("PUT"))
  {
    opcode = 25;
    found = true;
  }
  else if(s == ("INT"))
  {
    opcode = 26;
    found = true;
  }
  else if(s == ("IPO") || s == ("IN"))
  {
    opcode = 27;
    found = true;
  }
  else if(s == ("OPO") || s == ("OUT"))
  {
    opcode = 28;
    found = true;
  }
  else if(s == ("DEL") || s == ("DELAY"))
  {
    opcode = 29;
    found = true;
  }
  else if(s == ("PUSH"))
  {
    opcode = 30;
    found = true;
  }
  else if(s == ("POP"))
  {
    opcode = 31;
    found = true;
  }
  else if(s == ("ERR") || s == ("ERROR"))
  {
    opcode = 32;
    found = true;
  }
  else if(s == ("INC"))
  {
    opcode = 33;
    found = true;
  }
  else if(s == ("DEC"))
  {
    opcode = 34;
    found = true;
  }
  else if(s == ("SHL"))
  {
    opcode = 35;
    found = true;
  }
  else if(s == ("SHR"))
  {
    opcode = 36;
    found = true;
  }
  else if(s == ("ROL"))
  {
    opcode = 37;
    found = true;
  }
  else if(s == ("ROR"))
  {
    opcode = 38;
    found = true;
  }
  else if(s == ("JZ"))
  {
    opcode = 39;
    found = true;
  }
  else if(s == ("JNZ"))
  {
    opcode = 40;
    found = true;
  }
  else if(s == ("JAE") || s == ("JGE"))
  {
    opcode = 41;
    found = true;
  }
  else if(s == ("JLE") || s == ("JBE"))
  {
    opcode = 42;
    found = true;
  }
  else if(s == ("SAL"))
  {
    opcode = 43;
    found = true;
  }
  else if(s == ("SAR"))
  {
    opcode = 44;
    found = true;
  }
  else if(s == ("NEG"))
  {
    opcode = 45;
    found = true;
  }
  else if(s == ("JTL"))
  {
    opcode = 46;
    found = true;
  }
  else
  {
    //error proofing
  }

  //registers
  // can check if the code was already found
  if(!found)
  {
    if(s == ("COLCNT"))
    {
      opcode = 8;
      microcode = 1;
      found = true;
    }
    else if(s == ("METERS"))
    {
      opcode = 9;
      microcode = 1;
      found = true;
    }
    else if(s == ("COMBASE"))
    {
      opcode = 10;
      microcode = 1;
      found = true;
    }
    else if(s == ("COMEND"))
    {
      opcode = 11;
      microcode = 1;
      found = true;
    }
    else if(s == ("FLAGS"))
    {
      opcode = 64;
      microcode = 1;
      found = true;
    }
    else if(s == ("AX"))
    {
      opcode = 65;
      microcode = 1;
      found = true;
    }
    else if(s == ("BX"))
    {
      opcode = 66;
      microcode = 1;
      found = true;
    }
    else if(s == ("CX"))
    {
      opcode = 67;
      microcode = 1;
      found = true;
    }
    else if(s == ("DX"))
    {
      opcode = 68;
      microcode = 1;
      found = true;
    }
    else if(s == ("EX"))
    {
      opcode = 69;
      microcode = 1;
      found = true;
    }
    else if(s == ("FX"))
    {
      opcode = 70;
      microcode = 1;
      found = true;
    }
    else if(s == ("SP"))
    {
      opcode = 71;
      microcode = 1;
    }
    else
    {
      //error proofing - if needed
    }
  }

  //constants
  if(!found)
  {
    if(s == ("MAXINT"))
    {
      opcode = 32767;
      microcode = 0;
      found = true;
    }
    else if(s == ("MININT"))
    {
      opcode = 32768;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_SPEDOMETER"))
    {
      opcode = 1;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_HEAT"))
    {
      opcode = 2;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_COMPASS"))
    {
      opcode = 3;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_TANGLE") || s == ("P_TURRET_OFS"))
    {
      opcode = 4;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_THEADING") || s == ("P_TURRET_ABS"))
    {
      opcode = 5;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_ARMOR") || s == ("P_DAMAGE"))
    {
      opcode = 6;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_SCAN"))
    {
      opcode = 7;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_ACCURACY"))
    {
      opcode = 8;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_RADAR"))
    {
      opcode = 9;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_RANDOM") || s == ("P_RAND"))
    {
      opcode = 10;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_THROTTLE"))
    {
      opcode = 11;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_TROTATE") || s == ("P_OFS_TURRET"))
    {
      opcode = 12;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_TAIM") || s == ("P_ABS_TURRET"))
    {
      opcode = 13;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_STEERING"))
    {
      opcode = 14;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_WEAP") || s == ("P_WEAPON") || s == ("P_FIRE"))
    {
      opcode = 15;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_SONAR"))
    {
      opcode = 16;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_ARC") || s == ("P_SCANARC"))
    {
      opcode = 17;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_OVERBURN"))
    {
      opcode = 18;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_TRANSPONDER"))
    {
      opcode = 19;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_SHUTDOWN"))
    {
      opcode = 20;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_CHANNEL"))
    {
      opcode = 21;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_MINELAYER"))
    {
      opcode = 22;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_MINETRIGGER"))
    {
      opcode = 23;
      microcode = 0;
      found = true;
    }
    else if(s == ("P_SHIELD") || s == ("P_SHIELDS"))
    {
      opcode = 24;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_DESTRUCT"))
    {
      opcode = 0;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_RESET"))
    {
      opcode = 1;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_LOCATE"))
    {
      opcode = 2;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_KEEPSHIFT"))
    {
      opcode = 3;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_OVERBURN"))
    {
      opcode = 4;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_ID"))
    {
      opcode = 5;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_TIMER"))
    {
      opcode = 6;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_ANGLE"))
    {
      opcode = 7;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_TID") || s == ("I_TARGETID"))
    {
      opcode = 8;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_TINFO") || s == ("I_TARGETINFO"))
    {
      opcode = 9;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_GINFO") || s == ("I_GAMEINFO"))
    {
      opcode = 10;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_RINFO") || s == ("I_ROBOTINFO"))
    {
      opcode = 11;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_COLLISIONS"))
    {
      opcode = 12;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_RESETCOLCNT"))
    {
      opcode = 13;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_TRANSMIT"))
    {
      opcode = 14;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_RECEIVE"))
    {
      opcode = 15;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_DATAREADY"))
    {
      opcode = 16;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_CLEARCOM"))
    {
      opcode = 17;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_KILLS") || s == ("I_DEATHS"))
    {
      opcode = 18;
      microcode = 0;
      found = true;
    }
    else if(s == ("I_CLEARMETERS"))
    {
      opcode = 19;
      microcode = 0;
      found = true;
    }
  }
