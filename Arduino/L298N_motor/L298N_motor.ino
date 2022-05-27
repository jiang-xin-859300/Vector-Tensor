#define ENA 11 
#define IN1 13 
#define IN2 15 

void setup() {
  // put your setup code here, to run once:
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENA,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(IN1,LOW);   //电机反转
  digitalWrite(IN2,HIGH);
  analogWrite(ENA,50);
   delay(100);
    digitalWrite(IN1,LOW); //控制电机停下
    digitalWrite(IN2,LOW); 
    analogWrite(ENA,255); 
    delay(100);
  
}
