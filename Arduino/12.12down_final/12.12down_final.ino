/**************************参数定义***********************************/
#define SPEED  80   //全局变量，通过调节SPEED值来控制电机转速。  注意！！！SPEED值越小速度越快！
#define TRACKSPEED 200 //全局变量，通过调节TRACKSPEED值来控制电机转速。  注意！！！TRACKSPEED值越小循迹速度越快！

#define TraceYes 0
#define TraceNo 1

#define LaserYes 1
#define LaserNo 0

/**************************引脚定义***********************************/
#define IN1_F1  48
#define IN2_F1  24
#define ENA_F1  5

#define IN1_R1  50
#define IN2_R1  26
#define ENA_R1  44

#define IN1_F2  52
#define IN2_F2  28
#define ENA_F2  4

#define IN1_R2  47
#define IN2_R2  22
#define ENA_R2  46

#define Laser1 11   //定义激光传感器1接口引脚 11
#define Laser2 13   //定义激光传感器2接口引脚 13
#define Laser3 15   //定义激光传感器3接口引脚 15
#define Laser4 17   //定义激光传感器4接口引脚 17

#define Real_Laser1 A12
#define Real_Laser2 A0

#define signal_EN 49   //Arduino通讯引脚
#define signal_1 51
#define signal_2 53

#define QRcode_1 16  //OpenMV通讯引脚
#define QRcode_2 19
#define QRcode_3 18

#define onfield 18
#define reallyonfield 19

#define treestick 0

/************************************包含语言************************************************/
#include <Arduino.h>
#include <LiquidCrystal.h>//lcd
#include <SoftwareSerial.h>
#include <Servo.h>
#include <String.h>

/****************************************机器人简图*********************************************/
//           
//        机器人简图
//    F1 |  |-------------|   
//       |  |      R1 —— |  laser1
//          |             |
//          |红外4         |红外1
//          |             |
//          |             |
//          |红外3         |红外2
//          | —— R2      |  |
//  laser2  |-------------|  |  F2
//     
//        F1、F2、R1、R2表示电机，横线方向表示轮子方向
//        F1表示轮子方向向前进的第一个轮子，F2表示轮子方向向前进的第二个轮子
//        R1表示轮子方向向右的第一个轮子，F2表示轮子方向向右的第二个轮子

//
//  digitalWrite(IN1,LOW);//正对轮子，顺时针转动
//  digitalWrite(IN2,HIGH);//
//  digitalWrite(IN1,HIGH);//正对轮子，逆时针转动
//  digitalWrite(IN2,LOW);//
//
/*****************************************函数定义*******************************************/

int flag=1;
int item=2;
int a=0;

/******************************************测试与停止****************************************/

void test_all(){
  digitalWrite(IN1_F1,HIGH);//F1逆时针转
  digitalWrite(IN2_F1,LOW);
  
  digitalWrite(IN1_F2,LOW);//F2顺时针转
  digitalWrite(IN2_F2,HIGH);

  digitalWrite(IN1_R1,LOW);//R1顺时针转
  digitalWrite(IN2_R1,HIGH);
  
  digitalWrite(IN1_R2,HIGH);//R2逆时针转
  digitalWrite(IN2_R2,LOW);
  
  analogWrite(ENA_F1,255);
  analogWrite(ENA_F2,255);
  analogWrite(ENA_R1,255);
  analogWrite(ENA_R2,255);
}

void Stop(){
  digitalWrite(IN1_F2,LOW);//F1顺时针转
  digitalWrite(IN2_F2,HIGH);

  digitalWrite(IN1_F2,LOW);//F2顺时针转
  digitalWrite(IN2_F2,HIGH);

  digitalWrite(IN1_R1,LOW);//R1顺时针转
  digitalWrite(IN2_R1,HIGH);
  
  digitalWrite(IN1_R2,HIGH);//R2逆时针转
  digitalWrite(IN2_R2,LOW);
  
  analogWrite(ENA_F1,0);
  analogWrite(ENA_F2,0);
  analogWrite(ENA_R1,0);
  analogWrite(ENA_R2,0);
}

/**********************************************************************************************/

void GoForward(int T)      //F1和F2转速相同，机器人整体向前进
{
  digitalWrite(IN1_F1,HIGH);//F1逆时针转
  digitalWrite(IN2_F1,LOW);
  
  digitalWrite(IN1_F2,LOW);//F1顺时针转
  digitalWrite(IN2_F2,HIGH);
  
  analogWrite(ENA_F1,120);
  analogWrite(ENA_F2,120);
  delay(T);
  analogWrite(ENA_F1,0);
  analogWrite(ENA_F2,0);
  
}

void GoBackward(int T)     //F1和F2转速相同，机器人整体向后退（电机转向与上一个函数相反）
{
  digitalWrite(IN1_F1,LOW);//F1顺时针转
  digitalWrite(IN2_F1,HIGH);
  
  digitalWrite(IN1_F2,HIGH);//F2逆时针转
  digitalWrite(IN2_F2,LOW);
  
  analogWrite(ENA_F1,120);
  analogWrite(ENA_F2,120);
  delay(T);
  analogWrite(ENA_F1,0);
  analogWrite(ENA_F2,0);
}


void GoLeft(int T)     //R1和R2转速相同，机器人整体向左行驶,定时停止
{
  digitalWrite(IN1_R1,HIGH);//R1逆时针转
  digitalWrite(IN2_R1,LOW);
  
  digitalWrite(IN1_R2,LOW);//R2顺时针转
  digitalWrite(IN2_R2,HIGH);
  
  analogWrite(ENA_R1,255);
  analogWrite(ENA_R2,255);
  delay(T);
  analogWrite(ENA_R1,0);
  analogWrite(ENA_R2,0);
}

void GoRight(int T)     //R1和R2转速相同，机器人整体向右行驶
{
  digitalWrite(IN1_R1,LOW);//R1顺时针转
  digitalWrite(IN2_R1,HIGH);
  
  digitalWrite(IN1_R2,HIGH);//R2逆时针转
  digitalWrite(IN2_R2,LOW);
  
  analogWrite(ENA_R1,255);
  analogWrite(ENA_R2,255);
  delay(T);
  analogWrite(ENA_R1,0);
  analogWrite(ENA_R2,0);
}

/************************************************************************************************/
void TrackFieldRightR1R2(int T){   //在田垄上车子R1比R2快，差速
  
  digitalWrite(IN1_R1,LOW);//R1顺时针转
  digitalWrite(IN2_R1,HIGH);
  
  digitalWrite(IN1_R2,HIGH);//R2逆时针转
  digitalWrite(IN2_R2,LOW);

  analogWrite(ENA_R1,255);
  analogWrite(ENA_R2,120);
  
  delay(T);
  analogWrite(ENA_R2,255);
}

void TrackFieldRightR2R1(int T){   //在田垄上车子R2比R1快，差速
    
  digitalWrite(IN1_R1,LOW);//R1顺时针转
  digitalWrite(IN2_R1,HIGH);
  
  digitalWrite(IN1_R2,HIGH);//R2逆时针转
  digitalWrite(IN2_R2,LOW);

  analogWrite(ENA_R2,255);
  analogWrite(ENA_R1,120);

  delay(T);
  analogWrite(ENA_R1,255);
}

void TrackFieldLeftR1R2(int T){   //在田垄上车子R1比R2快，差速
  digitalWrite(IN1_R1,HIGH);//R1逆时针转
  digitalWrite(IN2_R1,LOW);
  
  digitalWrite(IN1_R2,LOW);//R2顺时针转
  digitalWrite(IN2_R2,HIGH);
  
  analogWrite(ENA_R1,255);
  analogWrite(ENA_R2,120);
 
  delay(T);
  analogWrite(ENA_R2,255);
}

void TrackFieldLeftR2R1(int T){   //在田垄上车子R2比R1快，差速;
  digitalWrite(IN1_R1,HIGH);//F1逆时针转
  digitalWrite(IN2_R1,LOW);
  
  digitalWrite(IN1_R2,LOW);//F1顺时针转
  digitalWrite(IN2_R2,HIGH);

  analogWrite(ENA_R2,255);
  analogWrite(ENA_R1,120);
 
  delay(T);
  analogWrite(ENA_R1,255);
}

/***************************************************************************************/

void LaserForwardF1F2(int T){   //循边缘过去，车子F1比F2快，差速
  digitalWrite(IN1_F1,HIGH);//F1逆时针转
  digitalWrite(IN2_F1,LOW);
  
  digitalWrite(IN1_F2,LOW);//F2顺时针转
  digitalWrite(IN2_F2,HIGH);

  analogWrite(ENA_F1,255);
  analogWrite(ENA_F2,150);
  delay(T);
  analogWrite(ENA_F1,255);
  analogWrite(ENA_F2,255);

}


void LaserForwardF2F1(int T){//循边缘过去，车子F2比F1快，差速
  digitalWrite(IN1_F1,HIGH);//F1逆时针转
  digitalWrite(IN2_F1,LOW);
  
  digitalWrite(IN1_F2,LOW);//F2顺时针转
  digitalWrite(IN2_F2,HIGH);

  analogWrite(ENA_F2,255);
  analogWrite(ENA_F1,150);
  delay(T);
  analogWrite(ENA_F1,255);
  analogWrite(ENA_F2,255);
}


void LaserBackwardF1F2(int T){   //循边缘回来，车子F1比F2快，差速
  digitalWrite(IN1_F1,LOW);//F1顺时针转
  digitalWrite(IN2_F1,HIGH);
  
  digitalWrite(IN1_F2,HIGH);//F2逆时针转
  digitalWrite(IN2_F2,LOW);

  analogWrite(ENA_F1,255);
  analogWrite(ENA_F2,150);
  delay(T);
  analogWrite(ENA_F1,255);
  analogWrite(ENA_F2,255);
}

void LaserBackwardF2F1(int T){   //循边缘回来，车子F2比F1快，差速
  digitalWrite(IN1_F1,LOW);//F1顺时针转
  digitalWrite(IN2_F1,HIGH);
  
  digitalWrite(IN1_F2,HIGH);//F2逆时针转
  digitalWrite(IN2_F2,LOW);

  analogWrite(ENA_F2,255);
  analogWrite(ENA_F1,150);
  delay(T);
  analogWrite(ENA_F1,255);
  analogWrite(ENA_F2,255);
}

/*****************************************差速加平移代码*************************************/

void TrackFieldRightR1R2(int T){   //在田垄上车子R1比R2快，差速
  int T1=25;
  digitalWrite(IN1_R1,LOW);//R1顺时针转
  digitalWrite(IN2_R1,HIGH);
  
  digitalWrite(IN1_R2,HIGH);//R2逆时针转
  digitalWrite(IN2_R2,LOW);

  digitalWrite(IN1_F1,LOW);//F1顺时针转
  digitalWrite(IN2_F1,HIGH);
  
  digitalWrite(IN1_F2,HIGH);//F2逆时针转
  digitalWrite(IN2_F2,LOW);

  analogWrite(ENA_R1,255);
  analogWrite(ENA_R2,0);
  
  analogWrite(ENA_F1,255);//R2不动，R1动的同时，F1F2水平右移，增加行驶平稳度
  analogWrite(ENA_F2,255);
  delay(T1);
  analogWrite(ENA_F1,0);
  analogWrite(ENA_F2,0);
  
  delay(T-T1);
  analogWrite(ENA_R1,255);
  analogWrite(ENA_R2,255);
}

/******************************************差速加反转代码**************************************/

void TrackFieldRightR1R2(int i){   //在田垄上车子R1比R2快，差速
  for(int lyy=i;lyy<=1;i++){
     int T1=25;
  digitalWrite(IN1_R1,LOW);//R1顺时针转
  digitalWrite(IN2_R1,HIGH);
  
  digitalWrite(IN1_R2,LOW);//R2顺时针转
  digitalWrite(IN2_R2,HIGH);

  analogWrite(ENA_R1,255);
  analogWrite(ENA_R2,255);
  
  delay(25);

  digitalWrite(IN1_R2,HIGH);//R2逆时针转
  digitalWrite(IN2_R2,LOW);
  
  analogWrite(ENA_R1,255);
  analogWrite(ENA_R2,255);
  }
}

/*********************************************前后循迹差速************************************/

void LaserForwardRight(int step){       //机器人沿边缘远离起点循迹行驶
  int lyy;
  for(lyy=0;lyy<step;lyy++){
    int real_laser1=digitalRead(Real_Laser1);
   
    if(real_laser1==LaserYes){
       LaserForwardF2F1(100);
    }
    else {
      LaserForwardF1F2(100);} 
    }
  }

void LaserForwardLeft(int step){       //机器人沿边缘远离起点循迹行驶
  int lyy;
  for(lyy=0;lyy<step;lyy++){
    int real_laser2=digitalRead(Real_Laser2);
   
    if(real_laser2==LaserYes){
         LaserForwardF2F1(100);
    }
    else {
      LaserForwardF1F2(100);} 
    }
  } 
  
void LaserBackward(int step){       //机器人沿边缘靠近起点循迹行驶
  int lyy;
  for(lyy=0;lyy<step;lyy++){
  int real_laser2=digitalRead(Real_Laser2);
   
    if(real_laser2==LaserYes){
         LaserBackwardF2F1(100);
    }
    else {
      LaserBackwardF1F2(100);} 
  
  }
}

/*****************************************左右循垄代码***************************************/

void TrackRight(int step){       //机器人在田垄上向右循迹行驶
  int lyy;
  for(lyy=0;lyy<step;lyy++){
    int laser1=digitalRead(Laser1);
    int laser2=digitalRead(Laser2);
    int laser3=digitalRead(Laser3);
    int laser4=digitalRead(Laser4);
    //该版本激光探测到物体输出1，无物体输出0，可按照实际情况修改
    if(laser1==TraceNo&& laser2==TraceNo&& laser3==TraceNo&& laser4==TraceNo)   //四个激光都亮，向右走
      {GoRight(25);}
    else if(laser1==TraceNo&& laser2==TraceYes&& laser3==TraceNo&& laser4==TraceYes)   //R2需要比R1转得快才能调回来，差速
      {TrackFieldRightR1R2(25);}
    else if(laser1==TraceYes&& laser2==TraceYes&& laser3==TraceNo&& laser4==TraceYes)   //R2需要比R1转得快才能调回来，差速
      {TrackFieldRightR1R2(25);}
    else if(laser1==TraceNo&& laser2==TraceYes&& laser3==TraceYes&& laser4==TraceYes)   //R2需要比R1转得快才能调回来，差速
      {TrackFieldRightR1R2(25);}

    else if(laser1==TraceYes&& laser2==TraceNo&& laser3==TraceYes&& laser4==TraceNo)   //R1需要比R2转得快才能调回来，差速
      {TrackFieldRightR2R1(25);}
    else if(laser1==TraceYes&& laser2==TraceYes&& laser3==TraceYes&& laser4==TraceNo)   //R1需要比R2转得快才能调回来，差速
      {TrackFieldRightR2R1(25);}
    else if(laser1==TraceYes&& laser2==TraceNo&& laser3==TraceYes&& laser4==TraceYes)   //R1需要比R2转得快才能调回来，差速
      {TrackFieldRightR2R1(25);}
      
    else if(laser1==TraceNo&& laser2==TraceYes&& laser3==TraceYes&& laser4==TraceNo)   // 整体需要向前走，平移
      {TrackFieldRightR1R2(25);}      
    else if(laser1==TraceYes&& laser2==TraceNo&& laser3==TraceNo&& laser4==TraceYes)   // 整体需要向后走，平移
      {TrackFieldRightR2R1(25);}  
    else {GoRight(25);} 
  }
}

void TrackLeft(int step){       //机器人在田垄上向右循迹行驶
  int lyy;
  for(lyy=0;lyy<step;lyy++){
    int laser1=digitalRead(Laser1);
    int laser2=digitalRead(Laser2);
    int laser3=digitalRead(Laser3);
    int laser4=digitalRead(Laser4);
    //该版本激光探测到物体输出1，无物体输出0，可按照实际情况修改
    if(laser1==TraceNo&& laser2==TraceNo&& laser3==TraceNo&& laser4==TraceNo)   //四个激光都亮，向右走
      {GoLeft(25);}
    else if(laser1==TraceNo&& laser2==TraceYes&& laser3==TraceNo&& laser4==TraceYes)   //R2需要比R1转得快才能调回来，差速
      {TrackFieldLeftR2R1(25);}
    else if(laser1==TraceYes&& laser2==TraceYes&& laser3==TraceNo&& laser4==TraceYes)   //R2需要比R1转得快才能调回来，差速
      {TrackFieldLeftR2R1(25);}
    else if(laser1==TraceNo&& laser2==TraceYes&& laser3==TraceYes&& laser4==TraceYes)   //R2需要比R1转得快才能调回来，差速
      {TrackFieldLeftR2R1(25);}

    else if(laser1==TraceYes&& laser2==TraceNo&& laser3==TraceYes&& laser4==TraceNo)   //R1需要比R2转得快才能调回来，差速
      {TrackFieldLeftR1R2(25);}
    else if(laser1==TraceYes&& laser2==TraceYes&& laser3==TraceYes&& laser4==TraceNo)   //R1需要比R2转得快才能调回来，差速
      {TrackFieldLeftR1R2(25);}
    else if(laser1==TraceYes&& laser2==TraceNo&& laser3==TraceYes&& laser4==TraceYes)   //R1需要比R2转得快才能调回来，差速
      {TrackFieldLeftR1R2(25);}
      
    else if(laser1==TraceNo&& laser2==TraceYes&& laser3==TraceYes&& laser4==TraceNo)   // 整体需要向前走，平移
      {TrackFieldLeftR1R2(25);}      
    else if(laser1==TraceYes&& laser2==TraceNo&& laser3==TraceNo&& laser4==TraceYes)   // 整体需要向后走，平移
      {TrackFieldLeftR2R1(25);}  
    else {GoLeft(25);} 
  }
}

void adjust_back(int T){                      //向左循迹，检测到树后适当慢速回退
  digitalWrite(IN1_R1,LOW);//R1顺时针转
  digitalWrite(IN2_R1,HIGH);
  
  digitalWrite(IN1_R2,HIGH);//R2逆时针转
  digitalWrite(IN2_R2,LOW);
  
  analogWrite(ENA_R1,120);
  analogWrite(ENA_R2,120);
  delay(T);
  analogWrite(ENA_R1,0);
  analogWrite(ENA_R2,0);
}

/**************************************************开始总代码*****************************************/

void setup() {
  // put your setup code here, to run once:
  pinMode(IN1_R1,OUTPUT);
  pinMode(IN2_R1,OUTPUT);
  pinMode(ENA_R1,OUTPUT);
  pinMode(IN1_F1,OUTPUT);
  pinMode(IN2_F1,OUTPUT);
  pinMode(ENA_F1,OUTPUT);
  pinMode(IN1_R2,OUTPUT);
  pinMode(IN2_R2,OUTPUT);
  pinMode(ENA_R2,OUTPUT);
  pinMode(IN1_F2,OUTPUT);
  pinMode(IN2_F2,OUTPUT);
  pinMode(ENA_F2,OUTPUT);
  
  digitalWrite(IN1_R1,LOW);
  digitalWrite(IN2_R1,HIGH);
  digitalWrite(IN1_R2,LOW);
  digitalWrite(IN2_R2,HIGH);
  digitalWrite(IN1_F1,HIGH);
  digitalWrite(IN2_F1,LOW);
  digitalWrite(IN1_F2,LOW);
  digitalWrite(IN2_F2,HIGH);

  pinMode(onfield,OUTPUT);
  pinMode(reallyonfield,OUTPUT);
  
  digitalWrite(onfield,LOW);
  digitalWrite(reallyonfield,HIGH);
   
}

/****************************************************循环总代码****************************************/

void loop() {
  if(item==1){
    while(1){
      if(digitalRead(QRcode_1)==0 && digitalRead(QRcode_2)==0&&digitalRead(QRcode_3)==1){
        a=1;
        item=2;
        break;
      }
      else if(digitalRead(QRcode_1)==0 && digitalRead(QRcode_2)==1&&digitalRead(QRcode_3)==0){
        a=2;
        item=2;
        break;
      }
      else if(digitalRead(QRcode_1)==0 && digitalRead(QRcode_2)==1&&digitalRead(QRcode_3)==1){
        a=3;
        item=2;
        break;
      }
      else if(digitalRead(QRcode_1)==1 && digitalRead(QRcode_2)==0&&digitalRead(QRcode_3)==0){
        a=4;
        item=2;
        break;
      }
      else if(digitalRead(QRcode_1)==1 && digitalRead(QRcode_2)==0&&digitalRead(QRcode_3)==1){
        a=5;
        item=2;
        break;
      }
    }
  }//扫二维码，判断第几条垄不用播种
  
  
  if(item==2){ //已扫码
  
      int laser1=digitalRead(Laser1);
      int laser2=digitalRead(Laser2);
      int laser3=digitalRead(Laser3);
      int laser4=digitalRead(Laser4);
  
      int real_laser1=digitalRead(Real_Laser1);//F1处激光
      int real_laser2=digitalRead(Real_Laser2);//F2处激光
      
      int tree=0;
  
  
    if(flag%2==1&&flag<6&&flag!=a){
      LaserForwardLeft(13);

      while(true){
        LaserForwardLeft(1);
        
        if(digitalRead(Real_Laser1)==LaserNo){ 
          delay(10);
          if(digitalRead(Real_Laser1)==LaserNo){ 
            LaserForwardLeft(2);   //记得改一下往前走的时间（距离）
            Stop();
            break;             
          }
        else { LaserForwardLeft(1); }   //多次确认以免错误信号
        }
      }         //直走直至到垄

      GoRight(500);
      while(true){
        TrackRight(1);
        if(digitalRead(treestick)==1){
          if((digitalRead(laser1)==0)&&(digitalRead(laser4)==0)&&(digitalRead(laser2)==1)&&(digitalRead(laser3)==1)){
            while(1){
              GoForward(1);
              if((digitalRead(laser2)==0)||(digitalRead(laser3)==0)){
                break;
              }
            }
          }
          else if((digitalRead(laser1)==1)&&(digitalRead(laser4)==1)&&(digitalRead(laser2)==0)&&(digitalRead(laser3)==0)){
            while(1){
              GoBackward(1);
              if((digitalRead(laser1)==0)||(digitalRead(laser4)==0)){
                break;
              }
            }  
          }
          
            Stop();     
            tree++;
            delay(50);
            if(digitalRead(treestick)==1){
            digitalWrite(onfield,HIGH);
            digitalWrite(reallyonfield,LOW);
            while(1){
              if(digitalRead(signal_EN)==1){
                if(digitalRead(signal_1)==1&&digitalRead(signal_2)==0){
                  digitalWrite(onfield,LOW);
                  digitalWrite(reallyonfield,HIGH);
                  TrackRight(50);
                  break;
                } 
              }

            }
              
            }
            else{
              TrackLeft(1);
              if(digitalRead(treestick)==1){
                digitalWrite(onfield,HIGH);
                digitalWrite(reallyonfield,LOW);
                while(1){
                  if(digitalRead(signal_EN)==1){
                    if(digitalRead(signal_1)==1&&digitalRead(signal_2)==0){
                      digitalWrite(onfield,LOW);
                      digitalWrite(reallyonfield,HIGH);
                      TrackRight(50);
                      break;
                    } 
                  }
    
                }
              }
            }

        }
        
        if(((digitalRead(Laser1)+digitalRead(Laser2)+digitalRead(Laser3)+digitalRead(Laser4))==4)&&tree>0){
          flag++;
          delay(600);
          tree=0;
          Stop();          
          break;
          
        }
        
      }     //向右寻垄直至出垄
    }       //奇数结束
    
    if(flag%2==0&&flag<6&&flag!=a){
      LaserForwardRight(24);
      while(true){
        LaserForwardRight(1);
        if(digitalRead(Real_Laser2)==LaserYes){
          delay(10);
          if(digitalRead(Real_Laser2)==LaserYes){
            LaserBackward(9);//记得改一下往后退的时间（距离）
            Stop();
            break;             
          }
        else { LaserForwardRight(1); }
        }
      }//直走直至到垄
      
      GoLeft(500);

      while(true){
        TrackLeft(1);
        if(digitalRead(treestick)==1){
          if(digitalRead(treestick)==1){
          if((digitalRead(laser1)==0)&&(digitalRead(laser4)==0)&&(digitalRead(laser2)==1)&&(digitalRead(laser3)==1)){
            while(1){
              GoForward(1);
              if((digitalRead(laser2)==0)||(digitalRead(laser3)==0)){
                break;
              }
            }
          }
          
          else if((digitalRead(laser1)==1)&&(digitalRead(laser4)==1)&&(digitalRead(laser2)==0)&&(digitalRead(laser3)==0)){
            while(1){
              GoBackward(1);
              if((digitalRead(laser1)==0)||(digitalRead(laser4)==0)){
                break;
              }
            }  
          }
        }
            Stop();     
            tree++;
            delay(50);
            adjust_back(500);
            Stop();  
            digitalWrite(onfield,HIGH);
            digitalWrite(reallyonfield,LOW);
            while(1){
              if(digitalRead(signal_EN)==1){
                if(digitalRead(signal_1)==1&&digitalRead(signal_2)==0){
                  digitalWrite(onfield,LOW);
                  digitalWrite(reallyonfield,HIGH);
                  TrackLeft(50);
                  break;
                } 
              }

            }
              
            
//            else{
//              TrackRight(1);
//              if(digitalRead(treestick)==1){
//                digitalWrite(onfield,HIGH);
//                digitalWrite(reallyonfield,LOW);
//                while(1){
//                  if(digitalRead(signal_EN)==1){
//                    if(digitalRead(signal_1)==1&&digitalRead(signal_2)==0){
//                      digitalWrite(onfield,LOW);
//                      digitalWrite(reallyonfield,HIGH);
//                      TrackLeft(50);
//                      break;
//                    } 
//                  }
//    
//                }
//              }
//            }

        }
            
        if(((digitalRead(Laser1)+digitalRead(Laser2)+digitalRead(Laser3)+digitalRead(Laser4))==4)&&tree>0){
          flag++;
    
          delay(600);
          tree=0;
          Stop();
          break;         
        }
          
      }//向左寻垄直至出垄  
    }//偶数结束

    
    if(flag%2==1&&flag<6&&flag==a){
      LaserForwardLeft(13);
      while(true){
        LaserForwardLeft(1);
        if(digitalRead(Real_Laser1)==LaserNo){ 
          delay(10);
          if(digitalRead(Real_Laser1)==LaserNo){ 
            LaserForwardLeft(2);//记得改一下往后退的时间（距离）
            Stop();
            break;             
          }
        else { LaserForwardLeft(1); }//多次确认以免错误信号
        }
      }
      
      GoRight(500);
  
      while(true){
        TrackRight(1);
        if((digitalRead(Laser1)+digitalRead(Laser2)+digitalRead(Laser3)+digitalRead(Laser4))==4){
          flag++;       
          delay(600);
          tree=0;
          Stop();  
          break;
        }
      }
    }
    
    if(flag%2==0&&flag<6&&flag==a){
     
      LaserForwardRight(24);
      while(true){
        LaserForwardRight(1);
        if(digitalRead(Real_Laser2)==LaserYes){ 
          delay(10);
          if(digitalRead(Real_Laser2)==LaserYes){ 
            LaserBackward(9);//记得改一下往后退的时间（距离）
            Stop();
            break;             
          }
        else { LaserForwardRight(1); }//多次确认以免错误信号
        }
      }

      GoLeft(500); 
      while(true){
        TrackLeft(1);
        if((digitalRead(Laser1)+digitalRead(Laser2)+digitalRead(Laser3)+digitalRead(Laser4))==4){
          flag++;
          delay(600);
          Stop();  
          tree=0;
          break;
        }
      }
    }
    
    if(flag==6){    //最后一遍原路返回

      GoLeft(500);
      while(true){
        TrackLeft(1);
          //上层
        if((digitalRead(Laser1)+digitalRead(Laser2)+digitalRead(Laser3)+digitalRead(Laser4))==4){
          flag++;
          delay(600);
          Stop();
          break;
        }
      }//向左寻垄直至出垄
    }
    
    if(flag==7){
  
      while(true){
        LaserBackward(1);
//        if(digitalRead(stop)==1){
//          Stop();//走到终止区  
//        }
      }  
    }
  }         //item=2结束
}         //loop结束
