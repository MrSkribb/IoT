#include <ESP8266WiFi.h>
const char* ssid     = "SSID";      // SSID of local network
const char* password = "password";   // Password of network

const int pir = 16; // Pin of PIR
const int buzzer = 15; // Pin of Buzzer

WiFiClient client;

String result;
String motion ;

void connectwifi() { // Function that handles connection to WiFi
  
   Serial.print("Connecting to WiFi");
   Serial.println("...");
   WiFi.begin(ssid, password);
   int retries = 0;
   
while ((WiFi.status() != WL_CONNECTED) && (retries < 30)) { // Tries to connect 30 times before closing
   retries++;
   delay(500);  
}

if (retries > 30) {
    Serial.println(F("WiFi connection FAILED"));
}

if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("WiFi connected!"));
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}    
}


void setup() 
{
  pinMode(pir, INPUT); 
  pinMode(buzzer, OUTPUT);
  Serial.begin(115200);
  connectwifi();
  delay(3000);
}

void ifttt() // Handles the API functionality through IFTTT
{
  const char host[ ]        = "maker.ifttt.com";          // maker channel of IFTTT
  const char trigger[ ]     = "Motion_Detected";                   // Trigger event name
  const char APIKey[ ]      = "apikey";      // IFTTT API Key
  Serial.print("Connect to: ");
  Serial.println(host);
  // WiFiClient to make HTTP connection
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
    }

// Build URL
  String url = String("/trigger/") + trigger + String("/with/key/") + APIKey;
  Serial.print("Requesting URL: ");
  Serial.println(url);

// Send request to IFTTT
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n"); // Sends GET request to IFTTT
   delay(20);

// Read all the lines of the reply from server and print them to Serial
  Serial.println("Respond:");
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    }
  Serial.println();
  Serial.println("Connection closed... ");
  Serial.println(); 
  Serial.println();
  client.stop();  
  delay (20000); // Stops email spam
 }


void sound() { // Function for buzzer sound

  tone(buzzer, 500, 25000); // Will play a 25 second beep
  delay(500);
  
}


void loop() 
{
 if(digitalRead(pir)== HIGH)  
  {

   sound(); // Plays sound function
   motion = digitalRead(pir); //PIR sensor pin change this 
   Serial.println("Alarm detected!"); //Send things to serial
   Serial.print("Motion: ");
   Serial.println(motion);
   ifttt(); // Calls IFTTT function
   
  } 
  else 
  {
    //Serial.println("no trigger"); //for debugging
  }
}


  
