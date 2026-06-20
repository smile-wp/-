//盒子开关
#include <Servo.h>
Servo myServo;

const int TrigPin = 2;
const int EchoPin = 3;
float cm;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(9);
  Serial.begin(9600);
  pinMode(TrigPin,OUTPUT);
  pinMode(EchoPin,INPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(6,LOW);
  digitalWrite(TrigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin,LOW);


  cm = pulseIn(EchoPin,HIGH);
  
   Serial.print("cm=");
    Serial.print(cm);
    if(cm<=20 && cm>=0){
      openDoor();
    }
  Serial.print("cm");
  Serial.println();
  delay(1000);

}
void openDoor(){
   digitalWrite(5,HIGH);
  for(int i=0;i<=100;i++){
    myServo.write(i);
    delay(10);
  }
    delay(5000);
    for(int i=100;i>=0;i--){
      myServo.write(i);
      delay(5);
    }
    delay(100);
    digitalWrite(5,LOW);
}
