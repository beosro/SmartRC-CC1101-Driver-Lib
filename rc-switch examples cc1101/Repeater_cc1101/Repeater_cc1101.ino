/*
  Simple example for repeating decimal code for cc1101
  
  https://github.com/LSatan/RCSwitch-CC1101-Driver-Lib
  https://github.com/sui77/rc-switch/
  ----------------------------------------------------------
  Mod by Little Satan. Have Fun!
  ----------------------------------------------------------
*/
#include <ELECHOUSE_CC1101_RCS_DRV.h>
#include <RCSwitch.h>

int esp;   // for ESP8266 & Arduino setting. 
int pinRx; // int for Receive pin.
int pinTx; // int for Transmit pin.

RCSwitch mySwitch = RCSwitch();

int number = 15;  // set number of transmission repetitions.
int TIME = 3000; // set delay befor repeat. For direct repetition after receive set 0.

int long value;      // int to save value
int bits;           // int to save bit number
int prot;          // int to save Protocol number
int puls;         // int to save pulse length

void setup() {
  Serial.begin(9600);

#ifdef ESP8266
esp = 1; pinRx = 4; pinTx = 5;  // for esp8266! Receiver on pin 4 = D2. Transmit on pin 5 = D1.
#else
esp = 0; pinRx = 0; pinTx = 6;  // for Arduino! Receiver on interrupt 0 => that is pin #2. Transmit on pin 6.
#endif  

//CC1101 Settings:                          (Settings with "//" are optional!)
  ELECHOUSE_cc1101.setESP8266(esp);       // esp8266 & Arduino SPI pin settings. Don´t change this line!
//ELECHOUSE_cc1101.setRxBW(16);          // set Receive filter bandwidth (default = 812khz) 1 = 58khz, 2 = 67khz, 3 = 81khz, 4 = 101khz, 5 = 116khz, 6 = 135khz, 7 = 162khz, 8 = 203khz, 9 = 232khz, 10 = 270khz, 11 = 325khz, 12 = 406khz, 13 = 464khz, 14 = 541khz, 15 = 650khz, 16 = 812khz.
//ELECHOUSE_cc1101.setChsp(80);         // set Channle spacing (default = 80khz) you can set 25,50,80,100,150,200,250,300,350,405.
//ELECHOUSE_cc1101.freq2(0x10);        // set your own frequency Parameters Freq2. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.freq1(0x27);       // set your own frequency Parameters Freq1. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.freq0(0x62);      // set your own frequency Parameters Freq0. From Smart RF Studio. Must set Frequency settings F_xxx.
//ELECHOUSE_cc1101.setChannel(175); // set channel 80khz steps.0 - 255 default channel number is 175 = 433,92Mhz / 315Mhz 
  ELECHOUSE_cc1101.Init(F_433);    // Must set! set frequency - F_315, F_433, F_868, F_965 MHz. custom = F_xxx. set TxPower  PA10, PA7, PA5, PA0, PA_10, PA_15, PA_20, PA_30. example: F_433_PA10. (default = PA10)
  ELECHOUSE_cc1101.SetRx();       // set Recive on
  mySwitch.enableReceive(pinRx); // Receiver on

}
void loop() {

   if (value > 0) {

  ELECHOUSE_cc1101.SetTx();           // set Transmit on
  mySwitch.disableReceive();         // Receiver off
  mySwitch.enableTransmit(pinTx);   // Transmit on


  mySwitch.setRepeatTransmit(number); // transmission repetitions.
  mySwitch.setProtocol(prot);        // send Received Protocol
  mySwitch.setPulseLength(puls);    // send Received Delay
  mySwitch.send(value, bits);      // send Received value/bits

    Serial.print("Transmit ");
    Serial.print( value );
    Serial.print(" / ");
    Serial.print( bits );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.print( prot );
    Serial.print(" Delay: ");    
    Serial.println( puls );      


  ELECHOUSE_cc1101.SetRx();      // set Recive on
  mySwitch.disableTransmit();   // set Transmit off
  mySwitch.enableReceive(pinRx);   // Receiver on
    
   value = 0; // Reset value after transmit for receive.

   }
   
   else{
  if (mySwitch.available()){
    
    Serial.print("Received ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print("Protocol: ");
    Serial.print( mySwitch.getReceivedProtocol() );
    Serial.print(" Delay: ");    
    Serial.println( mySwitch.getReceivedDelay() );

    value =  mySwitch.getReceivedValue();        // save received Value
    bits = mySwitch.getReceivedBitlength();     // save received Bitlength
    prot = mySwitch.getReceivedProtocol();     // save received Protocol
    puls =  mySwitch.getReceivedDelay();      // save received pulse length
    
    mySwitch.resetAvailable();
    delay(TIME);   
   }
   }

}