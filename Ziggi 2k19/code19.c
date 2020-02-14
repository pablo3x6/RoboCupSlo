////////////////////////////////////////
//
//	File : ai.c
//	CoSpace Robot
//	Version 1.0.0
//	Jan 1 2016
//	Copyright (C) 2016 CoSpace Robot. All Rights Reserved
//
//////////////////////////////////////
//
// ONLY C Code can be compiled.
//
/////////////////////////////////////

#define CsBot_AI_H//DO NOT delete this line
#ifndef CSBOT_REAL
#include <windows.h>
#include <stdio.h>
#include <math.h>
#define DLL_EXPORT extern __declspec(dllexport)
#define false 0
#define true 1
#endif//The robot ID : It must be two char, such as '00','kl' or 'Cr'.
char AI_MyID[2] = {'6', '9'};

int Duration = 0;
int SuperDuration = 0;
int bGameEnd = false;
int CurAction = 0;
int CurGame = 0;
int SuperObj_Num = 0;
int SuperObj_X = 0;
int SuperObj_Y = 0;
int Teleport = 0;
int LoadedObjects = 0;
int CurObjects = 0;
int US_Front = 0;
int US_Left = 0;
int US_Right = 0;
int CSLeft_R = 0;
int CSLeft_G = 0;
int CSLeft_B = 0;
int CSRight_R = 0;
int CSRight_G = 0;
int CSRight_B = 0;
int PositionX = 0;
int PositionY = 0;
int TM_State = 0;
int Compass = 0;
int Time = 0;
int WheelLeft = 0;
int WheelRight = 0;
int LED_1 = 0;
int MyState = 0;
int AI_SensorNum = 13;
//Our vars
int Durat = 0;
int searchThing = 0;
int Angle = 0;
int onTrap = 0;
int onBorder = 0;
int isUltrasonic = 0;
int Area = 0;
int superObjectBool = 0;
int onDeposit = false;
int pickedUp[4] = {0, 0, 0, 0}; // red, cyan, black, PRPL
int forceLorR = -1;
int borderUp = 250;
int borderDown = 10;
int borderLeft = 10;
int borderRight = 350;
int searchFor = 0;
int goingX = 0;
int goingY = 0;
int searching = 0;
int Timer = 0;
int USTrack = 0;
int timeout = 0;

/*////////////////////////////////////////////////

      cOSpAcE sImuLaToR jE aMaZinG aND wE lOVe iT
Ta koda je mešanica med depresijo, monsterom in CBD čajem ;)
  Še sama nimava pojma kaj se dogaja tu, ampak deluje :D

--------------------------------------------------

                       TODO
(X) pointi at some point (bolj accurate pointi)
(X) more accurate areas
??? Timer za failed area 
(X) hitrejši swamp 
(X) borderi 
(Y) vecja area za red midmid 
(X) BLuBluLeft indepndence day
!!! morma tudi uporabit topleft (timer or boolean)
(X) počasneje s super objectom al pa več ko 4 objecte 
(X) zadnih 30s gre na deposit če ma ka pri sebi
(W) WORLD 1 NEEDS SOME LOVE MAN!!!!!!!!
(X) prvo gre iz blk na mid pa pol na blu
(X) funkcija za steno
??? smaller ova giant stvar veš ke mislim

--------------------------------------------------
                      
                      LEGENDA
(X) -> Porihtano
(Y) -> Probala, nea dela vredi, might try again
(Z) -> Zbe is working on it
(S) -> Širo is working on it
(W) -> WTF IZ GOING ON HERE
!!! -> Obvezno razmislit
??? -> Nevema kak bi izpeljala (na nek simple način)
/// -> Praznina

////////////////////////////////////////////////*/

#define CsBot_AI_C//DO NOT delete this line

DLL_EXPORT void SetGameID(int GameID)
{
    CurGame = GameID;
    bGameEnd = 0;
}

DLL_EXPORT int GetGameID()
{
    return CurGame;
}

//Only Used by CsBot Dance Platform
DLL_EXPORT int IsGameEnd()
{
    return bGameEnd;
}

#ifndef CSBOT_REAL

DLL_EXPORT char* GetDebugInfo()
{
  char info[1024];
  sprintf(info, "Timeout=%d;Duration=%d;goingX=%d;goingY=%d;Area=%d;searchFor=%d;Angle=%d;SearchBuffer=%d;superObjectBool=%d;Action=%d;Load=%d;Red=%d;Cyan=%d;Black=%d;Purple=%d;borderUp=%d;borderDown=%d;borderLeft=%d;borderRight=%d;WheelL=%d;WheelR=%d;", timeout, Durat, goingX, goingY, Area, searchFor, Angle, searchThing, superObjectBool, CurAction, LoadedObjects, pickedUp[0], pickedUp[1], pickedUp[2], pickedUp[3], borderUp, borderDown, borderLeft, borderRight, WheelLeft, WheelRight);
  return info;
}

DLL_EXPORT char* GetTeamName()
{
     return "S3R5 Ziggi";
}

DLL_EXPORT int GetCurAction()
{
    return CurAction;
}

//Only Used by CsBot Rescue Platform
DLL_EXPORT int GetTeleport()
{
    return Teleport;
}

//Only Used by CsBot Rescue Platform
DLL_EXPORT void SetSuperObj(int X, int Y, int num)
{
    SuperObj_X = X;
    SuperObj_Y = Y;
    SuperObj_Num = num;
}
//Only Used by CsBot Rescue Platform
DLL_EXPORT void GetSuperObj(int *X, int *Y, int *num)
{
    *X = SuperObj_X;
    *Y = SuperObj_Y;
    *num = SuperObj_Num;
}

#endif ////CSBOT_REAL

DLL_EXPORT void SetDataAI(volatile int* packet, volatile int *AI_IN)
{

    int sum = 0;

    US_Front = AI_IN[0]; packet[0] = US_Front; sum += US_Front;
    US_Left = AI_IN[1]; packet[1] = US_Left; sum += US_Left;
    US_Right = AI_IN[2]; packet[2] = US_Right; sum += US_Right;
    CSLeft_R = AI_IN[3]; packet[3] = CSLeft_R; sum += CSLeft_R;
    CSLeft_G = AI_IN[4]; packet[4] = CSLeft_G; sum += CSLeft_G;
    CSLeft_B = AI_IN[5]; packet[5] = CSLeft_B; sum += CSLeft_B;
    CSRight_R = AI_IN[6]; packet[6] = CSRight_R; sum += CSRight_R;
    CSRight_G = AI_IN[7]; packet[7] = CSRight_G; sum += CSRight_G;
    CSRight_B = AI_IN[8]; packet[8] = CSRight_B; sum += CSRight_B;
    PositionX = AI_IN[9]; packet[9] = PositionX; sum += PositionX;
    PositionY = AI_IN[10]; packet[10] = PositionY; sum += PositionY;
    TM_State = AI_IN[11]; packet[11] = TM_State; sum += TM_State;
    Compass = AI_IN[12]; packet[12] = Compass; sum += Compass;
    Time = AI_IN[13]; packet[13] = Time; sum += Time;
    packet[14] = sum;

}
DLL_EXPORT void GetCommand(int *AI_OUT)
{
    AI_OUT[0] = WheelLeft;
    AI_OUT[1] = WheelRight;
    AI_OUT[2] = LED_1;
    AI_OUT[3] = MyState;
}
/***************************************************************************************************************/
/*
16t == 1s
*/

struct colors {
  int miR;
  int miG;
  int miB;
  int maR;
  int maG;
  int maB;
};

struct areas {
  int startX;
  int startY;
  int endX;
  int endY;
};

struct points {
  int x;
  int y;
};

struct colors red = {200, 0, 0, 255, 60, 60};
struct colors cyan = {0, 220, 220, 40, 255, 255};
struct colors black = {0, 0, 0, 60, 60, 60};
struct colors yellow = {190, 190, 0, 255, 255, 50};
struct colors orange = {200, 150, 0, 240, 200, 20};
struct colors blue = {20, 20, 220, 60, 60, 255};
struct colors gray = {130, 140, 185, 165, 165, 210};
struct colors purple = {230, 0, 230, 255, 50, 255};
struct colors purple2 = {190, 0, 200, 220, 50, 255};

//                         sx  sy   ex   ey
struct areas AreaDepDL1 = {0, 85, 35, 45};
struct areas AreaDepDL2 = {45, 95, 65, 75};
struct areas AreaDepDL3 = {45, 170, 105, 100};
struct areas AreaMidL = {110, 210, 180, 100};
struct areas AreaTopL = {0, 270, 110, 165};
struct areas AreaBluBlu1 = {110, 255, 165, 205};
struct areas AreaBluBlu2 = {165, 255, 235, 210};
struct areas AreaBluBlk = {140, 55, 215, 10};
struct areas AreaDownL = {105, 55, 140, 0};
struct areas AreaMidMid = {180, 210, 230, 80};
struct areas AreaTopR = {300, 255, 360, 240};
struct areas AreaMidR = {230, 130, 330, 80};
struct areas AreaDepDR2 = {230, 80, 330, 25};
struct areas AreaDepDR1 = {330, 80, 360, 25};
struct areas AreaDownR = {230, 25, 300, 0};

struct areas SuperArea = {0, 0, 0, 0};
struct areas SuperAreaWide = {0, 0, 0, 0};

struct points superObject = {0, 0};
struct points PointDepDL1 = {15, 80};
struct points PointDepDL2 = {55, 80};
struct points PointDepDL3 = {70, 100};
struct points PointTopL = {50, 220};
struct points PointBluBlu1 = {140, 220};
struct points PointBluBlu2 = {220, 255};
struct points PointBluBlk = {155, 20};
struct points PointDownL = {125, 40};
struct points PointDepDL = {20, 20};
struct points PointMidL = {150, 150};
struct points PointMidMid = {205, 125};
struct points PointMidR = {280, 115};
struct points PointDownR = {265, 10};
struct points PointDepDR2 = {280, 70};
struct points PointDepDR1 = {345, 60};
struct points PointDepDR = {345, 10};

int onSwamp() {
  if(isColL(gray) || isColR(gray))
    return true;
  else
    return false;
}

void Drive(int left, int right) {
  if (onSwamp()) {
    WheelLeft = left * 2;
    WheelRight = right * 2;
  }else{
    WheelLeft = left;
    WheelRight = right;
  } 
}

int isColR(struct colors color) {
  if (CSRight_R >= color.miR && CSRight_R <= color.maR && CSRight_G >= color.miG && CSRight_G <= color.maG && CSRight_B >= color.miB && CSRight_B <= color.maB)
    return true;
  else
    return false;
}

int isColL(struct colors color) {
  if (CSLeft_R >= color.miR && CSLeft_R <= color.maR && CSLeft_G >= color.miG && CSLeft_G <= color.maG && CSLeft_B >= color.miB && CSLeft_B <= color.maB)
    return true;
  else
    return false;
}

//Zračuna okolico SuperObjecta in da v struct 
void calcSuperArea() {
  //Manjši super area (SuperArea)
  SuperArea.startX = SuperObj_X - 5;
  SuperArea.endX = SuperObj_X + 5;
  SuperArea.startY = SuperObj_Y + 5;
  SuperArea.endY = SuperObj_Y - 5;

  //večji super area (SuperAreaWide)
  SuperAreaWide.startX = SuperObj_X - 25;
  SuperAreaWide.endX = SuperObj_X + 25;
  SuperAreaWide.startY = SuperObj_Y + 25;
  SuperAreaWide.endY = SuperObj_Y - 25;
}

//0 is left 1 is right
int leftOrRight() {
  if (forceLorR != -1) {
    return forceLorR;
  }
  if (Angle < Compass){
    if ((Compass - Angle) < 180) {
      return 1;
    } else return 0;
  }

  if (Angle > Compass){
    if ((Angle - Compass) < 180) {
      return 0;
    } else return 1;
  }
}

void setAction(int action, double dur) {
  dur = dur * 16;
  Durat = (int)dur;
  CurAction = action;
}

void whereBeWalls() {
  if (US_Front < 10) {
    searchThing = 8;
    if (US_Left <= US_Right){
      Drive(1, -1);
    } else Drive(-1, 1);
  }

  if (US_Right < 10) {
    if (superObjectBool && checkArea(SuperArea)){
      forceLorR = 0;
    } else {
      forceLorR = -1;
    }
    searchThing = 8;
    Drive(1, 2);
  }

  if (US_Left < 10) {
    if (superObjectBool && checkArea(SuperArea)) {
      forceLorR = 1;
    } else {
      forceLorR = -1;
    }
    searchThing = 8;
    if(onSwamp())
      Drive(2, -2);
    else
      Drive(2, 1);
  }

  if (US_Left < 7)
    Drive(1, -1);

  if (US_Right < 7)
    Drive(-1, 1);

  if ((US_Front+US_Left+US_Right) <= 20) {
    searchThing = 8;
    if(onBorder == 0 && onTrap == false)
    setAction(3, 0.5);
  }
}

void calcAngle(struct points point) {
  goingX = point.x;
  goingY = point.y;

  if (PositionX != 0 && PositionY != 0) {
    if (point.y > PositionY) {
      if (point.x > PositionX)
        Angle = (atan2(point.y - PositionY, point.x - PositionX) * 180 / 3.14) + 270;
      else
        Angle = atan2(PositionX - point.x, point.y - PositionY) * 180 / 3.14;
    } else {
      if (point.x > PositionX)
        Angle = (atan2(point.x - PositionX, PositionY - point.y) * 180 / 3.14) + 180;
      else
        Angle = (atan2(PositionY - point.y, PositionX - point.x) * 180 / 3.14) + 90;
    }
  }
}

// red, cyan, black, PRPL
void whatAmISearchingFor(){
  if((LoadedObjects >= 6) || pickedUp[3] > 0 || (Time >= 450 && LoadedObjects != 0)) {
    searchFor = 10; // DEPOSIT
  } else if (pickedUp[1] < 2) {
    searchFor = 1; // CYAN
  } else if (pickedUp[0] < 2) {
    searchFor = 0; // RED
  } else if (pickedUp[2] < 2) {
    searchFor = 2; // BLACK
  } else {
    searchFor = 10;
  }
  
}

//nastavi border nazaj na deafult
void setBorderDeafult() {
  borderUp = 250;
  borderDown = 10;
  borderLeft = 10;
  borderRight = 350;
}

void setBorder(struct areas area) {
  borderUp = area.startY;
  borderDown = area.endY;
  borderLeft = area.startX;
  borderRight = area.endX;



  if (checkArea(AreaBluBlu1) || checkArea(AreaBluBlu2) || checkArea(AreaTopL)) {
    borderRight = 230;
  }

  if (checkArea(AreaMidMid)){
    borderLeft = 140;
  }
}



//nastavi onBorder glede na to na kerem borderu je
void isOnBorder() {
  //1 is top, 2 is left, 3 is down, 4 is right
  if (PositionY >= borderUp)
    onBorder = 1;
  else if (PositionX <= borderLeft && PositionX != 0)
    onBorder = 2;
  else if (PositionY <= borderDown && PositionY != 0)
    onBorder = 3;
  else if (PositionX >= borderRight)
    onBorder = 4;
}



//Obrne se proti sredini, ko prije do bordera (plac rabi spredi tuji)
void whereBeBorders() {
  isOnBorder();
  if (onBorder > 0) {
    searchThing = 6;
    switch(onBorder) {
      case 1:
        if (Compass > 90 && Compass < 270 && US_Front > 30 && US_Left > 15 && US_Right > 15 && !isColL(yellow) && !isColR(yellow))
          onBorder = 0;
        else 
          Drive(-1, 1);
        break;
      case 2:
        if (Compass > 180 && Compass < 360 && US_Front > 30 && US_Left > 15 && US_Right > 15 && !isColL(yellow) && !isColR(yellow))
          onBorder = 0;
        else 
          Drive(-1,1);
        break;
      case 3:
        if ((Compass < 90 || Compass > 270) && US_Front > 30 && US_Left > 15 && US_Right > 15 && !isColL(yellow) && !isColR(yellow))
          onBorder = 0;
        else Drive(-1, 1);
        break;
      case 4:
        if (Compass < 180 && US_Front > 30 && US_Left > 15 && US_Right > 15 && !isColL(yellow) && !isColR(yellow))
          onBorder = 0;
        else Drive(-1, 1);
        break;
    }
  }
}



//FIND AREA
void calcArea() {
    if (checkArea(AreaTopL)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointDepDL3);
          setBorderDeafult();
          break;
        case 0:
          calcAngle(PointDepDL3);
          setBorderDeafult();
          break;
        case 1:
          //nic nea nareji
          break;
        case 2:
          calcAngle(PointMidL);
          setBorderDeafult();
          break;
      }
      Area = 1;
      if(searchFor == 1){
          setBorder(AreaBluBlu1);
          Area = 0;
      }
    }
    else if (checkArea(AreaMidL)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointDepDL3);
          setBorderDeafult();
          break;
        case 0:
          calcAngle(PointMidMid);
          break;
        case 1:
          calcAngle(PointBluBlu1);
          setBorderDeafult();
          break;
        case 2:
          calcAngle(PointDownL); 
          setBorderDeafult();
          break;
      }
      Area = 2;
    }
    else if (checkArea(AreaDepDL3)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointDepDL2);
          break;
        case 0:
          calcAngle(PointMidL);
          break;
        case 1:
          calcAngle(PointMidL);
          break;
        case 2:
          calcAngle(PointMidL);
          break;
      }
      Area = 3;
    }
    else if (checkArea(AreaDepDL2)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointDepDL1);
          break;
        case 0:
          calcAngle(PointDepDL3);
          break;
        case 1:
          calcAngle(PointDepDL3);
          break;
        case 2:
          calcAngle(PointDownL);
          break;
      }
      Area = 4;
    }
    else if (checkArea(AreaDepDL1)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointDepDL);
          break;
        case 0:
          calcAngle(PointDepDL2);
          break;
        case 1:
          calcAngle(PointDepDL2);
          break;
        case 2:
          calcAngle(PointDepDL2);
          break;
      }
      Area = 5;
    }
    else if (checkArea(AreaDownL)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointMidL);
          break;
        case 0:
          calcAngle(PointMidMid);
          break;
        case 1:
          calcAngle(PointMidL);
          break;
        case 2:
          calcAngle(PointBluBlk);
          break;
      }
      Area = 6;
    }
    else if (checkArea(AreaBluBlk)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointDownR);
          setBorderDeafult();
          break;
        case 0:
          calcAngle(PointDownL);
          setBorderDeafult();
          break;
        case 1:
          calcAngle(PointDownL);
          setBorderDeafult();
          break;
        case 2:
          // NIKAM NEA GRE 
          break;
      }
      Area = 7;
      if(searchFor == 2){
          setBorder(AreaBluBlk);
          Area = 0;
      }
    }
    else if (checkArea(AreaDownR)) {
      switch(searchFor) {
        
        case 10:
          calcAngle(PointDepDR2);
          break;
        case 0:
          calcAngle(PointMidR);
          break;
        case 1:
          calcAngle(PointDepDR2);
          break;
        case 2:
          calcAngle(PointBluBlk);
          break;
      }
      Area = 8;
    }
    else if (checkArea(AreaDepDR2)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointDepDR1);
          break;
        case 0:
          // NIČ NEA DELA
          break;
        case 1:
          calcAngle(PointMidR);
          break;
        case 2:
          calcAngle(PointDownR);
          break;
      }
      Area = 9;
    }
    else if (checkArea(AreaDepDR1)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointDepDR);
          break;
        case 0:
          calcAngle(PointMidR);
          break;
        case 1:
          calcAngle(PointDepDR2);
          break;
        case 2:
          calcAngle(PointDepDR2);
          break;
      }
      Area = 10;
    }
    else if (checkArea(AreaMidMid)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointMidR);
          setBorderDeafult();
          break;
        case 0:
          //nikam ne gre
          break;
        case 1:
          calcAngle(PointMidL);
          setBorderDeafult();
          break;
        case 2:
          calcAngle(PointMidL);
          setBorderDeafult();
          break;
      }
      Area = 11;
      if(searchFor == 0) {
          setBorder(AreaMidMid);
          Area = 0;
      }
    }
    else if (checkArea(AreaMidR)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointDepDR2);
          break;
        case 0:
          calcAngle(PointMidMid);
          break;
        case 1:
          calcAngle(PointBluBlu1);
          break;
        case 2:
          calcAngle(PointDownL);
          break;
      }
      Area = 12;
    }
    else if (checkArea(AreaBluBlu1)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointTopL);
          setBorderDeafult();
          break;
        case 0:
          calcAngle(PointMidL);
          setBorderDeafult();
          break;
        case 1:
          // NIKAM TI PRAVIM STARI
          break;
        case 2:
          calcAngle(PointMidL);
          setBorderDeafult();
          break;
      }
      Area = 13;
      if(searchFor == 1) {
          setBorder(AreaBluBlu1);
          Area = 0;
      }
    }
    else if (checkArea(AreaTopR)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointBluBlu2);
          break;
        case 0:
          calcAngle(PointBluBlu2);
          break;
        case 1:
          calcAngle(PointBluBlu2);
          break;
        case 2:
          calcAngle(PointBluBlu2);
          break;
      }
      Area = 14;
    }
    else if (checkArea(AreaBluBlu2)) {
      switch(searchFor) {
        case 10:
          calcAngle(PointBluBlu1);
          setBorderDeafult();
          break;
        case 0:
          calcAngle(PointBluBlu1);
          setBorderDeafult();
          break;
        case 1:
          //nea nareji nic
          break;
        case 2:
          calcAngle(PointBluBlu1);
          setBorderDeafult();
          break;
      }
      Area = 14;
      if(searchFor == 1) {
          setBorder(AreaBluBlu1);
          Area = 0;
      }
    }
    else
      Area = 0;
}


int shouldISearch() {

  if (searchThing == 0 && PositionX != 0 && PositionY != 0 && CurAction == 0){
    if ((LoadedObjects >= 4 || pickedUp[3] > 0 || superObjectBool == 1 || searchFor!= 10) && Area!= 0)
      return true;
    else
      return false;
  } 
  else
    return false;
}

//Preveri, če je pickup pametna ideja (da še ma plac za superobject, če obstaja)
int shouldIPickUp() {

  if(LoadedObjects>=6)
    return false;

  if(searchFor == 10)
    return false;
  
/* if(CurGame == 0){
  if(pickedUp[2] >= 4 && (isColL(black) || isColR(black)))
    return false;

  if(pickedUp[1] >= 4 && (isColL(cyan) || isColR(cyan)))
    return false;

  if(pickedUp[0] >= 3 && (isColL(red) || isColR(red)))
    return false;
  
  return true;
  }*/

  if(pickedUp[2] >= 2 && (isColL(black) || isColR(black)))
    return false;

  if(pickedUp[1] >= 2 && (isColL(cyan) || isColR(cyan)))
    return false;

  if(pickedUp[0] >= 2 && (isColL(red) || isColR(red)))
    return false;

  if (isColL(purple) || isColR(purple) || isColL(purple2) || isColR(purple2))
    return true;
  if(superObjectBool != 1 && LoadedObjects < 6){
    return true;
  } else if(LoadedObjects < 5 && superObjectBool){
    return true;
  } else return false;
}

int checkArea(struct areas area)
{
  if ((PositionX > area.startX && PositionX < area.endX) && (PositionY < area.startY && PositionY > area.endY))
    return true;
  else
    return false;
}

void goToDepositW1() {
  if (LoadedObjects >= 3 && ((Compass >= 0 && Compass <= 80) || (Compass <= 359 && Compass >= 320)) && US_Left < 20) {
    USTrack = true;
    if (US_Left < 10)
      Drive(3, 1);
    else if (US_Left > 15)
      Drive(1, 3);
    
    if (US_Front < 20)
      Drive(1, -1);

  } else
    USTrack = false;
}

void doStuff() {
  if (Durat > 0)
    Durat--;

  if (CurAction != 0) {
    switch (CurAction) {
    case 1:
      Drive(0, 0);
      LED_1 = 1;
      break;
    case 2:
      Drive(0, 0);
      LED_1 = 2;
      if (Durat < 10)
        Drive(-5, -5);
      if (Durat < 6)
        Drive(-5, 5);
      if (Durat < 2) {
        onDeposit = false;
        pickedUp[0] = 0;
        pickedUp[1] = 0;
        pickedUp[2] = 0;
        pickedUp[3] = 0;
      }
      break;
    case 3:
      if (Durat >= 5)
        Drive(-1, -1);
      else
        Drive(-3, 3);
      break;
    case 5:
      Drive(3, -3);
      break;
    default:
      break;
    }
  }
  else {
    if (CurGame == 0)
      Drive(3, 3);
    
    if (pickedUp[3] > 0 || LoadedObjects >= 2)
      Drive(3, 3);
    else
      Drive(4, 4);
  }
  /*
      1 = Pickup
      2 = Deposit
      */
}

/***************************************************************************************************************/
void Game0() {
  //Teleport v naslednji svet (takoj ker je svet1 v pizdi)
  if (Time > 180 && LoadedObjects < 5  && !onDeposit) {
    pickedUp[0]=0;
    pickedUp[1]=0;
    pickedUp[2]=0;
    pickedUp[3]=0;
    LoadedObjects = 0;
    Teleport = 3;
  } else if (Time > 210 && !onDeposit) {
    pickedUp[0]=0;
    pickedUp[1]=0;
    pickedUp[2]=0;
    pickedUp[3]=0;
    LoadedObjects = 0;
    Teleport = 3;
  }

  doStuff();

  goToDepositW1();

  // PICKUP (gleda barve spodi pa shit pobira)
  if (shouldIPickUp()) {
    if (isColL(black) || isColR(black)) {
      if (CurAction == 0) {
        setAction(1, 3);
        pickedUp[2]++;
      }
    }
    if (isColL(red) || isColR(red)) {
      if (CurAction == 0) {
        setAction(1, 3);
        pickedUp[0]++;
      }
    }
    if (isColL(cyan) || isColR(cyan)) {
      if (CurAction == 0) {
        setAction(1, 3);
        pickedUp[1]++;
      }
    }
  }

  // DEPOSIT
  if (isColL(orange) && LoadedObjects > 2)
    Drive(0, 2);
  if (isColR(orange) && LoadedObjects > 2)
    Drive(2, 0);
  if (LoadedObjects > 0 && isColL(orange) && isColR(orange)) {
    if (CurAction == 0) {
      onDeposit = true;
      setAction(2, 4);
      LoadedObjects = 0;
    }
  } else if (CurAction != 2)
    onDeposit = false;

  // TRAP (preveri če je na rumenem ki)
  if (LoadedObjects > 0 && (isColL(yellow) || isColR(yellow)))
    onTrap = true;

  // TRAP (preveri če je na rumenem ki) (onTrap nastavi 3, da pol preveri če je na levi al desni)
  if (LoadedObjects > 0 && onTrap == 0 && (isColL(yellow) || isColR(yellow))) {
    searchThing = 16;
    onTrap = 3;
  }

  if (onTrap > 0){
    if(onTrap == 3) {
      if (isColR(yellow)) {
        onTrap = 1;
      } else if (isColL(yellow)) {
        onTrap = 2;
      }
    }

    if (onTrap == 1)
      Drive(-2, 2);
    else 
      Drive(2, -2);

    if(US_Front > 20 && US_Left > 5 && US_Right > 5 && !(isColL(yellow) || isColR(yellow))){
      onTrap = false;
    }
  }

  // TRAP (preveri če je not v lukni)
  if (isColL(blue) && isColR(blue))
    Drive(-3, -3);

  // ULTRASONIC (yeah...)
  if (CurAction == 0 && !onTrap && !USTrack) {
    if (US_Front < 15) {
      if (Compass > 60 && Compass < 120) 
        Drive(1, -1);
      else if (US_Left > US_Right)
        Drive(-2, 2);
      else
        Drive(2, -2);
    }
    if (US_Left < 10)
      Drive(2, -2);
    if (US_Right < 10)
      Drive(-2, 2);
  }

  if (CurAction == 0 && !onTrap &&  USTrack) {
    if (US_Front < 7) {
      if (Compass > 60 && Compass < 120) 
        Drive(1, -1);
      else if (US_Left > US_Right)
        Drive(-2, 2);
      else
        Drive(2, -2);
    }
    if (US_Left < 5)
      Drive(2, -2);
    if (US_Right < 5)
      Drive(-2, 2);
  }

  // RESET
  if (CurAction != 0 && Durat == 0) {
    if (CurAction == 1)
      LoadedObjects++;
    CurAction = 0;
    LED_1 = 0;
  }
}

void Game1() {
  if (searchThing > 0)
    searchThing--;

  if (checkArea(SuperArea))
    searchThing = 2;

  //RESET CurAction
  if (CurAction != 0 && Durat == 0) {
    if (CurAction == 1)
      LoadedObjects++;
    CurAction = 0;
    LED_1 = 0;
  }

  doStuff();
//**************************************************************
    if ((checkArea(AreaBluBlu1) || checkArea(AreaBluBlu2)) && searchFor == 1) {
      if (timeout == 0) {
       timeout = Time + 20;
      }

      if (Time >= timeout && timeout != 0) {
        pickedUp[1]++;
        timeout = 0;
      }
    } else if ((!(checkArea(AreaBluBlu1) || checkArea(AreaBluBlu2)) && searchFor == 0 && !checkArea(AreaMidMid) && PositionX != 0 && PositionY != 0) || pickedUp[2] == 2)  {
      timeout = 0;
    }

    if (checkArea(AreaBluBlk) && searchFor == 2) {
      if (timeout == 0) {
       timeout = Time + 20;
      }
    
      if (Time >= timeout && timeout != 0) {
        pickedUp[2]++;
        timeout = 0;
      }
    } else if ((!checkArea(AreaBluBlk) && searchFor == 10) || pickedUp[2] == 2) {
      timeout = 0;
    }

    if (checkArea(AreaMidMid) && searchFor == 0) {
      if (timeout == 0) {
       timeout = Time + 20;
      }
    
      if (Time >= timeout && timeout != 0) {
        pickedUp[0]++;
        timeout = 0;
      }
    } else if ((!checkArea(AreaMidMid) && searchFor == 2 && !checkArea(AreaBluBlk) && PositionX != 0 && PositionY != 0) || pickedUp[0] == 2) {
      timeout = 0;
    }

//**************************************************************
  if (SuperObj_Num > 0 && superObjectBool == -1) {
      superObject.x = SuperObj_X;
      superObject.y = SuperObj_Y;
      calcSuperArea();
      superObjectBool = true;
  }
  /*else if (SuperObj_Num == 0 && superObjectBool)
    superObjectBool = false;*/

  //E S T E T I K AAAAAA
  whatAmISearchingFor();
 
  if(onBorder == 0)
    calcArea();

  if (superObjectBool) {
    calcAngle(superObject);
    Area = 1;
  }

  if (shouldISearch()) {
    if ((Compass > Angle + 12 - 4 * superObjectBool || Compass < Angle - 12 + 4 * superObjectBool) || (US_Front < 20 && US_Left < 5 && US_Right < 5)) {
      if (leftOrRight() == 1)
        Drive(1, -1);
      else
        Drive(-1, 1);
    }
    else 
      forceLorR = -1;
  }

  // PICKUP (gleda barve spodi pa shit pobira)
  if (shouldIPickUp() && CurAction == 0) {
    if (isColL(black) || isColR(black)) {
      if (CurAction == 0) {
          setAction(1, 3);
          pickedUp[2]++;
          if (checkArea(AreaBluBlk))
           timeout += 20;
      }
    }else
    if (isColL(red) || isColR(red)) {
      if (CurAction == 0) {
        setAction(1, 3);
        pickedUp[0]++;
        if (checkArea(AreaMidMid))
          timeout += 20;
      }
    }else
    if (isColL(cyan) || isColR(cyan)) {
      if (CurAction == 0) {
        setAction(1, 3);
        pickedUp[1]++;
        if (checkArea(AreaBluBlu1) || checkArea(AreaBluBlu2))
          timeout += 20;
        }
      }else
    if ((isColL(purple) || isColR(purple) || isColL(purple2) || isColR(purple2)) && superObjectBool) {
      if (CurAction == 0) {
        setAction(1, 3);
        superObjectBool = 0;
        pickedUp[3]++;
      }
    }
  }
  
  // DEPOSIT
  if (isColL(orange) && (LoadedObjects > 2 || pickedUp[3] > 0) && CurAction == 0)
    Drive(0, 2);
  if (isColR(orange) && (LoadedObjects > 2 || pickedUp[3] > 0) && CurAction == 0)
    Drive(2, 0);
  if ((LoadedObjects > 2 || pickedUp[3] > 0) && isColL(orange) && isColR(orange)) {
    if (pickedUp[0]>=1 && pickedUp[1]>=1 && pickedUp[2]>=1)
      superObjectBool = -1;
    if (CurAction == 0) {
      setAction(2, 4);
      LoadedObjects = 0;
    }
  }

  // ULTRASONIC (yeah...)
  if (CurAction == 0)
    whereBeWalls();

  // TRAP (preveri če je na rumenem ki) (onTrap nastavi 3, da pol preveri če je na levi al desni)
  if (LoadedObjects > 0 && onTrap == 0 && (isColL(yellow) || isColR(yellow)) && superObjectBool == 0) {
    searchThing = 16;
    onTrap = 3;
  }

  if (onTrap > 0) {
    if(onTrap == 3) {
      if (isColR(yellow)) {
        onTrap = 1;
      } else if (isColL(yellow) ){
        onTrap = 2;
      }
    }

    if (onTrap == 1)
      Drive(-1, 1);
    else 
      Drive(1, -1);

    if(US_Front > 20 && US_Left > 5 &&US_Right > 5 && !(isColL(yellow) || isColR(yellow)))
      onTrap = false;
  }

  // TRAP (preveri če je not v lukni)
  if (isColL(blue) || isColR(blue)) {
    setAction(2, 0.2);
    LoadedObjects = 0;
    Drive(-5, -5);
  }

  if (CurAction == 0)
    whereBeBorders();
}

DLL_EXPORT void OnTimer() {
  switch (CurGame) {
  case 9:
    break;
  case 10:
    WheelLeft = 0;
    WheelRight = 0;
    LED_1 = 0;
    MyState = 0;
    break;
  case 0:
    Game0();
    break;
  case 1:
    Game1();
    break;
  default:
    break;
  }
}
