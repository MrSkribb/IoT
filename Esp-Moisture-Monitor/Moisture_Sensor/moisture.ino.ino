#include <ESP8266WiFi.h>

const char* ssid     = "";      // SSID of local network
const char* password = "";   // Password of network

double analogValue = 0.0;
double analogVolts = 0.0;
unsigned long timeHolder = 0;

WiFiClient client; 

int greenLED = 13; //D7
int blueLED = 12; // D6

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
  // put your setup code here, to run once:

  Serial.begin(115200);
  delay(10);
  connectwifi(); 
  delay(1000);
   
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  digitalWrite(blueLED, LOW);
  digitalWrite(greenLED, LOW);
  
  delay(1000);
}

void ifttt() // Handles the API functionality through IFTTT
{
  const char host[ ]        = "maker.ifttt.com";          // maker channel of IFTTT
  const char trigger[ ]     = "Water_Needed";                   // Trigger event name
  const char APIKey[ ]      = "";      // IFTTT API Key
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

void loop() {
  
  analogValue = analogRead(A0); // read the analog signal
  
  analogVolts = (analogValue * 3.08) / 1024;
  int chartValue = (analogValue * 100) / 400;
  chartValue = 100 - chartValue;
  
  Serial.print(chartValue);
  delay(10000); // Checks moisture levels consistently

  if (chartValue >-15) { // If moisture is too low

    Serial.println();
    Serial.print("Plants need water! ");
    digitalWrite(greenLED, LOW);
    digitalWrite(blueLED, HIGH);
    ifttt(); // Sends email    
  }
  else { // If moisture is all good 
    Serial.println();
    Serial.print("Water is not needed. ");
    digitalWrite(blueLED, LOW);
    digitalWrite(greenLED, HIGH);
    
  }
  
  
  
}
