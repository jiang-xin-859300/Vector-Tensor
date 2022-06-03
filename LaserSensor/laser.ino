

void compare(int L, int R){
  if(L < R){
    Serial.println(1);
  }else if(L >= R){
    Serial.println(0);
  }
}

int LaserRead(int serial){
  char val;
  char a[10];
  int distance;
  int flag = 0;
  
  while (flag == 0){
    val = SerialRead(serial);
    Serial.print("ll\n");
    if (val >= '0' && val <='9'){  // read the value continuously
      a[0] = val;
      for (int i=1; i<9; i++){
        val = SerialRead(serial);
        if (val >= '0' && val <='9'){  // judge val is noise or not
          a[i] = val;
        }
        else{
          a[i] = '\0';
          if (a[0] >= '0' && a[0] <= '9' && a[1] == '\0') break;  // skip the '0-9' of "state 0-9"
          distance = atoi(a);
          direct = dir(serial);
          Serial.print(direct);
          Serial.print(":");
          Serial.print(distance);
          Serial.println("mm");
          flag = 1;
          break;
        }
      }
    }    
  }
  if(flag == 1)
    return distance;
}

char SerialRead(int serial){
  char val;
  if(serial == 3)
    val = Serial3.Read();
  else if(serial == 1)
    val = Serial1.Read();
  return val;
}

String dir(int serial){
  char direct[5];
  if(serial == 3)
    direct[5] = "Left";
  else if(serial == 1)
    direct[5] = "Right";
  return direct;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial3.begin(115200);
  Serial1.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int distance_L = LaserRead(3);
  int distance_R = LaserRead(1);
  compare(distance_L, distance_R);
  delay(1);
}
