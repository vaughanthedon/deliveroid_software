/*
 * This is the final server code of the deliveroid system
 * Please use this code when running the robot's full functions
 * Configure your network with the network with your own ssid and password
 * 
 * Created by: Team Deliveroid
 * 04/15/2022
*/ 

#include <SPI.h>
#include <WiFi101.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "secrets.h"


// Creating variables for sensors, motors, and enables
// IR Sensors
int IR1 = 8; // Right Sensor
int IR2 = 9; // Left Sensor
// Ultrasonic Sensor
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
// Motors
int mot1 = 6; // Right motors forward
int mot2 = 7; // Right motors reverse
int mot3 = 4; // Left motors forward
int mot4 = 5; // Left motors reverse
// Motors Enables
int en1 = 0;
int en2 = 1;
// Creating LCD Display
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Network Configuration
char ssid[] = SECRET_SSID;             //  your network SSID (name) between the " "
char pass[] = SECRET_PASS;      // your network password between the " "
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;      //connection status
WiFiServer server(80);            //server socket
WiFiClient client = server.available(); // client socket


void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to");
  lcd.setCursor(0,1);
  lcd.print("Deliveroid!");
  delay(2000);
  pinMode(en1, OUTPUT);
  pinMode(en2, OUTPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(mot1, OUTPUT);
  pinMode(mot2, OUTPUT);
  pinMode(mot3, OUTPUT);
  pinMode(mot4, OUTPUT);
  Serial.begin(9600);

  
  enable_WiFi();
  connect_WiFi();

  server.begin();
  printWifiStatus();
}

void loop() {
  client = server.available();

  // When client connects, display information to the LCD
  // and print the webserver on their end
  if (client) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("User");
    lcd.setCursor(0,1);
    lcd.print("Connected!");
    printWEB();
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  // print the server's IP address
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Server location: ");
  lcd.setCursor(0,1);
  lcd.print(ip);
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}

void enable_WiFi() {
  // if the board needs a firmware update 
  // otherwise Wi-Fi module will be started
  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
}

void connect_WiFi() {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    lcd.setCursor(0,0);
    lcd.print("Attempting to");
    lcd.setCursor(0,1);
    lcd.print("connect...");
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
}

void motorControl() {
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("DELIVERING...");
  lcd.setCursor(0,1);
  lcd.print("Watch your step");
  while(1) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH); // Reads the echoPin
    int distance = duration * 0.034 / 2; // Calculating the distance, Speed of sound wave
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" ");
    if (distance <= 35){
      digitalWrite(mot1, LOW);
      digitalWrite(mot2, LOW);
      digitalWrite(mot3, LOW);
      digitalWrite(mot4, LOW);
      analogWrite(en1, 0);
      analogWrite(en2, 0);
    } 
    else if(digitalRead(IR1)==LOW && digitalRead(IR2)==LOW) { //IR will not glow on black line 
      digitalWrite(mot1, LOW);
      digitalWrite(mot2, LOW);
      digitalWrite(mot3, LOW);
      digitalWrite(mot4, LOW);
      analogWrite(en1, 0);
      analogWrite(en2, 0);
      break;
    }
    else if(digitalRead(IR1)==HIGH && digitalRead(IR2)==HIGH){ //IR not on black line
      digitalWrite(mot1, HIGH);
      digitalWrite(mot2, LOW);
      digitalWrite(mot3, HIGH);
      digitalWrite(mot4, LOW);
      analogWrite(en1, 120);
      analogWrite(en2, 120);
      
    }
    else if(digitalRead(IR1)==LOW && digitalRead(IR2)==HIGH) { //Tilt robot towards right by stopping the right wheel and moving the right one
      digitalWrite(mot1, HIGH);
      digitalWrite(mot2, LOW);
      digitalWrite(mot3, LOW);
      digitalWrite(mot4, LOW);
      analogWrite(en1, 200);
      analogWrite(en2, 150);
      delay(100);
    }
    else if(digitalRead(IR1)==HIGH && digitalRead(IR2)==LOW) { //Tilt robot towards left by stopping the left wheel and moving the right one
      digitalWrite(mot1, LOW);
      digitalWrite(mot2, LOW);
      digitalWrite(mot3, HIGH);
      digitalWrite(mot4, LOW);
      analogWrite(en1, 150);
      analogWrite(en2, 200);
      delay(100);
    }
     
    else
    { // Stop both the motors
      digitalWrite(mot1, LOW);
      digitalWrite(mot2, LOW);
      digitalWrite(mot3, LOW);
      digitalWrite(mot4, LOW);
      analogWrite(en1, 0);
      analogWrite(en2, 0);
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Arrived!");
}

void comeHome() {
  while(digitalRead(IR1)==LOW && digitalRead(IR2)==LOW) {
    digitalWrite(mot1, LOW);
      digitalWrite(mot2, LOW);
      digitalWrite(mot3, HIGH);
      digitalWrite(mot4, LOW);
      analogWrite(en1, 200);
      analogWrite(en2, 150);
      delay(100);
  }
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Returning...");
  lcd.setCursor(0,1);
  lcd.print("Watch your step");
  motorControl();
}

void printWEB() {
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Awaiting");
  lcd.setCursor(2,1);
  lcd.print("Instruction");

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            //create the buttons
            client.println("<!DOCTYPE html>");
            client.println("<html>");
            client.println("<head>");
            client.println("<style>");
            client.print(".content {text-align: center; font-size:30px; border-bottom: 5px solid #333333; margin-bottom:15px;}");
            client.print(".button {font: bold 22px; font-size:18px; text-decoration:none; background-color: #EEEEEE; color: #333333; padding: 2px 6px 2px 6px; border-top: 1px solid #CCCCCC; border-right: 1px solid #333333; border-bottom: 1px solid #333333; border-left: 1px solid #CCCCCC;}");
            client.print("#link {text-align:center;}");
            client.print("</style>");
            client.println("<body>");
            client.println("<div class='content'>");
            client.print("<h1>Welcome to Deliveroid!</h1>");
            client.print("</div>");
            client.print("<div id='link'>");
            client.print("<a class='button' id='deliver' href=\"/F\">Start Delivering!</a><br>");
            client.print("<br>");
            client.print("<br>");
            client.print("<a class='button' href=\"/R\">Return Home!</a><br>");
            client.print("</div>");
            client.print(" ");
//            client.print("Click <a href=\"/S\">here</a> move reverse<br><br>");
//            client.print(" ");
//            client.print("Click <a href=\"/L\">here</a> turn right<br><br>");
//            client.print(" ");
//            client.print("Click <a href=\"/R\">here</a> turn left<br><br>");
//            client.print(" ");
            client.print("Click <a href=\"/B\">here</a> stop<br><br>");
            client.print("</body>");
            client.println();

            int randomReading = analogRead(A1);
            client.print("Random reading from analog pin: ");
            client.print(randomReading);




            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.startsWith("GET /F")) { // robot will begin to follow line
          motorControl();
          delay(300);
        }
        if (currentLine.startsWith("GET /R")) {
          comeHome();
          delay(300);
        }
        if (currentLine.startsWith("GET /B")) { // stop
          digitalWrite(mot1, LOW);
          digitalWrite(mot2, LOW);
          digitalWrite(mot3, LOW);
          digitalWrite(mot4, LOW);
          digitalWrite(en1, LOW);
          digitalWrite(en2, LOW);
          delay(30);
        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
