/*
 * This program is to demonstrate the motors working over 
 * WiFi, please configure the ssid and password to the network name
 * and password of your local network respectively
 * 
 * Created by: Team Deliveroid
 * 03/22/2022
*/
#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "";      //  your network SSID (name)
char pass[] = "";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

int motorPin = 6; //motorA+
int motorPin2 = 7; //motorA-
int motorPin3 = 4; //motorB+
int motorPin4 = 5; //motorB-
int motorPin5 = 2; //pwm INA
int motorPin6 = 3; //pwm INB


void setup() {
  Serial.begin(115200);      // initialize serial communication
  pinMode(6, OUTPUT);      // set the LED pin mode
  pinMode(motorPin, OUTPUT); //motorA+
  pinMode(motorPin2, OUTPUT); //motorA-
  pinMode(motorPin3, OUTPUT); //motorB+
  pinMode(motorPin4, OUTPUT); //motorB-
  pinMode(motorPin5, OUTPUT); //pwm INA
  pinMode(motorPin6, OUTPUT); //pwm INB 
   
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);       // don't continue
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}

void loop(){  
  WiFiClient client = server.available();   // listen for incoming clients

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

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> turn the LED on pin 6 on<br>");
            client.print("Click <a href=\"/L\">here</a> turn the LED on pin 6 off<br>");
            client.print("Click <a href=\"/W\">here</a> move FORWARD 8,0 on<br>");
            client.print("Click <a href=\"/S\">here</a> STOP off<br>");
            client.print("Click <a href=\"/X\">here</a> move REVERSE 7,1 on<br>");
            client.print("Click <a href=\"/A\">here</a> move LEFT 7,0 on<br>");
            client.print("Click <a href=\"/D\">here</a> move RIGHT 8,1 on<br>");
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

        // Check to see if the client request was "GET /H" or "GET /L" or "GET /W" or "GET /S" or "GET /X" or "GET /A" or "GET /D":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(6, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(6, LOW);                // GET /O turns the LED off
        }
        if (currentLine.endsWith("GET /W")) {
          digitalWrite(6, HIGH);               // GET /W moves FORWARD
          // digitalWrite(7, LOW);
          digitalWrite(4, HIGH);               
          // digitalWrite(5, LOW);
          digitalWrite(2, HIGH);               
          digitalWrite(3, HIGH);
        }
        if (currentLine.endsWith("GET /S")) {
          digitalWrite(2, LOW);                // GET /S STOP
          digitalWrite(3, LOW);
          digitalWrite(8, LOW);               
          digitalWrite(7, LOW);
          digitalWrite(0, LOW);               
          digitalWrite(1, LOW);                         
        }
        if (currentLine.endsWith("GET /X")) {
          digitalWrite(8, LOW);               // GET /X moves REVERSE
          digitalWrite(7, HIGH);               
          digitalWrite(0, LOW);          
          digitalWrite(1, HIGH);
          digitalWrite(2, HIGH);               
          digitalWrite(3, HIGH);                        
        }
        if (currentLine.endsWith("GET /A")) {
          digitalWrite(8, LOW);               // GET /A moves LEFT
          digitalWrite(7, HIGH);               
          digitalWrite(0, HIGH);          
          digitalWrite(1, LOW);
          digitalWrite(2, HIGH);               
          digitalWrite(3, HIGH);          
        }
         if (currentLine.endsWith("GET /D")) {
          digitalWrite(8, HIGH);               // GET /D moves RIGHT
          digitalWrite(7, LOW);               
          digitalWrite(0, LOW);          
          digitalWrite(1, HIGH);
          digitalWrite(2, HIGH);               
          digitalWrite(3, HIGH);            
        }                                       
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
