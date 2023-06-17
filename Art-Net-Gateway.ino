/*
    Transmit all received Art-Net messages out of the serial port
*/

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ArtnetnodeWifi.h>
#include <espDMX.h>

#define VERSION     "1.0.1"
#define ARTNET_NODE "Art-Net Gateway"
#define WM_AP_NAME  "Art-Net Gateway Setup"
#define AP_NAME     "Art-Net Gateway"

WiFiUDP         UdpSend;
ArtnetnodeWifi  artnetnode;

bool shouldSaveConfig = false;

void saveConfigCallback ()
{
  shouldSaveConfig = true;
}

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  // Forward every Art-Net frame to DMX out on UART1
  dmxB.setChans(data, length);
}

void setup()
{
  WiFiManager wm;
  uint16_t    u;
  char        universe[33];

  Serial.begin(115200);
  Serial.println("");
  Serial.print("Art-Net Gateway ");
  Serial.print(VERSION);
  Serial.println(" - https://github.com/alf45tar/Art-Net-Gateway");

  // Read default universe from EEPROM
  EEPROM.begin(512);
  EEPROM.get(0, u);
  u = constrain(u, 0, 32767);
  itoa(u, universe, 10);

  Serial.print("Get EEPROM Art-Net Universe: ");
  Serial.println(universe);

  // Connect to last AP within 15 seconds or run WiFiManager portal
  // If no user connected to portal within 60 seconds switch to AP mode
  WiFiManagerParameter universe_par("universe", "Art-Net Universe [0-32767]", universe, 5);
  wm.addParameter(&universe_par);
  wm.setSaveConfigCallback(saveConfigCallback);
  wm.setConnectTimeout(15);
  wm.setConfigPortalTimeout(60);

  if (!wm.autoConnect(WM_AP_NAME)) {
    // WiFiManager portal timeout
    Serial.print("StartAP with SSID:  ");
    Serial.print(AP_NAME);
    Serial.print("... ");
    Serial.println(WiFi.softAP(AP_NAME) ? "Ready" : "Failed!");
    Serial.print("AP IP address:  ");
    Serial.println(WiFi.softAPIP());
  }

  // If universe changed save to EEPROM
  u = atoi(universe_par.getValue());
  u = constrain(u, 0, 32767);
  if (shouldSaveConfig) {
    EEPROM.put(0, u);
    EEPROM.commit();
    Serial.print("Save EEPROM Art-Net Universe: ");
    Serial.println(universe_par.getValue());
  }
  EEPROM.end();

  // Init Art-Net universe
  artnetnode.setName(ARTNET_NODE);
  artnetnode.setUniverse(u);
  artnetnode.setStartingUniverse(u);
  artnetnode.setNumPorts(1);
  artnetnode.enableDMXOutput(0);
  artnetnode.begin();

  // Init DMX on UART1
  dmxB.begin();
  dmxB.clearChans();

  // Callback for each packet received
  artnetnode.setArtDmxCallback(onDmxFrame);
}

void loop()
{
  // Run the Art-Net node
  artnetnode.read();
}
