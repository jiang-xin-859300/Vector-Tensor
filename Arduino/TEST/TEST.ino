//右前轮 Left Front (abbr LF)，对应arduino板步进1
#define DIR1 4 //HIGH向前，LOW向后
#define STP1 5
#define ENA1 7

//左前轮 Right Front (abbr RF)，对应步进2
#define DIR2 16 //HIGH向前，LOW向后
#define STP2 19
#define ENA2 18

//右后轮 Right Rear (abbr RR)，对应步进4
#define DIR3 46 //HIGH向前，LOW向后
#define STP3 44
#define ENA3 42

//左后轮 Left Rear (abbr LR)，对应步进3
#define DIR4 49 //LOW向前，HIGH向后
#define STP4 51
#define ENA4 53

/*******************Declaration Part**************************/
void go_forward(int step, int speed); //speed数值越大，速度越慢
void go_backward(int step, int speed);
void go_sideway_left(int step, int speed);
void go_sideway_right(int step, int speed);
void test_all();
/*************************************************************/

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

void loop(){
      //设置四个轮子的方向DIR向前，使能端ENA不抱死
//    set_LF(LOW, HIGH);
//    set_RF(HIGH, HIGH);
//    set_LR(HIGH, LOW);
//    set_RR(LOW, HIGH);
//
//    //驱动电机
//    for( int i = 0;i<=300 ;i++)
//    {
//        Track_PWM(STP4, 300); 
//       
//    }

      for(int j=0;j<=6;j++){
          go_backward(3000,300);
      }


       for(int j=0;j<=20;j++)
       {
           go_sideway_left(500,300);
       }
    
      for(int j=0;j<=6;j++){
          go_forward(3000,300);
      }

      while(1){
        delay(10);
      }
    
}



/**************************以下函数确定四个轮子的旋转方向(DIR)，和使能端(ENA)状态**************************/
//Left Front
void set_LF(int ENAstate, int DIRstate){
    digitalWrite(ENA2, ENAstate); //使能端HIGH,轮子中止抱死状态
    digitalWrite(DIR2, DIRstate); //DIR设定方向
}

//Right Front
void set_RF(int ENAstate, int DIRstate){
    digitalWrite(ENA1, ENAstate); //使能端HIGH,轮子中止抱死状态
    digitalWrite(DIR1, DIRstate); //DIR设定方向
}

//Left Rear
void set_LR(int ENAstate, int DIRstate){
    digitalWrite(ENA4, ENAstate); //使能端HIGH,轮子中止抱死状态
    digitalWrite(DIR4, DIRstate); //DIR设定方向
}

//Right Rear
void set_RR(int ENAstate, int DIRstate){
    digitalWrite(ENA3, ENAstate); //使能端HIGH,轮子中止抱死状态
    digitalWrite(DIR3, DIRstate); //DIR设定方向
}

/****************************************************/

void Track_PWM(int Pin, int delay_us) {
    //产生一定频率的 PWM 波来驱动步进电机
    //Pin 产生 PWM 的引脚 delay_ms 影响 PWM 的频率 一般设置 100 开始调
    //delay_ms 数值越小，速度越快，但是速度过快会打滑，需要自己把控
    digitalWrite(Pin, HIGH);                 //将指定的引脚置高电平
    delayMicroseconds(delay_us);             //持续高电平 delay_us 时间（单位是微秒）
    digitalWrite(Pin, LOW);                  //将指定的引脚置低电平
    delayMicroseconds(delay_us);             //持续低电平 delay_us 时间（单位是微秒）

}


//前进
void go_forward(int step, int speed){

    //设置四个轮子的方向DIR向前，使能端ENA不抱死
    set_LF(LOW, HIGH);
    set_RF(HIGH, HIGH);
    set_LR(HIGH, LOW);
    set_RR(LOW, HIGH);

    //驱动电机
    for( int i = 0;i<=step ;i++)
    {
        Track_PWM(STP1, speed); 
        Track_PWM(STP4, speed);         
    }

}

//后退
void go_backward(int step, int speed){

    //设置四个轮子的方向DIR向前，使能端ENA不抱死
    set_LF(LOW, LOW);
    set_RF(HIGH, LOW);
    set_LR(HIGH, HIGH);
    set_RR(LOW, LOW);

    //驱动电机
    for( int i = 0;i<=step ;i++)
    {
        Track_PWM(STP1, speed); 
        Track_PWM(STP4, speed);         
    }

}

//向左平移
void go_sideway_left(int step, int speed){
    set_LF(HIGH,HIGH);
    set_RF(LOW, LOW);
    set_LR(LOW, HIGH);
    set_RR(HIGH, HIGH);

    for( int i = 0;i<=step ;i++)
    {
      

        Track_PWM(STP2, speed); 
        Track_PWM(STP3, speed); 
    
    }

}

//向右平移
void go_sideway_right(int step, int speed){
    set_LF(HIGH,LOW);
    set_RF(LOW, HIGH);
    set_LR(LOW, LOW);
    set_RR(HIGH, LOW);

    for( int i = 0;i<=step ;i++)
    {

        Track_PWM(STP2, speed); 
        Track_PWM(STP3, speed);       
    }
}
void test_all(){
  
  go_forward(1000,400);
  delay(500);
  go_backward(1000, 400);
   delay(500);
  go_sideway_right(1000,400);
    delay(500);
  go_sideway_left(1000,400);
    delay(500);
  
}
