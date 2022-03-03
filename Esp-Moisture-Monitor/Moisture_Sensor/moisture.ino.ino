#include <ESP8266WiFi.h> // Library needed 

const char* ssid     = "ssid";      // SSID of local network
const char* password = "password";   // Password of network

// Declare variables
double analogValue = 0.0;
double analogVolts = 0.0;
unsigned long timeHolder = 0;

WiFiClient client; // Starts the wifi client

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
  
void setup() {

  Serial.begin(115200);
  delay(10);
  connectwifi(); 
  delay(1000); 
}

void ifttt() // Handles the API functionality through IFTTT
{
  const char host[ ]        = "maker.ifttt.com";          // maker channel of IFTTT
  const char trigger[ ]     = "Water_Needed";                   // Trigger event name
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
  delay (10000); // Stops email spam 
 }

void loop() {
  
  analogValue = analogRead(A0); // read the analog signal
  
  // Converts the signal into a more readable value
  analogVolts = (analogValue * 3.08) / 1024;
  int chartValue = (analogValue * 100) / 400;
  chartValue = 100 - chartValue;
  
  Serial.print(chartValue);
  delay(60000); // Checks moisture levels consistently | Once a minute 

  if (chartValue >-15) { // If soil is dry enough send a web request via ifttt api

    Serial.print("Plants need water! "); 
    ifttt(); 

    
  }
  
  
}
