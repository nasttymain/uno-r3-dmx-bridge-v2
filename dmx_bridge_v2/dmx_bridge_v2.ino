// () 2024 /\/asTTY
// [OK] Arduino Uno R3 @16MHz
// Arduino Should be run on 5V supply

// DMX Shield Settings
// Only 127 channels avail
// https://ssci.to/4274
// https://sourceforge.net/projects/dmxlibraryforar/
#include <Conceptinetics.h>
#define DMX_MASTER_CHANNELS   128
#define RXEN_PIN                2
DMX_Master dmx_master ( DMX_MASTER_CHANNELS, RXEN_PIN );

// UART Settings
// settings: 38400bps、8N1
// https://github.com/arduino/ArduinoCore-avr/blob/master/libraries/SoftwareSerial/
#include <SoftwareSerial.h>
#define LED_RECEIVE_PIN 13
#define TX_PIN 10
#define RX_PIN 11
#define topc softuart
SoftwareSerial softuart(RX_PIN, TX_PIN);

unsigned char pqueue = 0;
unsigned char ch = 0;

void setup() {

  pinMode(LED_RECEIVE_PIN,OUTPUT);
  
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);

  digitalWrite(LED_RECEIVE_PIN, HIGH);
  delay(1000);
  
  dmx_master.enable();

  topc.begin(38400);
  topc.listen();

  digitalWrite(LED_RECEIVE_PIN, LOW);

}

void loop() {

  while(1){
    if(topc.available()){
      pqueue = topc.read();
      if(pqueue & 0b10000000 == 0b00000000){
        // channel
        if(pqueue == 127){
          // align signal
          dmx_master.setChannelRange(0, DMX_MASTER_CHANNELS, 0)
        }else{
          ch = pqueue;
        }
      }else{
        // color
        unsigned char col = (pqueue & 0b01111111) * 2;
        dmx_master.setChannelValue (ch, col);
      }
    }
  }

}
