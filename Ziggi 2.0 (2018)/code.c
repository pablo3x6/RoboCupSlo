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
int onSwamp = 0;
int isUltrasonic = 0;
int Area = 0;
int superObjectBool = 0;
int onDeposit = false;
int pickedUp[4] = {0, 0, 0, 0};
int forceLorR = -1;
int borderUp = 250;
int borderDown = 10;
int borderLeft = 10;
int borderRight = 350;

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
  sprintf(info, "superObjBool=%d;Area=%d;Angle=%d;searchThing=%d;onTrap=%d;Duration=%d;SuperDuration=%d;bGameEnd=%d;CurAction=%d;CurGame=%d;SuperObj_Num=%d;SuperObj_X=%d;SuperObj_Y=%d;Teleport=%d;LoadedObjects=%d;CurObjects=%d;US_Front=%d;US_Left=%d;US_Right=%d;CSLeft_R=%d;CSLeft_G=%d;CSLeft_B=%d;CSRight_R=%d;CSRight_G=%d;CSRight_B=%d;PositionX=%d;PositionY=%d;TM_State=%d;Compass=%d;Time=%d;WheelLeft=%d;WheelRight=%d;LED_1=%d;MyState=%d;borderDown=%d;", superObjectBool, Area, Angle, searchThing, onTrap, Duration, SuperDuration, bGameEnd, CurAction, CurGame, SuperObj_Num, SuperObj_X, SuperObj_Y, Teleport, LoadedObjects, CurObjects, US_Front, US_Left, US_Right, CSLeft_R, CSLeft_G, CSLeft_B, CSRight_R, CSRight_G, CSRight_B, PositionX, PositionY, TM_State, Compass, Time, WheelLeft, WheelRight, LED_1, MyState, borderDown);
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

struct colors
{
  int miR;
  int miG;
  int miB;
  int maR;
  int maG;
  int maB;
};

struct areas
{
  int startX;
  int startY;
  int endX;
  int endY;
};

struct points
{
  int x;
  int y;
};

struct colors red = {200, 0, 0, 255, 60, 60};
struct colors cyan = {0, 220, 220, 40, 255, 255};
struct colors black = {0, 0, 0, 60, 60, 60};
struct colors yellow = {190, 190, 0, 255, 255, 50};
struct colors yellowB = {0, 0, 150, 100, 100, 255};
struct colors orange = {200, 110, 0, 255, 150, 20};
struct colors blue = {20, 20, 240, 60, 60, 255};
struct colors gray = {130, 140, 185, 165, 165, 210};
struct colors purple = {230, 0, 230, 255, 15, 255};
struct colors purple2 = {90, 0, 230, 130, 15, 255};

//                     sx  sy   ex   ey
struct areas AreaUL = {0, 255, 85, 190};
struct areas AreaUM = {85, 255, 280, 180};
struct areas AreaUR = {280, 255, 350, 90};
struct areas AreaDL = {0, 115, 70, 0};
struct areas AreaDR = {105, 90, 350, 0};
struct areas AreaMM = {140, 140, 220, 90};
struct areas AreaFL = {85, 190, 115, 150};
struct areas AreaFR = {260, 120, 290, 90};

//POINTAREAS
struct areas AreaBla1 = {50, 230, 90, 180};
struct areas AreaBla2 = {100, 160, 125, 110};
struct areas AreaBla3 = {0, 50, 275, 0};
struct areas AreaBlu1 = {0, 90, 70, 50};
struct areas AreaBlu2 = {80, 95, 165, 60};
struct areas AreaRed1 = {140, 255, 350, 190};

struct areas SwampUL = {50, 230, 85, 180};
struct areas SwampMR = {220, 160, 280, 120};
struct areas SwampML = {100, 165, 130, 95};
struct areas SwampDR = {290, 90, 320, 40};
struct areas SwampDD = {275, 90, 290, 0};

struct areas SwampLine1 = {85, 160, 100, 105};
struct areas SwampLine2 = {80, 105, 100, 40};
struct areas SwampLine3 = {70, 40, 120, 0};

struct areas SuperArea = {0, 0, 0, 0};
struct areas SuperAreaWide = {0, 0, 0, 0};

struct points depositLeftL = {70, 140};
struct points depositLeftR = {135, 195};
struct points depositRightL = {255, 60};
struct points depositRightR = {320, 130};
struct points superObject = {0, 0};
struct points flowerLeft = {100, 170};
struct points black1 = {70, 210};
struct points black2 = {115, 135};
struct points black3 = {100, 30};
struct points blue1 = {40, 70};
struct points blue2 = {130, 75};
struct points red1 = {165, 235};

void Drive(int left, int right)
{
  if (isOnSwamp() || isColL(gray) || isColR(gray))
  {
    WheelLeft = left * 2;
    WheelRight = right * 2;
  }else{
    WheelLeft = left;
    WheelRight = right;
  }
}

int isColR(struct colors color)
{
  if (CSRight_R >= color.miR && CSRight_R <= color.maR && CSRight_G >= color.miG && CSRight_G <= color.maG && CSRight_B >= color.miB && CSRight_B <= color.maB)
    return true;
  else
    return false;
}

int isColL(struct colors color)
{
  if (CSLeft_R >= color.miR && CSLeft_R <= color.maR && CSLeft_G >= color.miG && CSLeft_G <= color.maG && CSLeft_B >= color.miB && CSLeft_B <= color.maB)
    return true;
  else
    return false;
}

//Zračuna okolico SuperObjecta in da v struct 
void calcSuperArea()
{
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
int leftOrRight()
{
  if (forceLorR != -1){
    return forceLorR;
  }
  if (Angle < Compass){
    if ((Compass - Angle) < 180){
      return 1;
    }else return 0;
  }

  if (Angle > Compass){
    if ((Angle - Compass) < 180){
      return 0;
    }else return 1;
  }
}

void setAction(int action, double dur)
{
  dur = dur * 16;
  Durat = (int)dur;
  CurAction = action;
}

void whereBeWalls() {
  if (US_Front < 10)
  {
    searchThing = 2;
    if (US_Left<US_Right){
      Drive(1, -1);
    }else Drive(-1, 1);
  }

  if (US_Right < 10){
    if (superObjectBool && checkArea(SuperArea)){
      forceLorR = 0;
    }else{
      forceLorR = -1;
    }
    searchThing = 4;
    Drive(1, 2);
  }

  if (US_Left < 10){
    if (superObjectBool && checkArea(SuperArea)){
      forceLorR = 1;
    }else{
      forceLorR = -1;
    }
    searchThing = 4;
    Drive(2, 1);
  }

  if (US_Left < 7)
    Drive(1, -1);

  if (US_Right < 7)
    Drive(-1, 1);

  if ((US_Front+US_Left+US_Right) <= 40){
    searchThing = 4;
    setAction(3, 0.5);
  }
}

//nastavi border nazaj na deafult
void setBorderDeafult(){
  borderUp = 250;
  borderDown = 10;
  borderLeft = 10;
  borderRight = 350;
}

void setBorder(struct areas area){
  borderUp = area.startY;
  borderDown = area.endY;
  borderLeft = area.startX;
  borderRight = area.endX;
}

//nastavi onBorder glede na to na kerem borderu je
void isOnBorder()
{
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
void whereBeBorders()
{
  isOnBorder();
  if (onBorder>0)
  {
    switch(onBorder)
    {
      case 1:
        if (Compass>90 && Compass<270 && US_Front>30 && US_Left>15 && US_Right>15 && !isColL(yellow) && !isColR(yellow))
        {
          onBorder = 0;
        }else Drive(1,-1);
        break;
      case 2:
        if (Compass>180 && Compass<360 && US_Front>30 && US_Left>15 && US_Right>15 && !isColL(yellow) && !isColR(yellow))
        {
          onBorder = 0;
        }else Drive(1,-1);
        break;
      case 3:
        if ((Compass<90 || Compass>270) && US_Front>30 && US_Left>15 && US_Right>15 && !isColL(yellow) && !isColR(yellow))
        {
          onBorder = 0;
        }else Drive(1,-1);
        break;
      case 4:
        if (Compass<180 && US_Front>30 && US_Left>15 && US_Right>15 && !isColL(yellow) && !isColR(yellow))
        {
          onBorder = 0;
        }else Drive(1,-1);
        break;
    }
  }
}

void calcAngle(struct points point)
{
  if (PositionX != 0 && PositionY != 0)
  {
    if (point.y > PositionY)
    {
      if (point.x > PositionX)
      {
        Angle = (atan2(point.y - PositionY, point.x - PositionX) * 180 / 3.14) + 270;
      }
      else
      {
        Angle = atan2(PositionX - point.x, point.y - PositionY) * 180 / 3.14;
      }
    }
    else
    {
      if (point.x > PositionX)
      {
        Angle = (atan2(point.x - PositionX, PositionY - point.y) * 180 / 3.14) + 180;
      }
      else
      {
        Angle = (atan2(PositionY - point.y, PositionX - point.x) * 180 / 3.14) + 90;
      }
    }
  }
}

int shouldISearch(){
  if (searchThing == 0 && PositionX != 0 && PositionY != 0 && CurAction == 0){
    if ((LoadedObjects > 4 || pickedUp[3] > 0 || superObjectBool == 1) && Area!=0){
      return true;
    }else{
      return false;
    }
  }else{
    return false;
  }
}

//Preveri, če je pickup pametna ideja (da še ma plac za superobject, če obstaja)
int shouldIPickUp(){
  if(superObjectBool != 1 && LoadedObjects < 6){
    return true;
  }else if(LoadedObjects < 5 && superObjectBool){
    return true;
  }else return false;
}

int checkArea(struct areas area)
{
  if ((PositionX > area.startX && PositionX < area.endX) && (PositionY < area.startY && PositionY > area.endY))
    return true;
  else
    return false;
}

int isOnSwamp(){
  if (checkArea(SwampDD) || checkArea(SwampDR) || checkArea(SwampMR) || checkArea(SwampML) || checkArea(SwampUL))
    return true;
  else
    return false;
}

void doStuff()
{
  if (Durat > 0)
    Durat--;

  if (CurAction != 0)
  {
    switch (CurAction)
    {
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
      if (Durat < 2)
        onDeposit = false;
        pickedUp[0]=0;
        pickedUp[1]=0;
        pickedUp[2]=0;
        pickedUp[3]=0;
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
  else
  {
    if (CurGame == 0)
    {
      if (LoadedObjects == 0)
      {
        Drive(3, 3);
      }
      else
      {
        if (US_Front > 40 && US_Left > 20 && US_Right > 20)
          Drive(3, 3);
        else
          Drive(2, 2);
      }
    }
    else if (isColL(gray) || isColR(gray))
    {
      Drive(5, 5);
    }
    else
    {
        if (US_Front > 30 && US_Left > 15 && US_Right > 15)
          Drive(4, 4);
        else
          Drive(3, 3);
    }
  }
  /*
      1 = Pickup
      2 = Deposit
    
      */
}

/***************************************************************************************************************/
void Game0()
{
  //Teleport v naslednji svet (čas odvisn odvisn od LoadedObjects)
  if (Time > 210 && LoadedObjects < 3 && !onDeposit)
  {
    pickedUp[0]=0;
    pickedUp[1]=0;
    pickedUp[2]=0;
    pickedUp[3]=0;
    LoadedObjects = 0;
    Teleport = 1;
  }
  else if (Time > 230 && !onDeposit && LoadedObjects < 4)
  {
    pickedUp[0]=0;
    pickedUp[1]=0;
    pickedUp[2]=0;
    pickedUp[3]=0;
    LoadedObjects = 0;
    Teleport = 1;
  }
  else if (Time > 270 && !onDeposit)
  {
    pickedUp[0]=0;
    pickedUp[1]=0;
    pickedUp[2]=0;
    pickedUp[3]=0;
    LoadedObjects = 0;
    Teleport = 1;
  }

  doStuff();

  // PICKUP (gleda barve spodi pa shit pobira)
  if (!(LoadedObjects >= 6))
  {
    if (isColL(black) || isColR(black))
    {
      if (CurAction == 0)
        setAction(1, 3);
    }
    if (isColL(red) || isColR(red))
    {
      if (CurAction == 0)
        setAction(1, 3);
    }
    if (isColL(cyan) || isColR(cyan))
    {
      if (CurAction == 0)
        setAction(1, 3);
    }
  }

  // DEPOSIT
  if (isColL(orange) && LoadedObjects > 2)
    Drive(0, 2);
  if (isColR(orange) && LoadedObjects > 2)
    Drive(2, 0);
  if (LoadedObjects > 0 && isColL(orange) && isColR(orange))
  {
    if (CurAction == 0)
    {
      onDeposit = true;
      setAction(2, 4);
      LoadedObjects = 0;
    }
  }
  else if (CurAction != 2)
  {
    onDeposit = false;
  }

  // TRAP (preveri če je na rumenem ki)
  if (LoadedObjects > 0 && (isColL(yellow) || isColR(yellow) || isColL(yellowB) || isColR(yellowB)))
  {
    onTrap = true;
  }

  if (onTrap > 0){
    if (isColL(yellow) || isColL(yellowB)){
      Drive(1, -2);
      onTrap = 1;
    }

    if (isColR(yellow) || isColR(yellowB)){
      Drive(-2, 1);
      onTrap = 2;
    }

    if (US_Front<=20 || US_Left<=5 || US_Right<=5){
      if (onTrap == 1)
        Drive(1, -2);
      else 
        Drive(-2, 1);
    }

    if(US_Front>20 && US_Left>5 &&US_Right>5 && !(isColL(yellow) || isColR(yellow) || isColL(yellowB) || isColR(yellowB))){
      onTrap = false;
    }
  }

  // TRAP (preveri če je not v lukni)
  if (isColL(blue) && isColR(blue))
    Drive(-3, -3);

  // ULTRASONIC (yeah...)
  if (CurAction == 0 && !onTrap)
    whereBeWalls();

  // RESET
  if (CurAction != 0 && Durat == 0)
  {
    if (CurAction == 1)
      LoadedObjects++;
    CurAction = 0;
    LED_1 = 0;
  }
}

void Game1()
{
  if (searchThing > 0)
    searchThing--;

  if (checkArea(SuperArea))
  {
    searchThing = 2;
  }

  //RESET CurAction
  if (CurAction != 0 && Durat == 0)
  {
    if (CurAction == 1)
      LoadedObjects++;
    CurAction = 0;
    LED_1 = 0;
  }

  doStuff();

  if (SuperObj_Num>0 && superObjectBool==-1){
      superObject.x = SuperObj_X;
      superObject.y = SuperObj_Y;
      calcSuperArea();
      superObjectBool = true;
  }
  /*else if (SuperObj_Num == 0 && superObjectBool)
    superObjectBool = false;*/

  //Check area pol pa kalkulira do najbližje bolnice
  if (checkArea(AreaUL))
  {
    calcAngle(flowerLeft);
    Area = 1;
  }
  else if (checkArea(AreaUM))
  {
    calcAngle(depositLeftR);
    Area = 2;
  }
  else if (checkArea(AreaUR))
  {
    calcAngle(depositRightR);
    Area = 3;
  }
  else if (checkArea(AreaMM))
  {
    calcAngle(depositLeftL);
    Area = 4;
  }
  else if (checkArea(AreaDL))
  {
    calcAngle(depositLeftL);
    Area = 5;
  }
  else if (checkArea(AreaDR))
  {
    calcAngle(depositRightL);
    Area = 6;
  }
  else if (checkArea(AreaFL))
  {
    calcAngle(depositLeftL);
    Area = 7;
  }
  else if (checkArea(AreaFR))
  {
    calcAngle(depositRightL);
    Area = 8;
  }
  else
    Area = 0;

  if (superObjectBool == true)
  {
    calcAngle(superObject);
    Area = 1;
  }

  if (shouldISearch()){
    if (Compass > Angle + 12 - 4 * superObjectBool || Compass < Angle - 12 + 4 * superObjectBool){
      if (leftOrRight() == 1)
      {
        Drive(1, -1);
      }
      else
      {
        Drive(-1, 1);
      }
    }else forceLorR = -1;
  }

  // PICKUP (gleda barve spodi pa shit pobira)
  if (shouldIPickUp())
  {
    if (isColL(black) || isColR(black))
    {
      if (CurAction == 0)
        pickedUp[2]++;
        setAction(1, 3);
    }
    if (isColL(red) || isColR(red))
    {
      if (CurAction == 0)
        pickedUp[0]++;
        setAction(1, 3);
    }
    if (isColL(cyan) || isColR(cyan))
    {
      if (CurAction == 0)
        pickedUp[1]++;
        setAction(1, 3);
      }
    if ((isColL(purple) || isColR(purple) || isColL(purple2) || isColR(purple2)) && superObjectBool)
    {
      if (CurAction == 0)
      {
        superObjectBool = 0;
        pickedUp[3]++;
        setAction(1, 3);
      }
    }
  }
  
  // DEPOSIT
  if (isColL(orange) && (LoadedObjects > 2 || pickedUp[3] > 0))
    Drive(0, 2);
  if (isColR(orange) && (LoadedObjects > 2 || pickedUp[3] > 0))
    Drive(2, 0);
  if ((LoadedObjects > 2 || pickedUp[3] > 0) && isColL(orange) && isColR(orange))
  {
    if (pickedUp[0]>=1 && pickedUp[1]>=1 && pickedUp[2]>=1){
      superObjectBool = -1;
    }
    if (CurAction == 0)
    {
      setAction(2, 4);
      LoadedObjects = 0;
    }
  }

  // ULTRASONIC (yeah...)
  if (CurAction == 0)
    whereBeWalls();

  // TRAP (preveri če je na rumenem ki)
  if (LoadedObjects > 0 && (isColL(yellow) || isColR(yellow) || isColL(yellowB) || isColR(yellowB)))
  {
    searchThing = 16;
    onTrap = true;
  }

  if (onTrap>0){
    if (isColL(yellow) || isColL(yellowB)){
      Drive(0, -2);
      onTrap = 1;
    }

    if (isColR(yellow) || isColR(yellowB)){
      Drive(-2, 0);
      onTrap = 2;
    }

    if (US_Front<=20 || US_Left<=5 || US_Right<=5){
      if (onTrap == 1)
        Drive(2, -2);
      else 
        Drive(-2, 2);
    }

    if(US_Front>20 && US_Left>5 &&US_Right>5 && !(isColL(yellow) || isColR(yellow) || isColL(yellowB) || isColR(yellowB))){
      onTrap = false;
    }
  }

  // TRAP (izklopi US na rumeni)
  if (!isColL(yellow) && !isColR(yellow))
    onTrap = false;

  // TRAP (preveri če je not v lukni)
  if (isColL(blue) || isColR(blue))
  {
    Drive(-5, -5);
  }
  if (CurAction == 0)
    whereBeBorders();
}

DLL_EXPORT void OnTimer()
{
  switch (CurGame)
  {
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
