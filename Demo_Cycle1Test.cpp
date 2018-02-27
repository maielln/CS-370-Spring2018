#include <string>
#include <iostream>
#include <array>

#include "Robot.h"
#include "Missile.h"
#include "fOpen.h"

int main(void)
{
  Robot Sitting_Duck("Quakers");
  Robot Sitting_Shooter("Noob");
  char SSPath[] = "C:/Users/ShadowFoxNikki/Desktop/Shooter.at2";
  fOpen SSFile;
  int j = 0;
  SSFile.openFile(SSPath);
  int allMissiles = 0;
  int fileSize = SSFile.getBufferSize();
  int *file = SSFile.getBuffer(); //we will read in a file that only shoots

  Missile Active_missiles[20];

  for(int i = 0; i < fileSize; i++)
  {
    if(*file == '\n') //since I know this robot only shoots, every new line will be one missile that is generated
      j++;
    file++;
  }

  Sitting_Duck.robot_config(1, 5, 1, 1, 1, 1, 1);
  Sitting_Shooter.robot_config(1, 5, 1, 1, 1, 1, 1); //testing configuration

  Sitting_Duck.set_coordinates(0, 0);
  Sitting_Shooter.set_coordinates(10, 10); //defaut coordinates for now, overrrides randomized coordinates

  for(i = 0; i < j; i++)  //decided to just create all missiles at once without creating a stack, that's for later
  {
    Active_missiles[i] = new Missile();
    Active_missiles[i].missileInit();
    Active_missiles[i].damage(Sitting_Shooter.shotstrength);
    Active_missiles[i].shoot(1);
    allMissiles++;
    cout<<"Robot "<<Sitting_Shooter.Name<<" has fired a missile!"<<endl;
  }

  for(i = 0; Sitting_Duck.health == 0; i++) //since all missiles are guarenteed to hit, every active missile gets updated
  {
    for(int w = 0; w < allMissiles; w++) //all missiles positions get updated
    {
      Active_missiles[w].x++;
      Active_missiles[w].y++;
    }
    for(int u = 0; u < allMissiles; u++) //checks every missile if they hit
    {
      if(Active_missiles[i].hitD(0,Sitting_Duck.x, Sitting_Duck.y))
      {
        Sitting_Duck -= Active_missiles[i].getDam();
        delete Active_missiles[i];
        Sitting_Shooter.hits++;
      }
    }
  }
  cout<<"Robot "<<Sitting_Shooter.Name<<" has killed "<<Sitting_Duck.Name<<"."<<endl;
  Sitting_Duck.deaths++;
  Sitting_Shooter.kills++;
}
