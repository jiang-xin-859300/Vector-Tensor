int cycle = 0;  // record the reading cycle

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial3.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  char val;
  char a [10];
  val = Serial3.read();
  if (val >= '0' && val <='9'){  // read the value continuously
    char a[10];
    a[0] = val;
    for (int i=1; i<9; i++){
      val = Serial3.read();
      if (val >= '0' && val <='9'){  // judge val is noise or not
        a[i] = val;
      }
      else{
        a[i] = '\0';
        if (a[0] == '0' && a[1] == '\0') continue;  // skip the '0' of "state 0"
        cycle ++;
        Serial.print("cycle");
        Serial.print(cycle);
        Serial.print(" ");
        Serial.print(a);
        Serial.println("mm");
        break;
      }
    }
  }
  delay(1);
}
