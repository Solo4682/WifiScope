#include <Arduino.h>
#include <Wifi.h>

const char *signalStrength(int rssi)
{
  if(rssi >= -50)
    return "Excellent";
  else if(rssi >= -60)
    return "Good";
  else if (rssi >= -70)
    return "Fair";
    else if (rssi >= -80)
    return "Weak";
  else
    return "Very weak";
}
const char *getSecurity(int type)
{
  switch(type)
  {
    case WIFI_AUTH_OPEN:
      return "Open";
    case WIFI_AUTH_WEP:
      return "WEP";
    case WIFI_AUTH_WPA_PSK:
      return "WPA-Personal";
    case WIFI_AUTH_WPA2_PSK:
      return "WPA2-Personal";
    case WIFI_AUTH_WPA2_ENTERPRISE:
      return "WPA2-Enterprise";
    case WIFI_AUTH_WPA_WPA2_PSK:
      return "WPA/WPA2-Personal";
    case WIFI_AUTH_WPA3_PSK:
      return "WPA3-Personal";
    case WIFI_AUTH_WPA2_WPA3_PSK:
      return "WPA2/WPA3-Personal";
    default:
      return "Unknown";
  }
}

void setup() 
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  Serial.println("====== WifiScope ======");
  Serial.println("Scanning to find available networks...");
  
  int networks = WiFi.scanNetworks();
  int bestChannel = 1;
  int channelCount[14] = {0};

  for(int i=0;i<networks;i++)
  {
    channelCount[WiFi.channel(i)-1]++;
  }

  Serial.print(" Networks Found : ");
  Serial.println(networks);
  for(int i=0;i<networks;i++)
  {
    Serial.printf("%s | Signal Strength : %s | Channel : %ld | Security : %s\n",WiFi.SSID(i).c_str(), signalStrength(WiFi.RSSI(i)), WiFi.channel(i), getSecurity(WiFi.encryptionType(i)));
  }
    Serial.println("\n ======Channel Count======");

  for(int i=0;i<14;i++)
  {
    if(channelCount[i]>0)
    {
      Serial.printf("Channel %d : %d networks\n", i+1, channelCount[i]);
    } 
  }
  
  int minimumNetworks = channelCount[0];
  for(int i=0;i<networks;i++)
  {
    if(channelCount[i]<minimumNetworks)
    {
      minimumNetworks = channelCount[i];
      bestChannel = i+1;
    }
  }
}

void loop () 
{

}