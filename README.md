# Art-Net-Gateway

Rock-solid ESP8266 based WiFi ArtNet to DMX.

# Why another one?

There are a lot of similar projects out there but no one I tried is stable. Art-Net Gateway never reboot after days of work.

# Supported board

I tested it only with ESP01S.

The ESP8266 has two hardware UARTS (Serial ports):
 - UART0 on pins GPIO1 (TX0) and GPIO3 (RX0)
 - UART1 on pins GPIO2 (TX1) and GPIO8 (RX1), however, GPIO8 is used to connect the flash chip. This means that UART1 can only transmit data.

DMX OUT interface (RS-485) is connected to UART1 (GPI02) using a [SN75176](https://www.ti.com/lit/ds/symlink/sn75176a.pdf) as transceiver.


UART0 (Serial) is used to upload the firmware and for error log.

# Schematic



```
ESP8266            RS-485                DMX OUT
-------            ------                -------

                     +5V
                      |
        +-------------+-----------+               
        |                         |
        |     +---------------+   |
        |   --| R         VCC |---+
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
