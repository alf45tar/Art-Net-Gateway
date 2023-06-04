# Art-Net-Gateway

Rock-solid ESP8266 based WiFi Art-Net to DMX. 

## Why another one?

There are a lot of similar projects out there but no one I tried is stable. Art-Net Gateway never restarts even after days of uninterrupted work.

## How to connect to the WiFi

Art-Net-Gateway uses a WiFi connection manager with web captive portal. The configuration portal is of the captive variety, so on various devices it will present the configuration dialogue as soon as you connect to the created access point.

- When your ESP starts up, it sets it up in Station mode and tries to connect to a previously saved Access Point
- If this is unsuccessful (or no previous network saved or after the 15 seconds timeout) it moves the ESP into Access Point mode
- Using any WiFi enabled device with a browser (computer, phone, tablet) connect to the newly created Access Point called "Art-Net Gateway Setup"
- Because of the captive portal you will either get a 'Join to network' type of popup or get any domain you try to access redirected to the configuration portal
- Choose one of the access points scanned, enter password, change the default Art-Net Universe and click Save
- ESP will try to connect. If successful Art-Net Gateway is up and running connected to your WiFi network. If not, reconnect to AP and reconfigure.
- If no user connect to the Access Point within 60 seconds another Access Point called "Art-Net Gateway" is created without the captive portal for normal operation

## How to compile it

- Download and install [Arduino IDE 2](https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing)
- Install the [WiFiManager](https://github.com/tzapu/WiFiManager) library using the Arduino IDE Library Manager via `Sketch > Include Libray > Manage Libraries`
- Download the [ArtnetnodeWifi](https://github.com/rstephan/ArtnetnodeWifi/archive/refs/heads/master.zip) library and add to Arduino IDE using `Sketch > Include Library > Add .ZIP Library` (or unzip into `~/Documents/Arduino/libraries/ArtnetnodeWifi` folder)
- Download the [espDMX](https://github.com/alf45tar/espDMX/archive/refs/heads/master.zip) library and add to Arduino IDE using `Sketch > Include Library > Add .ZIP Library` (or unzip into `~/Documents/Arduino/libraries/espDMX` folder)
- [Dowload](https://github.com/alf45tar/Art-Net-Gateway/archive/refs/heads/main.zip) the Art-Net Gateway repository and unzip into a folder called `Art-Net-Gateway`
- Open `Art-Net-Gateway.ino`
- Compile it!

## Supported boards

I tested it only with ESP01S. Any ESP8266 board should work as well.

The ESP8266 has two hardware UARTS (Serial ports):
 - UART0 on pins GPIO1 (TX0) and GPIO3 (RX0)
 - UART1 on pins GPIO2 (TX1) and GPIO8 (RX1), however, GPIO8 is used to connect the flash chip. This means that UART1 can only transmit data.

DMX OUT interface (RS-485) is connected to UART1 (GPI02) using a [SN75176](https://www.ti.com/lit/ds/symlink/sn75176a.pdf) as transceiver.

UART0 (Serial) is used to flash the firmware and for error log.

The gateway requires 3.3V for the ESP board and 5V for the RS-485 transceiver. The power circuit depends by the application.

## Schematic

```
+------------------------------------------+
| |                                        |
| |                         RX0 +  + 3.3V  |
| +---+  |                                 |
| +---+  |   +--------+   GPIO0 +  + RST   |
| +---+  |   | ESP01S |                    |
| +---+  |   +--------+   GPIO2 +  + CH_PD |
| +---+  |                                 |
| +---+  |                  GND +  + TX0   |
| +------+                                 |
+------------------------------------------+
    
            RESET
            Button
              |
            --+-- 
RST   -----+     +----- GND


            PROGRAM
            Button
              |
            --+-- 
GPIO0 -----+     +----- GND


                     +5V
                      |
        +-------------+-----------+               
        |                         |
        |     +---------------+   |
        |   --| R         VCC |---+       DMX OUT
        |     |               |
        |   --| RE/         B |---------- Data - (XLR pin 2)
        |     |    SN75176    |
        +-----| DE          A |---------- Data + (XLR pin 3)
              |               |
GPIO2 --------| D         GND |---+------ Ground (XLR pin 1)
              +---------------+   |
                                  |
GND   ----------------------------+
```

## How to flash ESP01S with Arduino Uno

There are a lot of different way to flash the ESP01S board. My preferred mode is with an Arduino Uno as USB-to-TTL (holding RST to GND).

Remember, the ESP runs on 3.3V, while your Arduino Uno has a 5V powered TX/RX. This option worked with my setup but be advised, it might damage your devices.

ESP01S|Arduino Uno|Note
------|-----------|----
TX0|TX|
RX0|RX|
3V3|3.3V|Connect only if the ESP01S board is not powered via other source 
GND|GND|
