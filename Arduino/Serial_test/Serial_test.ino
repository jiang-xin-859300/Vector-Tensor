void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
//  String rxdata="";
//  while(Serial.available()>0)
//  {
//    char inchar = Serial.read();  
//    rxdata+=(char)inchar;          //逐个字符接收串口缓冲区内的数据
//    delay(10);                     //等待数据完全进入串口缓冲区
//  }
//  if(rxdata!="")                     //如果接收到数据，就将其打印出来
//  { 
//    Serial.print("Input: ");
//    Serial.println(rxdata);
//}
  Serial1.println("sb");
}
