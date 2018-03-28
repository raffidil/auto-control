void setupHandler() {
  relayNode1.setProperty("power").send(pumpRelayState ? "OFF" : "ON");
  relayNode2.setProperty("power").send(motorRelayState ? "OFF" : "ON");
  relayNode3.setProperty("power").send(speedRelayState ? "SLOW" : "FAST");
}

void onHomieEvent(const HomieEvent& event) {
  switch(event.type) {
    case HomieEventType::CONFIGURATION_MODE:{
      lastLedColorSave(&lastLedColor,0,0,255);
      ledColor(lastLedColor.r,lastLedColor.g,lastLedColor.b);
      break;
    }

    case HomieEventType::NORMAL_MODE:{
      lastLedColorSave(&lastLedColor,255,255,255);
      ledColor(lastLedColor.r,lastLedColor.g,lastLedColor.b);
      break;
    }

    case HomieEventType::ABOUT_TO_RESET:
      break;

    case HomieEventType::WIFI_CONNECTED:{
      lastLedColorSave(&lastLedColor,0,255,0);
      ledColor(lastLedColor.r,lastLedColor.g,lastLedColor.b);
      // You can use event.ip, event.gateway, event.mask
      break;
    }

    case HomieEventType::WIFI_DISCONNECTED:{
      lastLedColorSave(&lastLedColor,255,0,0);
      ledColor(lastLedColor.r,lastLedColor.g,lastLedColor.b);
      // You can use event.wifiReason
      break;
    }

    case HomieEventType::MQTT_READY:{
      lastLedColorSave(&lastLedColor,0,0,0);
      ledColor(lastLedColor.r,lastLedColor.g,lastLedColor.b);
      break;
    }

    case HomieEventType::MQTT_DISCONNECTED:{
      lastLedColorSave(&lastLedColor,255,0,255);
      ledColor(lastLedColor.r,lastLedColor.g,lastLedColor.b);
      // You can use event.mqttReason
      break;
    }
  }
}

bool homieSetup(){
  Homie_setFirmware("relay", "1.0.0"); // The underscore is not a typo! See Magic bytes
  Homie.setSetupFunction(setupHandler);

  relayNode1.advertise("power").settable(powerOnHandlerPump);
  relayNode2.advertise("power").settable(powerOnHandlerMotor);
  relayNode3.advertise("power").settable(powerOnHandlerSpeed);
  Homie.disableResetTrigger();
  Homie.onEvent(onHomieEvent); // before Homie.setup()
  Homie.setup();
  return true;
}

void homieLoop(){
  Homie.loop();
  //Homie.reset();
}
