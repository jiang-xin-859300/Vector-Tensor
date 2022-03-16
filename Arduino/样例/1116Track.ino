/*************************************************************/
#include <Arduino.h>
#include <LiquidCrystal.h>//lcd
#include <SoftwareSerial.h>
#include <Servo.h>
#include <String.h>

/***********************机器人简图*******************************/
//           
//        机器人简图
//        |-------------|  | F2
//        | ——         |  |
//        |  R2         |
//        |激光4         |激光1
//        |             |
//        |             |
//        |激光3     R1  |激光2
//        |        ——  |
//      | |-------------|
//   F1 |
//        F1、F2、R1、R2表示电机，横线方向表示轮子方向
//        F1表示轮子方向向前进的第一个轮子，F2表示轮子方向向前进的第二个轮子
//        R1表示轮子方向向右的第一个轮子，F2表示轮子方向向右的第二个轮子

/*************************************************************/
/**************************参数定义***********************************/
#define SPEED  80   //全局变量，通过调节SPEED值来控制电机转速。  注意！！！SPEED值越小速度越快！
#define TRACKSPEED 150 //全局变量，通过调节TRACKSPEED值来控制电机转速。  注意！！！TRACKSPEED值越小循迹速度越快！

/**************************引脚定义***********************************/

#define F1_DIR 4           //左电机的方向控制接口对应 arduino 引脚 4
#define F1_EN 7            //左电机的使能端控制接口对应 arduino 引脚 7
#define F1_STP 5           //左电机的脉冲信号接口对应 arduino 引脚 5

#define F2_DIR 16           //左电机的方向控制接口对应 arduino 引脚 16
#define F2_EN 18            //左电机的使能端控制接口对应 arduino 引脚 18
#define F2_STP 19           //左电机的脉冲信号接口对应 arduino 引脚 19

#define R1_DIR 49          //右电机的方向控制接口对应 arduino 引脚 49
#define R1_EN 53           //右电机的使能端控制接口对应 arduino 引脚 53
#define R1_STP 51          //右电机的脉冲信号接口对应 arduino 引脚 51

#define R2_DIR 46          //右电机的方向控制接口对应 arduino 引脚 46
#define R2_EN 42           //右电机的使能端控制接口对应 arduino 引脚 42
#define R2_STP 44          //右电机的脉冲信号接口对应 arduino 引脚 44

#define Laser1 11   //定义激光传感器1接口引脚 11
#define Laser2 13  //定义激光传感器2接口引脚 13
#define Laser3 15   //定义激光传感器3接口引脚 15
#define Laser4 17  //定义激光传感器4接口引脚 17

/*************************************************************/

/**************************步进电机转速子函数定义***********************************/

void Track_PWM(int Pin, int delay_us) {
  //产生一定频率的 PWM 波来驱动步进电机
  //Pin 产生 PWM 的引脚 delay_ms 影响 PWM 的频率 一般设置 100 开始调
  //delay_ms 数值越小，速度越快，但是速度过快会打滑，需要自己把控
digitalWrite(Pin, HIGH);                 //将指定的引脚置高电平
delayMicroseconds(delay_us);             //持续高电平 delay_us 时间（单位是微秒）
digitalWrite(Pin, LOW);                  //将指定的引脚置低电平
delayMicroseconds(delay_us);             //持续低电平 delay_us 时间（单位是微秒）

}

/*************************************************************/


/**************************步进电机使能子函数定义***********************************/
void Track_Setup(){
pinMode(F1_DIR, OUTPUT);
pinMode(F1_EN, OUTPUT);
pinMode(F1_STP, OUTPUT);
pinMode(R1_DIR, OUTPUT);
pinMode(R1_EN, OUTPUT);
pinMode(R1_STP, OUTPUT);
pinMode(F2_DIR, OUTPUT);
pinMode(F2_EN, OUTPUT);
pinMode(F2_STP, OUTPUT);
pinMode(R2_DIR, OUTPUT);
pinMode(R2_EN, OUTPUT);
pinMode(R2_STP, OUTPUT);
digitalWrite(F1_EN, HIGH);         //EN 置高，表明之后可以控制电机F1
digitalWrite(R1_EN, HIGH);         //EN 置高，表明之后可以控制电机R1
digitalWrite(F2_EN, HIGH);         //EN 置高，表明之后可以控制电机F2
digitalWrite(R2_EN, HIGH);         //EN 置高，表明之后可以控制电机R2
}

void Stop()
{
  digitalWrite(F1_EN,LOW);         //EN 置低，表明之后无法控制电机
  digitalWrite(R1_EN,LOW);         //EN 置低，表明之后无法控制电机
  digitalWrite(F2_EN,LOW);         //EN 置低，表明之后无法控制电机
  digitalWrite(R2_EN,LOW);         //EN 置低，表明之后无法控制电机
  }
  
/*************************************************************/


// *********循迹子函数总览*********//
//GoForward(int Step)  //F1和F2转速相同，机器人整体向前进
//GoBackward(int Step) //F1和F2转速相同，机器人整体向后退
//GoLeft(int Step)     //R1和R2转速相同，机器人整体向左行驶
//GoRight(int Step)     //R1和R2转速相同，机器人整体向右行驶
//
//TrackFieldRightR1R2(int Step)  //在田垄上车子向右行驶，R1比R2快，差速
//TrackFieldRightR2R1(int Step)  //在田垄上车子向右行驶，R2比R1快，差速
//TrackFieldLeftR1R2(int Step)   //在田垄上车子向左行驶，R1比R2快，差速
//TrackFieldLeftR2R1(int Step)   //在田垄上车子向左行驶，R2比R1快，差速
//
//TrackRoadForwardF1F2(int Step)   //在场地上车子向前行驶，F1比F2快，差速
//TrackRoadForwardF2F1(int Step){   //在场地上车子向前行驶，F2比F1快，差速  
//TrackRoadBackwardF1F2(int Step){   //在场地上车子向后行驶，F1比F2快，差速
//TrackRoadBackwardF2F1(int Step){   //在场地上车子向后行驶，F2比F1快，差速

/**************************循迹子函数定义***********************************/ 
void GoForward(int Step)      //F1和F2转速相同，机器人整体向前进
{
  digitalWrite(F1_EN, HIGH);
  digitalWrite(F1_DIR, HIGH);

  digitalWrite(F2_EN, HIGH);
  digitalWrite(F2_DIR, LOW);
  
  for (int x = 0; x < Step; x++){
      Track_PWM(F1_STP, SPEED); 
      Track_PWM(F2_STP, SPEED);
  }
}

void GoBackward(int Step)     //F1和F2转速相同，机器人整体向后退（电机转向与上一个函数相反）
{
  digitalWrite(F1_EN, HIGH);
  digitalWrite(F1_DIR, LOW);

  digitalWrite(F2_EN, HIGH);
  digitalWrite(F2_DIR,HIGH);
  
  for (int x = 0; x < Step; x++)
  {
      Track_PWM(F1_STP, SPEED); 
      Track_PWM(F2_STP, SPEED);
  }
}

void GoLeft(int Step)     //R1和R2转速相同，机器人整体向左行驶
{
  digitalWrite(R1_EN, HIGH);
  digitalWrite(R1_DIR, LOW);

  digitalWrite(R2_EN, HIGH);
  digitalWrite(R2_DIR,HIGH);
  
  for (int x = 0; x < Step; x++)
  {
      Track_PWM(R1_STP, SPEED); 
      Track_PWM(R2_STP, SPEED);
  }
}

void GoRight(int Step)     //R1和R2转速相同，机器人整体向右行驶
{
  digitalWrite(R1_EN, HIGH);
  digitalWrite(R1_DIR,HIGH);

  digitalWrite(R2_EN, HIGH);
  digitalWrite(R2_DIR, LOW);
  
  for (int x = 0; x < Step; x++)
  {
      Track_PWM(R1_STP, SPEED); 
      Track_PWM(R2_STP, SPEED);
  }
}


void TrackFieldRightR1R2(int Step){   //在田垄上车子R1比R2快，差速
  digitalWrite(R1_EN, HIGH);     //机器人向右Right行驶差速1代码
  digitalWrite(R1_DIR, HIGH);
  
  digitalWrite(R2_EN, HIGH);
  digitalWrite(R2_DIR, LOW);

  for (int xlyy = 0; xlyy < Step; xlyy++)
  {
    digitalWrite(R1_STP, HIGH);
    digitalWrite(R2_STP, HIGH);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(R1_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(R1_STP, HIGH);
    digitalWrite(R2_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(R1_STP, LOW);
    delayMicroseconds(TRACKSPEED);
  }
}

void TrackFieldRightR2R1(int Step){   //在田垄上车子R2比R1快，差速
  digitalWrite(R1_EN, HIGH);     //机器人向右Right行驶差速2代码
  digitalWrite(R1_DIR, HIGH);
  
  digitalWrite(R2_EN, HIGH);
  digitalWrite(R2_DIR, LOW);

  for (int xlyy = 0; xlyy < Step; xlyy++)
  {
    digitalWrite(R2_STP, HIGH);
    digitalWrite(R1_STP, HIGH);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(R2_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(R2_STP, HIGH);
    digitalWrite(R1_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(R2_STP, LOW);
    delayMicroseconds(TRACKSPEED);
  }
}
void TrackFieldLeftR1R2(int Step){   //在田垄上车子R1比R2快，差速
  digitalWrite(R1_EN, HIGH);     //机器人向左Left行驶差速1代码
  digitalWrite(R1_DIR, LOW);
  
  digitalWrite(R2_EN, HIGH);
  digitalWrite(R2_DIR,HIGH);

  for (int xlyy = 0; xlyy < Step; xlyy++)
  {
    digitalWrite(R1_STP, HIGH);
    digitalWrite(R2_STP, HIGH);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(R1_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(R1_STP, HIGH);
    digitalWrite(R2_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(R1_STP, LOW);
    delayMicroseconds(TRACKSPEED);
  }
}

void TrackFieldLeftR2R1(int Step){   //在田垄上车子R2比R1快，差速
  digitalWrite(R1_EN, HIGH);     //机器人向左Left行驶差速2代码
  digitalWrite(R1_DIR, LOW);
  
  digitalWrite(R2_EN, HIGH);
  digitalWrite(R2_DIR,HIGH);

  for (int xlyy = 0; xlyy < Step; xlyy++)
  {
    digitalWrite(R2_STP, HIGH);
    digitalWrite(R1_STP, HIGH);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(R2_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(R2_STP, HIGH);
    digitalWrite(R1_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(R2_STP, LOW);
    delayMicroseconds(TRACKSPEED);
  }
}

void TrackRoadForwardF1F2(int Step){   //在场地上车子F1比F2快，差速
  digitalWrite(F1_EN, HIGH);    //机器人向前Forward行驶差速1代码
  digitalWrite(F1_DIR, LOW);

  digitalWrite(F2_EN, HIGH);
  digitalWrite(F2_DIR, HIGH);

  for (int xlyy = 0; xlyy < Step; xlyy++)
  {
    digitalWrite(F1_STP, HIGH);
    digitalWrite(F2_STP, HIGH);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(F1_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(F1_STP, HIGH);
    digitalWrite(F2_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(F1_STP, LOW);
    delayMicroseconds(TRACKSPEED);
  }
}

void TrackRoadForwardF2F1(int Step){   //在场地上车子F2比F1快，差速
  digitalWrite(F1_EN, HIGH);    //机器人向前Forward行驶差速2代码
  digitalWrite(F1_DIR, LOW);

  digitalWrite(F2_EN, HIGH);
  digitalWrite(F2_DIR, HIGH);

  for (int xlyy = 0; xlyy < Step; xlyy++)
  {
    digitalWrite(F2_STP, HIGH);
    digitalWrite(F1_STP, HIGH);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(F2_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(F2_STP, HIGH);
    digitalWrite(F1_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(F2_STP, LOW);
    delayMicroseconds(TRACKSPEED);
  }
}


void TrackRoadBackwardF1F2(int Step){   //在场地上车子F1比F2快，差速
  digitalWrite(F1_EN, HIGH);    //机器人向后Backward行驶差速1代码
  digitalWrite(F1_DIR,HIGH);

  digitalWrite(F2_EN, HIGH);
  digitalWrite(F2_DIR, LOW);

  for (int xlyy = 0; xlyy < Step; xlyy++)
  {
    digitalWrite(F1_STP, HIGH);
    digitalWrite(F2_STP, HIGH);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(F1_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(F1_STP, HIGH);
    digitalWrite(F2_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(F1_STP, LOW);
    delayMicroseconds(TRACKSPEED);
  }
}

void TrackRoadBackwardF2F1(int Step){   //在场地上车子F2比F1快，差速
  digitalWrite(F1_EN, HIGH);    //机器人向后Backward行驶差速2代码
  digitalWrite(F1_DIR,HIGH);

  digitalWrite(F2_EN, HIGH);
  digitalWrite(F2_DIR, LOW);

  for (int xlyy = 0; xlyy < Step; xlyy++)
  {
    digitalWrite(F2_STP, HIGH);
    digitalWrite(F1_STP, HIGH);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(F2_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(F2_STP, HIGH);
    digitalWrite(F1_STP, LOW);
    delayMicroseconds(TRACKSPEED);
    digitalWrite(F2_STP, LOW);
    delayMicroseconds(TRACKSPEED);
  }
}


/***********************机器人简图*******************************/
//           
//        机器人简图
//        |-------------|  | F2
//        | ——         |  |
//        |  R2         |
//        |激光4         |激光1
//        |             |
//        |             |
//        |激光3     R1  |激光2
//        |        ——  |
//      | |-------------|
//   F1 |
//        F1、F2、R1、R2表示电机，横线方向表示轮子方向
//        F1表示轮子方向向前进的第一个轮子，F2表示轮子方向向前进的第二个轮子
//        R1表示轮子方向向右的第一个轮子，F2表示轮子方向向右的第二个轮子

/*************************************************************/

void TrackRight(int step){       //机器人在田垄上向右循迹行驶
  int lyy;
  for(lyy=0;lyy<step;lyy++){
    int laser1=digitalRead(Laser1);
    int laser2=digitalRead(Laser2);
    int laser3=digitalRead(Laser3);
    int laser4=digitalRead(Laser4);
    //该版本激光探测到物体输出1，无物体输出0，可按照实际情况修改
    if(laser1==1&& laser2==1&& laser3==1&& laser4==1)   //四个激光都亮，向右走
      {GoRight(20);}
    else if(laser1==1&& laser2==0&& laser3==1&& laser4==0)   //R1需要比R2转得快才能调回来
      {TrackFieldRightR1R2(1);}
    else if(laser1==1&& laser2==0&& laser3==1&& laser4==0)   //R2需要比R1转得快才能调回来
      {TrackFieldRightR2R1(1);}
    else if(laser1==1&& laser2==0&& laser3==0&& laser4==1)   // 整体需要向前走
      {GoForward(1);}
    else if(laser1==0&& laser2==1&& laser3==1&& laser4==0)   // 整体需要向后走
      {GoBackward(1);}  
    else {GoRight(20);} 
  }
}
/***********************机器人简图*******************************/
//           
//        机器人简图
//        |-------------|  | F2
//        | ——         |  |
//        |  R2         |
//        |激光4         |激光1
//        |             |
//        |             |
//        |激光3     R1  |激光2
//        |        ——  |
//      | |-------------|
//   F1 |
//        F1、F2、R1、R2表示电机，横线方向表示轮子方向
//        F1表示轮子方向向前进的第一个轮子，F2表示轮子方向向前进的第二个轮子
//        R1表示轮子方向向右的第一个轮子，F2表示轮子方向向右的第二个轮子

/*************************************************************/
void TrackLeft(int step){       //机器人在田垄上向左循迹行驶
  int lyy;
  for(lyy=0;lyy<step;lyy++){
    int laser1=digitalRead(Laser1);
    int laser2=digitalRead(Laser2);
    int laser3=digitalRead(Laser3);
    int laser4=digitalRead(Laser4);
    //该版本激光探测到物体输出1，无物体输出0，可按照实际情况修改
    if(laser1==1&& laser2==1&& laser3==1&& laser4==1)   //四个激光都亮，向右走
      {GoLeft(20);}
    else if(laser1==1&& laser2==0&& laser3==1&& laser4==0)   //R2需要比R1转得快才能调回来
      {TrackFieldRightR2R1(1);}
    else if(laser1==1&& laser2==0&& laser3==1&& laser4==0)   //R1需要比R2转得快才能调回来
      {TrackFieldRightR1R2(1);}
    else if(laser1==1&& laser2==0&& laser3==0&& laser4==1)   // 整体需要向前走
      {GoForward(1);}
    else if(laser1==0&& laser2==1&& laser3==1&& laser4==0)   // 整体需要向后走
      {GoBackward(1);}  
    else {GoLeft(20);} 
  }
}

/**************************************************************/

void setup() {
  Track_Setup();  
}       //void setup 结束大括号

/**************************************************************/


void loop() {
  Track_PWM(5, 200)
}        // loop 结束大括号
