 //舵机控制测试

// 需要外接电源 +5V - +12V
// 舵机接5+ arduino.GND  型号端接 arduino.PWM 9
#include <Servo.h>
Servo myservo2; // 定义舵机变量名
void setup()
{
  myservo2.attach(2 ); // 定义舵机接口，9或10
  Serial.begin(9600);   //  打开串口，设置波特率为9600 bps
}
void loop()
{
   for( int i= 150;i>=45;i=i-20)
  {
    myservo2.write(i); // 设置舵机旋转的角度
    delay( 200);  
  }
}
