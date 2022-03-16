
#define DIR1 4
#define STP1 5
#define ENA1 7

#define DIR2 16
#define STP2 19
#define ENA2 18


#define DIR3 46
#define STP3 44
#define ENA3 42


#define DIR4 49
#define STP4 51
#define ENA4 53


void setup() {

    pinMode(DIR1,OUTPUT);
    pinMode(STP1,OUTPUT);
    pinMode(ENA1,OUTPUT);

    pinMode(DIR2,OUTPUT);
    pinMode(STP2,OUTPUT);
    pinMode(ENA2,OUTPUT);

    pinMode(DIR3,OUTPUT);
    pinMode(STP3,OUTPUT);
    pinMode(ENA3,OUTPUT);

    pinMode(DIR4,OUTPUT);
    pinMode(STP4,OUTPUT);
    pinMode(ENA4,OUTPUT);
    
}

void Track_PWM(int Pin, int delay_us) {
    //产生一定频率的 PWM 波来驱动步进电机
    //Pin 产生 PWM 的引脚 delay_ms 影响 PWM 的频率 一般设置 100 开始调
    //delay_ms 数值越小，速度越快，但是速度过快会打滑，需要自己把控
    digitalWrite(Pin, HIGH);                 //将指定的引脚置高电平
    delayMicroseconds(delay_us);             //持续高电平 delay_us 时间（单位是微秒）
    digitalWrite(Pin, LOW);                  //将指定的引脚置低电平
    delayMicroseconds(delay_us);             //持续低电平 delay_us 时间（单位是微秒）

}

void forward(int step,int speed){
    digitalWrite(ENA4,HIGH); //使能端HIGH,轮子中止抱死状态
    digitalWrite(DIR4,HIGH); //DIR设定方向

    for( int i = 0;i<=step ;i++)
    {
        Track_PWM(STP4, speed); 
    }

}

void loop(){

    forward(100,80);
}
