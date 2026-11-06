# WifiScope
An ESP-32-S3 based Wifi-Signal Strength Analayzer

This code is for one to assess signal strength using an esp-32's inbuilt WiFi 2.4Ghz signals.
The program shows the signal name, signal strength, signal security, and channel on which the signal runs (for now) (planning to implement live refresh option and do much work on CSS later on);
It mainly functions by reading the nearby WiFi signals actively (every time you reset your ESP-32);
The project also includes calculated interference which may be caused adjacent channels, thereby varying its formula by depending on both RSSI value and Channel overlap.

***THIS PROJECT IS STILL UNDER CONSTRUCTION***

<img width="591" height="1280" alt="WhatsApp Image 2026-11-06 at 11 56 58 PM" src="https://github.com/user-attachments/assets/51318777-27c6-4f5f-bd72-ddc96670dbff" />

This is an example of how the web-page looks.


***To set it up :***

*You will need to install the PlatformIO extension which helps directly code to your micro-controllers from VS-Code;
Download the code, initialize VS-code with PlatformIO and run the code;
You shall be seeing your esp-32's local website address;
Connect to your esp-32's locally held WiFi named WiFiScope and open the website address.

_**Hardware Required : An esp-32 which can minimum read WiFi 2.4Ghz signal**_;

To refer more about setting up PlatformIO visit "https://docs.platformio.org/en/latest/core/quickstart.html"


Planning on introduce the following : Live rescaning
                                      Mathematical interpretation of the data by using graphs
                                      Complete CSS makeover (I did nothing for now cuz i wanted to prioritize the code first😭😭)
                                      
