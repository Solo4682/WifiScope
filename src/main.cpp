#include <Arduino.h>
#include <Wifi.h>
#include <WebServer.h>
#include <LittleFS.h>

WebServer server(80);
int networks;
int congestion[14] = {0};
int bestChannel;
int bestCongestionCandidate;
float relativeCongestion;


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

void handleRoot()
{
  File file = LittleFS.open("/index.html", "r");
  if(!file)
  {
    server.send(500, "text/plain", "Failed to load index.html");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}
void handleNotFound()
{
  server.send(404,"text/html", "<h1> 404 - Paga Not Found </h1>" "<p> The requested page does not exist. </p>" "<a href = '/'> Rethurn to WifiScope </a>"); 
}
void handleCSS ()
{
  File file = LittleFS.open("/style.css", "r");

  if(!file)
  {
    server.send(500, "text/plain", "Failed to load style.css");
    return;
  }

  server.streamFile(file, "text/css");
  file.close();
}
void handleJS()
{
  File file = LittleFS.open("/script.js", "r");

  if(!file)
  {
    server.send(500, "text/plain", "Failed to load script.js");
    return;
  }

  server.streamFile(file, "application/javascript");
  file.close();
}
void handleData () 
{
  char json[200];
  snprintf(json, sizeof(json), "{\"networks\":%d,\"bestChannel\":%d,\"congestionScore\":%d,\"relativeCongestion\":%.2f}", networks, bestChannel, bestCongestionCandidate, relativeCongestion);

  server.send(200, "application/json", json);
}
void setup() 
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("SETUP STARTED");
  if(!LittleFS.begin(true))
  {
    Serial.println("LittleFS failed to start!!");
  }
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("WiFiScope");
  delay(1000);

  Serial.println("====== WifiScope ======");
  Serial.println("Scanning to find available networks...");
  
  networks = WiFi.scanNetworks();
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
  bestChannel = candidateChannels[0];
  bestCongestionCandidate = congestion[bestChannel - 1];
  int maxCongestion = congestion[0];

  for(int i=1;i<3;i++)
  {
    int currentChannel = candidateChannels[i];
    int currentCongestion = congestion[currentChannel-1];

    if(currentCongestion < bestCongestionCandidate)
    {
      bestCongestionCandidate = currentCongestion;
      bestChannel = currentChannel;
    }
  }

  for(int i=0;i<14;i++)
  {
    if(congestion[i] > maxCongestion)
    {
      maxCongestion = congestion[i];
    }
  }

  if (maxCongestion > 0)
  {
    relativeCongestion = ((float)bestCongestionCandidate / maxCongestion)*100.0;
  }
  else 
  {
    relativeCongestion = 0.0;
  }

  Serial.println("\n===== Channel Analysis =====");
  Serial.printf("Recommended Channel is : %d\n", bestChannel);
  Serial.printf("Congestion Score is : %d\n", bestCongestionCandidate);
  Serial.printf(" Relative Congestion is : %.2f%%\n", relativeCongestion);

  server.on("/", handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/script.js", handleJS);
  server.on("/api/data", handleData);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("\n====== WiFiScope Server ======");
  Serial.println(" Status : 🟢 Running ");
  Serial.println("WiFi Network : WiFiScope");
  Serial.print("IP Address : ");
  Serial.println(WiFi.softAPIP());
}


void loop () 
{
  server.handleClient();
}