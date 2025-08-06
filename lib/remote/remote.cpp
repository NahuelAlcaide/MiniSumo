#include "config.h"

#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE
#include <IRremote.hpp> 

void remoteSetup() {
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

uint16_t remoteTest() {
  if (IrReceiver.decode()) {

    IRData remoteData = IrReceiver.decodedIRData;
    IrReceiver.resume(); 
    if(remoteData.protocol == RC5 && remoteData.address == 0x1E) {
      IrReceiver.printIRResultShort(&Serial);
      IrReceiver.printIRSendUsage(&Serial);
      Serial.println();
      return remoteData.command;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}


