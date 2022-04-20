/*********************************************************
* This program is for controlling four DC motors
* Choose an option indicated in the IF statements to
* have the motors correspond with that action
* 
* Created by: Team Deliveroid
* 03/17/2022
*/

char t;
 
void setup() {
  pinMode(6, OUTPUT); // left motors forward
  pinMode(7, OUTPUT); // left motors reverse
  pinMode(4, OUTPUT); // right motors forward
  pinMode(5, OUTPUT); // right motors reverse
  pinMode(0, OUTPUT); // pwm INA
  pinMode(1, OUTPUT); // pwm INB
//  pinMode(1,OUTPUT);   //Led
  Serial.begin(9600);
 
}
 
void loop() {
if(Serial.available()){
  t = Serial.read();
  Serial.println(t);
}
 
if (t == 'F') { // move forward
          digitalWrite(6, HIGH);
          digitalWrite(7, LOW);
          digitalWrite(4, HIGH);
          digitalWrite(5, LOW);
          digitalWrite(0, HIGH);
          digitalWrite(1, HIGH);
          delay(300);
        }
        if (t == 'S') { // move reverse
          digitalWrite(6, LOW);
          digitalWrite(7, HIGH);
          digitalWrite(4, LOW);
          digitalWrite(5, HIGH);
          digitalWrite(0, HIGH);
          digitalWrite(1, HIGH);
          delay(300);
        }
        if (t == 'L') { // turn right
          digitalWrite(6, LOW);
          digitalWrite(7, LOW);
          digitalWrite(4, HIGH);
          digitalWrite(5, LOW);
          digitalWrite(0, HIGH);
          digitalWrite(1, HIGH);
          delay(300);
        }
        if (t == 'R') { // turn left
          digitalWrite(6, HIGH);
          digitalWrite(7, LOW);
          digitalWrite(4, LOW);
          digitalWrite(5, LOW);
          digitalWrite(0, HIGH);
          digitalWrite(1, HIGH);
          delay(300);
        }
        if (t == 'B') { // stop
          digitalWrite(6, LOW);
          digitalWrite(7, LOW);
          digitalWrite(4, LOW);
          digitalWrite(5, LOW);
          digitalWrite(0, LOW);
          digitalWrite(1, LOW);
          delay(30);
        }
        else if(t == 'W'){    //turn led on or off)
          digitalWrite(1,HIGH);
        }
        else if(t == 'w'){
          digitalWrite(1,LOW);
        }
}
