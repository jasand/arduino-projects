
#include <Servo.h>

#define SERVO_PIN 11
Servo servo;
const int BUF_LEN = 32;

void setup() {
  pinMode(SERVO_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  servo.writeMicroseconds(1500);
  Serial.begin(9600);
  delay(500);
}

void loop() {
  char line[BUF_LEN];
  char c;
  int lineIndex = 0;

  while(1) {
    
    while ( Serial.available()>0 ) {
      c = Serial.read();
      if (( c == '\n') || (c == '\r') ) {
        if ( lineIndex > 0 ) {   
          line[lineIndex] = '\0';
          Serial.print("Received: ");
          Serial.println(line);
          int us = atoi(line);
          servo.writeMicroseconds(us);  // 500us - 2400us for chinese S3003 servo
        }
        lineIndex = 0;
      } else {
        line[lineIndex++] = c;
      }
    }
    
  }
}
