#include <SPI.h>
#include <WiFiNINA.h>

#define ss "insert ssid here"
#define pw "insert pwd here"

char ssid[] = ss;
char pwd[] = pw;
int status = WL_IDLE_STATUS;

void printMAC(byte* mac){
  for(int i = 0; i<6; i++){
  Serial.print(mac[i], HEX);
  if(i<5) Serial.print(":");
  }
}

void scanNetworks(){
  int SSIDcount = WiFi.scanNetworks();
  if(SSIDcount == -1) Serial.println("Couldn't establish wireless connection");
  else{
    for(int network = 0; network < SSIDcount; network++){
      byte mac[6];
      WiFi.BSSID(network, mac);
      Serial.print(network);
      Serial.print(") ");
      Serial.print("BSSID ");
      printMAC(mac);
      Serial.print(" | Channel ");
      Serial.println(WiFi.channel(network));
    }
  }
}

void setup() {
  // Begin serial connection and wait for it to be established
  Serial.begin(9600);
  while(!Serial);

  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  printMAC(mac);
  Serial.print('\n');

  int scans = 3;
  for(int scan = 1; scan <= scans; scan++){
    Serial.print("Scan ");
    Serial.print(scan);
    Serial.print("/");
    Serial.println(scans);
    scanNetworks();
    // Wait for 10 seconds
    if(scan!=3) delay(10000);
  }

  // Try to connect to the assigned network
  while(status != WL_CONNECTED){
    Serial.print("Connecting to network: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pwd);
    // Wait for a second to try again
    delay(1000);
  }

  Serial.println("Connected succesfully");
}

void loop() {

}