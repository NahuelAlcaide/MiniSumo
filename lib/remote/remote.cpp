#include "config.h"

#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp>
#include "led.h"

void remoteSetup() {
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void printRemoteData(){
  IrReceiver.printIRResultShort(&Serial);
  IrReceiver.printIRSendUsage(&Serial);
  Serial.println();
}

uint16_t remoteHandler() {
  if (IrReceiver.decode()) {

    IRData remoteData = IrReceiver.decodedIRData;
    IrReceiver.resume(); 
    if(remoteData.address == REMOTE_ADDRESS) {
      ledBlinkQuick();
      printRemoteData();
      return remoteData.command;
    } else {
      printRemoteData();
      return 0;
    }
  } else {
    return 0;
  }
}


