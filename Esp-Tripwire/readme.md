# Esp-Tripwire

An IoT motion detection system powered by a passive infared sensor (PIR) and an ESP8266 NodeMCU.

This project was created for the purpose of movement monitoring which is powered by IoT capable hardware. The NodeMCU used provides a functionality that alerts the user via email whenever the PIR detects motion, as well as controlling a small buzzer to send out a 25 second 500hz tone. This is done by using webhooks through the IFTTT API. Despite being simple, this project consisted of numerous hardware challenges involving form factor and the source of power. A custom enclosure was made for the components which was done by cutting specific holes for the hardware to meet its needs. Multiple units of this could be used in conjunction to form a small network of IoT PIR sensors. 

Credit to MKme https://github.com/MKme for providing insight on using the IFTTT API.
