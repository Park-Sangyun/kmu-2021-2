#define PIN7 7

void setup() {
  pinMode(PIN7, OUTPUT);
  digitalWrite(PIN7, LOW);
  delay(1000);
}

void loop(){
  int i = 0;
  while(i < 5){
    digitalWrite(PIN7, HIGH);
    delay(100);

    digitalWrite(PIN7, LOW);
    delay(100);

    i ++;
  }

  digitalWrite(PIN7, HIGH);

  while(1) {}
  
}
