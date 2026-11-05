#include <Arduino.h>
#include <Wifi.h>

const char *signalStrength(int rssi)
{
  if(rssi >= -50)
  {
    return "Excellent";
  }
  else if(rssi >= -60)
  {
    return "Good";
  }
  else if (rssi >= -70)
  {
    return "Fair";
  }
    else if (rssi >= -80)
  {
    return "Weak";
  }
  else
  {
    return "Very weak";
  }
}

int rssiWeight(int rssi)
{
  if(rssi >=-50)
    return 5;
  else if (rssi >= -60)
    return 4;
  else if (rssi >=-70)
    return 3;
  else if (rssi >= -80)
    return 2;
  else
    return 1;

}

int channelWeight(int distance)
{
  if(distance ==0)
    return 4;
  else if (distance ==1)
    return 3;
  else if (distance == 2)
    return 2;
  else if (distance == 3)
    return 1;
  else
    return 0;    
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
  int congestion[14] = {0};
  int channelCount[14] = {0};
  int candidateChannels[3] = {1,6,11};

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

  for(int i=0;i<14;i++)
  {
    for(int j=0;j<networks;j++)
    {
      int distance = abs((i+1) - WiFi.channel(j));

      int signalWeight = rssiWeight(WiFi.RSSI(j));
      int overlapWeight = channelWeight(distance);

      congestion[i] += signalWeight*overlapWeight;

      
    }
  }

  Serial.println("\n===== Channel Congestion =====");

  for(int i=0;i<14;i++)
  {
    Serial.printf("Channel %d : Congestion Rate = %d\n", i+1, congestion[i]);
  }

  int bestChannel = candidateChannels[0];
  int lowestCongestion = congestion[bestChannel-1];

  for(int i=1;i<3;i++)
  {
    int currentChannel = candidateChannels[i];
    int currentCongestion = congestion[currentChannel-1];

    if(currentCongestion < lowestCongestion)
    {
      lowestCongestion = currentCongestion;
      bestChannel = currentChannel;
    }
  }
  Serial.println("\n===== Channel Recomendation =====");
  Serial.printf("Recommended Channel is : %d\n", bestChannel);
  Serial.printf("Congestion Score is : %d\n", lowestCongestion);
}





void loop () 
{

}