#define DIR2 16 //HIGH向前，LOW向后
#define STP2 19
#define ENA2 18

void Track_PWM(int Pin, int delay_us) {
    //产生一定频率的 PWM 波来驱动步进电机
    //Pin 产生 PWM 的引脚 delay_ms 影响 PWM 的频率 一般设置 100 开始调
    //delay_ms 数值越小，速度越快，但是速度过快会打滑，需要自己把控
    digitalWrite(Pin, HIGH);                 //将指定的引脚置高电平
    delayMicroseconds(delay_us);             //持续高电平 delay_us 时间（单位是微秒）
    digitalWrite(Pin, LOW);                  //将指定的引脚置低电平
    delayMicroseconds(delay_us);             //持续低电平 delay_us 时间（单位是微秒）
}

void setup() {
  // put your setup code here, to run once:
    pinMode(DIR2,OUTPUT);
    pinMode(STP2,OUTPUT);
    pinMode(ENA2,OUTPUT);

     digitalWrite(ENA2,HIGH);
    digitalWrite(DIR2,LOW);
}

void loop() {

      for( int i = 0;i<=1000 ;i++)
    {

        Track_PWM(STP2,300); 
    }

}
