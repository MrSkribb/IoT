#include <ESP8266WiFi.h>
#include <ESPCanary.h>

// Replace with your Wi-Fi credentials
const char* ssid = "yourssid";
const char* password = "yourpassword";
uint8_t newMACAddress[] = {0x00, 0x11, 0x32, 0x07, 0x0D, 0x66}; // Choose to emulate a device, this is a synology NAS

// Replace with your Gmail and App Password
const char* smtpServer = "smtp.gmail.com";
const int smtpPort = 465;  // Use 465 for SSL

// gmail accounts
const char* emailFrom = "your@gmail.com";
const char* emailPassword = "yourpassword";  
const char* emailTo = "recipient@gmail.com";

String canary = "your canary token for ftp server";  // --> http://canarytokens.org
const char* canaryTokenURL = "your canary token for macs";
WiFiServer server(21); // Create a server on port 21 (ftp)

// Array to store the MAC addresses of authorized devices 
const char* authorizedDevices[] = {
  "55:44:33:22:11:00",  // mac 1
  "55:44:33:22:11:00",  // mac 2
  "55:44:33:22:11:00",  // mac 3 etc..
};

const int numAuthorizedDevices = sizeof(authorizedDevices) / sizeof(authorizedDevices[0]);

void setup() {
  Serial.begin(115200);
  
  #ifdef ESP8266
    wifi_set_macaddr(STATION_IF, &newMACAddress[0]);
  #elif defined ESP32
    WiFi.mode(WIFI_STA);
    esp_wifi_set_mac(ESP_IF_WIFI_STA, &newMACAddress[0]);
  #endif
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  
  server.begin(); // Starts FTP Server

  }

 void loop() {
 
  WiFiClient client = server.available();
  if (client) { // If an ftp attempt is made 
    sendCanaryTokenNotification_ftp(client);
    client.stop();
  }

  // Get the list of connected & nearby devices
  int numDevices = WiFi.scanNetworks();
  
  // Compare detected devices with authorized devices
  for (int i = 0; i < numDevices; i++) {
    String macAddress = WiFi.BSSIDstr(i);
    bool isAuthorized = false;

    for (int j = 0; j < numAuthorizedDevices; j++) {
      if (macAddress.equalsIgnoreCase(authorizedDevices[j])) {
        isAuthorized = true;
        break;
      }
    }

    if (!isAuthorized) {
      sendCanaryTokenNotification(macAddress); // Send Canary Token notification with MAC address
      delay(60000); // wait a bit to stop spam
    }
  }
}

void sendCanaryTokenNotification(String macAddress) {
  Serial.println("New unauthorized device detected. Sending Canary Token notification...");
  Serial.println(macAddress);
  String urlMac = canaryTokenURL + macAddress;

  HTTPClient http;
  http.begin(urlMac);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
  } else {
    Serial.println("HTTP Request failed");
  }

  http.end();
}

void sendCanaryTokenNotification_ftp(WiFiClient& client) {
  Serial.println("FTP connection attempt detected. Sending Canary Token notification...");
  HTTPClient http;

  // Get the local IP address of the client
  IPAddress clientIP = client.remoteIP();
  Serial.print("Client IP address: ");
  Serial.println(clientIP);

  // Append the client IP address to your Canary URL
  String canaryWithIP = canary + "?ip=" + clientIP.toString();

  // Send the Canary Token notification with the client's IP address
  http.begin(canaryWithIP);
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
  } else {
    Serial.println("HTTP Request failed");
  }
  http.end();
}
