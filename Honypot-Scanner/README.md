# Honeypot Scanner

An IoT networking scanning honeypot using the ESP8266 NodeMCU

This project was created for the purpose of scanning a local network space and notifying the operator if a new unrecognised device is found. A MAC address whitelist is used in the code for the device to refer to already authorised devices. The emails are handled using a canary token. The device also acts as a honey pot that can emulate whatever device you choose using the mac address. It will also setup an FTP server, if this is accessed, it will also trigger an email event using canary tokens. Utilises the ESP8266 Wifi library and ESP Canary

![image](https://github.com/MrSkribb/IoT/assets/44088296/c73a1506-6c4a-4542-a5fa-3682a62a4184)
![image](https://github.com/MrSkribb/IoT/assets/44088296/75c0c846-3972-4891-b2bf-1b8424359953)

