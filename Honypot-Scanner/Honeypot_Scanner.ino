#include <ESP8266WiFi.h>
#ifdef ESP8266
#elif defined ESP32
#include <WiFi.h>
#include <esp_wifi.h>
#endif
#include <ESPCanary.h>

// Replace with your Wi-Fi credentials
const char* ssid = "myssid";
const char* password = "mypassword";
uint8_t newMACAddress[] = {0x00, 0x11, 0x32, 0x07, 0x0D, 0x66};

// Replace with your Gmail and App Password
const char* smtpServer = "smtp.gmail.com"; 
const int smtpPort = 465;  // Use 465 for SSL
const char* emailFrom = "youremail@example.com";
const char* emailPassword = "yourpassword"; 
const char* emailTo = "recipient@example.com";

String canary = ""; // canary token for ftp server
const char* canaryTokenURL = ""; // canary token for mac addresses

String ftp_user = "admin";    //if you replace this with "%" it will accept ANY username
String ftp_pass = "%"; //if you replace this with "%" it will accept ANY password
bool append_ip = false;      
String append_char = "?";     
FtpServer ftpSrv; 

// Array to store the MAC addresses of authorized devices, replace with relevant mac addresses
const char* authorizedDevices[] = {
  "00:00:00:00:00:00",  // mac 1
  "00:00:00:00:00:00",  // mac 2
  "00:00:00:00:00:00",  // mac 3
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
}
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

  // FTP Setup
  ftpSrv.begin(ftp_user, ftp_pass, canary, append_ip, append_char);    //username, password for ftp.  set ports in ESPCanary.h  (default 21, 50009 for PASV)
}

void loop() {
	
	// Get the list of connected devices
	int numDevices;
	WiFi.macAddress(numDevices);
	ftpSrv.handleFTP();        // serves ftp
	
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
			delay(60000);  // Wait for a minute before scanning again
			endCanaryTokenNotification();
		}
	}
}
void sendCanaryTokenNotification() {
  Serial.println("New unauthorized device detected. Sending Canary Token notification...");

  HTTPClient http;
  http.begin(canaryTokenURL);

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
  } else {
    Serial.println("HTTP Request failed");
  }

  http.end();
}
