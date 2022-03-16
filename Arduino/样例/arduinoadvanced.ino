//#include <Arduino.h>
//#include <SoftwareSerial.h>
//#include <Servo.h>
//#include <String.h>
#include<Wire.h>
#include<math.h>
//---------------变量定义--------------------------
//GY25定义
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN A0
Adafruit_NeoPixel strip = Adafruit_NeoPixel(64,PIN,NEO_GRB + NEO_KHZ800);

float Roll, Pitch, Yaw, a, b;
int flag_t=0;
unsigned char Re_buf[8],counter=0;

int Filter_Value; //滤波器定义
int count = 0;

//步进电机
#define DIR3 49//丝杆1
#define STP3 51
#define ENA3 53

#define DIR1 4//丝杆2
#define STP1 5
#define ENA1 7


#define DIR4 46//right
#define STP4 44
#define ENA4 42

//直流电机
#define en A6
#define aa A8
#define ab A10

//速度定义
#define SPD1 150  //前进速度
#define SPD2 150  //旋转速度

#define duoji1 34
#define duoji2  2  //旋转主舵机


int pillar = 11; //柱子
int YPR[3];
unsigned char sign=0;
int i;
int j;
int flag = 0;
int item = 0;
int flag_l = 0;
int item1 = 1 ;
int item2 = 1 ;
int flag_b = 0;
int spp=-3;
//------------------函数定义-------------------------
double anglediff(int a,int b)
{
  double x=a*PI/180;
  double y=b*PI/180;
  double c=(sin(x)-sin(y))*(sin(x)-sin(y)) + (cos(x)-cos(y))*(cos(x)-cos(y));
  return (acos((2.0-c)/2.0))/PI*180;
}
//舵机旋转
void rotation(int x, int t)
{
  double t3 = t+196-262;
  if(t3>360)
  t3-=360;
  else if(t3<0)
  t3+=360;
  else{}
  double temo = t3 / 360;
  double t1 = temo * 2000 + 500;
  int t2 = int(t1);
  //Serial.print(t2);
  unsigned char c = 200;

  while (c--) {
    digitalWrite(x, HIGH);
    delayMicroseconds(t2);
    digitalWrite(x, LOW);
    delayMicroseconds(20000 - t2);
  }
}


//GY25函数，每调用这个函数一次，Yaw水平方向的角度将刷新一次
void serialEvent() 
{
  while (Serial1.available()) {
    Re_buf[counter] = (unsigned char)Serial1.read();
    if (counter == 0 && Re_buf[0] != 0xAA) return; // 检查帧头
    counter++;
    if (counter == 8)             //接收到数据
    {
      counter = 0;               //重新赋值，准备下一帧数据的接收
      sign = 1;
    }
  }
}


//丝杆上下滑动
void shangxia(int ste, int ff) {

  int ct = 500;
  int cj = 1000;
  if (ff == 0) {
    digitalWrite(ENA1, HIGH);
    digitalWrite(DIR1, HIGH);

  }
  else {
    digitalWrite(ENA1, HIGH);
    digitalWrite(DIR1, LOW);

  }

  for (int i = 0; i < 50 ; i++)
  {
    digitalWrite(STP1, HIGH);
    delayMicroseconds(cj);
    digitalWrite(STP1, LOW);
    delayMicroseconds(cj);
  }
  for (int i = 0; i < 50 ; i++)
  {
    digitalWrite(STP1, HIGH);
    delayMicroseconds(cj - 100);
    digitalWrite(STP1, LOW);
    delayMicroseconds(cj - 100);
  }
  for (int i = 0; i < 50 ; i++)
  {
    digitalWrite(STP1, HIGH);
    delayMicroseconds(cj - 200);
    digitalWrite(STP1, LOW);
    delayMicroseconds(cj - 200);
  }


  for (int i = 0; i < ste-50 ; i++)
  {
    digitalWrite(STP1, HIGH);
    delayMicroseconds(ct);
    digitalWrite(STP1, LOW);
    delayMicroseconds(ct);
  }
    for (int i = 0; i < 50 ; i++)
  {
    digitalWrite(STP1, HIGH);
    delayMicroseconds(cj);
    digitalWrite(STP1, LOW);
    delayMicroseconds(cj);
  }
}



//机构行走
void backward(int Step)     //前进
{
  digitalWrite(ENA3, HIGH);
  digitalWrite(DIR3, LOW);

  digitalWrite(ENA4, HIGH);
  digitalWrite(DIR4, HIGH);

  for (int i = 0; i < Step ; i++)
  {
    digitalWrite(STP3, HIGH);
    digitalWrite(STP4, HIGH);
    delayMicroseconds(SPD2);
    digitalWrite(STP3, LOW);
    digitalWrite(STP4, LOW);
    delayMicroseconds(SPD2);
  }
}


void forward1(int Step)     //后退
{
  int Speed1 = 150;
  digitalWrite(ENA3, HIGH);
  digitalWrite(DIR3, HIGH);

  digitalWrite(ENA4, HIGH);
  digitalWrite(DIR4, LOW);

  for (int i = 0; i < Step ; i++)
  {
    digitalWrite(STP3, HIGH);
    digitalWrite(STP4, HIGH);
    delayMicroseconds(Speed1);
    digitalWrite(STP3, LOW);
    digitalWrite(STP4, LOW);
    delayMicroseconds(Speed1);
  }
}

void forward(int Step,int f)      //后退
{
  
  int Speed1 = 150;
  if(f==0){
  digitalWrite(ENA3, HIGH);
  digitalWrite(DIR3, HIGH);

  digitalWrite(ENA4, HIGH);
  digitalWrite(DIR4, LOW);
  }
  else {
      digitalWrite(ENA3, HIGH);
  digitalWrite(DIR3, LOW);

  digitalWrite(ENA4, HIGH);
  digitalWrite(DIR4, HIGH);
  }
  for (int i = 0; i < Step ; i++)
  {
    for(int jj=0;jj<40;jj++)
    {
    digitalWrite(STP3, HIGH);
    digitalWrite(STP4, HIGH);
    delayMicroseconds(Speed1);
    digitalWrite(STP3, LOW);
    digitalWrite(STP4, LOW);
    delayMicroseconds(Speed1);
    }


    digitalWrite(STP3, HIGH);
    digitalWrite(STP4, LOW);
    delayMicroseconds(Speed1);
    digitalWrite(STP3, LOW);
    digitalWrite(STP4, LOW);
    delayMicroseconds(Speed1);
  }
}

void rotfan(int Step)        //向右
{
  digitalWrite(ENA4, HIGH);
  digitalWrite(DIR4, LOW);

  digitalWrite(ENA3, HIGH);
  digitalWrite(DIR3, LOW);

  for (int i = 0; i < Step ; i++)
  {
    digitalWrite(STP3, HIGH);
    digitalWrite(STP4, HIGH);
    delayMicroseconds(SPD2);
    digitalWrite(STP3, LOW);
    digitalWrite(STP4, LOW);
    delayMicroseconds(SPD2);
  }
}



void right(int Step)        //向右
{
  digitalWrite(ENA3, HIGH);
  digitalWrite(DIR3, HIGH);

  digitalWrite(ENA4, HIGH);
  digitalWrite(DIR4, LOW);

  for (int i = 0; i < Step ; i++)
  {
    digitalWrite(STP3, HIGH);
    //digitalWrite(STP4, HIGH);
    delayMicroseconds(SPD2);
    digitalWrite(STP3, LOW);
    //digitalWrite(STP4, LOW);
    delayMicroseconds(SPD2);
  }
}

void left(int Step)         //向左
{
  digitalWrite(ENA3, HIGH);
  digitalWrite(DIR3, HIGH);

  digitalWrite(ENA4, HIGH);
  digitalWrite(DIR4, LOW);

  for (int i = 0; i < Step ; i++)
  {
    //digitalWrite(STP3, HIGH);
    digitalWrite(STP4, HIGH);
    delayMicroseconds(SPD2);
    //digitalWrite(STP3, LOW);
    digitalWrite(STP4, LOW);
    delayMicroseconds(SPD2);
  }
}


void left1(int Step)//向左
{
  digitalWrite(ENA3, HIGH);
  digitalWrite(DIR3, HIGH);

  //digitalWrite(ENA4, HIGH);
  //digitalWrite(DIR4, LOW);

  for (int i = 0; i < Step ; i++)
  {
    digitalWrite(STP3, HIGH);
    //digitalWrite(STP4, HIGH);
    delayMicroseconds(SPD2);
    digitalWrite(STP3, LOW);
    //digitalWrite(STP4, LOW);
    delayMicroseconds(SPD2);
  }
}



void count9() {

    Serial1.begin(115200);
  Serial1.write(0XA5);
  Serial1.write(0X52); 
  //delay(1000);
  Serial1.write(0XA5);
  Serial1.write(0X55); 
  delay(2000);
    //forward(300,0);
  forward1(2700);
  for(int tt=0;tt<=100;tt++)
  serialEvent();
  int flag_a=1;
  int ang=0;

  while(1)
  {
  serialEvent();
  if(sign)
  {  
     sign=0;
     if(Re_buf[0]==0xAA && Re_buf[7]==0x55)        //检查帧头，帧尾
     {                 
            YPR[0]=(Re_buf[1]<<8|Re_buf[2])/100;   //合成数据，去掉小数点后2位
           // Serial.print("YPR[0]: ");
            //Serial.println(YPR[0]);      //显示航向
     }
     if(flag_a)
     {
      ang=YPR[0];
      flag_a=0;
      //forward(500,0);
     }
  }
  //Serial.println(abs(YPR[0]-ang)); 
  if(anglediff(YPR[0],ang)>=88)
  break;
  right(30);
  }
  
  forward1(10200);
  
    while(1)
  {
  serialEvent();
  if(sign)
  {  
     sign=0;
     if(Re_buf[0]==0xAA && Re_buf[7]==0x55)        //检查帧头，帧尾
     {                 
            YPR[0]=(Re_buf[1]<<8|Re_buf[2])/100;   //合成数据，去掉小数点后2位
           // Serial.print("YPR[0]: ");
            //Serial.println(YPR[0]);      //显示航向
     }
     if(flag_a)
     {
      ang=YPR[0];
      flag_a=0;
     }
  }
  //Serial.println(abs(YPR[0]-ang)); 
  if(anglediff(YPR[0],ang)<=2)
  break;
  rotfan(15);
  }

  //rotfan(30);
  //backward(500);
  forward(100,1);
  rotfan(70);
  rotfan(30);
//    forward(500,0);
//      forward(500,1);
  //while(1);
 

}

//机构伸展
void color(int number,int brightness,int colour) //colour 0==white 1==yellow 2==green
{
  int location=number*5;
  brightness=5;
  if (colour==0){
  strip.setPixelColor(location,brightness,brightness,brightness);
  strip.setPixelColor(location+1,brightness,brightness,brightness);
  strip.setPixelColor(location+2,brightness,brightness,brightness);
  }
  else if (colour==1){
  strip.setPixelColor(location,brightness,brightness,0);
  strip.setPixelColor(location+1,brightness,brightness,0);
  strip.setPixelColor(location+2,brightness,brightness,0);
  }
  else{
  strip.setPixelColor(location,0,brightness,0);
  strip.setPixelColor(location+1,0,brightness,0);
  strip.setPixelColor(location+2,0,brightness,0);
  }
  
}

void rgb(int w,int y,int green)
{
  for(int gg=0;gg<w;gg++)
  {
    color(gg,1,0);
  }
  for(int gg=w;gg<w+y;gg++)
  {
    color(gg,1,1);
  }

  for(int gg=y+w;gg<w+y+green;gg++)
  {
    color(gg,1,2);
  }
  strip.show();
}
//-------------------初始化-------------
void setup() {
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  strip.begin();
  strip.show();
  pinMode(duoji2, OUTPUT);
  digitalWrite(duoji2, LOW);//先保证拉低
  Serial.begin(9600);


  pinMode(STP1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(ENA1, OUTPUT);

  pinMode(STP3, OUTPUT);
  pinMode(DIR3, OUTPUT);
  pinMode(ENA3, OUTPUT);

  pinMode(STP4, OUTPUT);
  pinMode(DIR4, OUTPUT);
  pinMode(ENA4, OUTPUT);
  pinMode(28, INPUT);

  digitalWrite(ENA1, LOW);
  digitalWrite(ENA3, LOW);
  digitalWrite(ENA4, LOW);
//  while(1){
//  rgb(2,2,2);
//  }
  while (1) {
    if (Serial.read() == 'B')
      break;
  }
    while (1) {
    if (digitalRead(A10) == HIGH)
      break;
  }

//  while(1)
//  {
//      forward1(5000);
//  }
  
//  while(1)
//  {
//    
//
//      if (Serial.available()){
//        int bb;
//        bb = Serial.parseInt();
//              bb=-bb;            
//          bb=bb/10;
//          bb+=180;
//          rotation(2, bb);
//          rotation(2, bb);
//            digitalWrite(duoji2, LOW);//先保证拉低
//      }
//  }
   //shangxia(500, 0);
 shangxia(3000, 1);
 // while(1);
 //count9();
    rotation(2, spp);
      digitalWrite(duoji2, LOW);//先保证拉低
  forward1(5000);
}

//----------------loop循环---------------
void loop()
{

  forward(2,flag_t);
  

  if (digitalRead(pillar) == 0)
  {

    
          if(flag_t==0)
          {
            
          }
          //forward(2,flag_t);
          else {
          forward(5,flag_t);
          delay(500);
          }
    digitalWrite(ENA3, HIGH);
    digitalWrite(ENA4, HIGH);

    Serial.println("H");
    Serial.read();
    Serial.read();
     char ff;
     while(1){
     int gb;
        //Serial.print("RGB");
        ff = Serial.read();
        if(ff=='D')
        {
        gb = Serial.parseInt();
        //Serial.println(gb);
        int www=gb/100;
        int yyy=(gb%100)/10;
        int ggg=gb%10;

        if(count<16)
        rgb(www,yyy,ggg);
        break;
        }
        
     }
    while (1)
    {
      int bb = 0;
      int ff1 = 500;
      int ff2 = 550;


      if (Serial.available())   //串口收到信息
      {

        //Serial.print("BE");
        ff = Serial.read();
        if (ff == 'W')
        {
          ff1 = 1000;
          ff2 = 1030;

        }
        else if (ff == 'Y')
        {
          ff1 = 800;
          ff2 = 830;

        }

        else{}

        bb = Serial.parseInt();
        //Serial.println(bb);
        
        if (bb == -1)
        {
          bb = spp;
          rotation(2, bb);
          rotation(2, bb);
            digitalWrite(duoji2, LOW);//先保证拉低
          if(flag_t==0)
          forward(70,flag_t);
          else 
          forward(150,flag_t);
          count++;
          if (count == 9)
          {
            count9();
            flag_t=1;
          }
          
          if (count == 14)
          {
            rgb(0,0,8);
            while (1) {}
            
          }
          break;
        }
        else 
        {   
          if(count!=9)
          {
          bb=-bb;            
          bb=bb/10;
          bb+=180;
          rotation(2, bb);
          rotation(2, bb);
            digitalWrite(duoji2, LOW);//先保证拉低

          shangxia(ff1, 0); 
          delay(1000);
          shangxia(ff2, 1);
          }
        }

      }
    }
           
  }
}
