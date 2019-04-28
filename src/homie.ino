void setupHandler() {
  relayNode1.setProperty("power").send(relayState1 ? "OFF" : "ON");
  relayNode2.setProperty("power").send(relayState2 ? "OFF" : "ON");
  relayNode3.setProperty("power").send(relayState3 ? "OFF" : "ON");
  relayNode4.setProperty("power").send(relayState4 ? "OFF" : "ON");
  relayNode5.setProperty("power").send(relayState5 ? "OFF" : "ON");


}
bool wifi_connected = false;
bool mqtt_connected = false;

void onHomieEvent(const HomieEvent &event) {
  switch (event.type) {
  case HomieEventType::CONFIGURATION_MODE:
    ledColor(0, 0, 255);
    break;
  case HomieEventType::NORMAL_MODE: {
    ledColor(255, 255, 255);
    break;
  }

  case HomieEventType::ABOUT_TO_RESET:
    // Do whatever you want when the device is about to reset
    break;
  case HomieEventType::WIFI_CONNECTED: {
    wifi_connected = true;
    if (!mqtt_connected){
      ledColor(0, 255, 0);
    }else{
    ledColor(0, 0, 0);
    }

    // You can use event.ip, event.gateway, event.mask
    break;
  }

  case HomieEventType::WIFI_DISCONNECTED:
    ledColor(255, 0, 0);
    wifi_connected=false;
    // You can use event.wifiReason
    break;
  case HomieEventType::MQTT_READY: {
    mqtt_connected = true;
    ledColor(0, 0, 0);
    break;
  }

  case HomieEventType::MQTT_DISCONNECTED: {
    ledColor(255, 0, 255);
    mqtt_connected=false;
    // Do whatever you want when MQTT is disconnected in normal mode

    // You can use event.mqttReason
    break;
  }
  }
}

bool homieSetup() {
  Homie_setFirmware("relay",
                    "1.0.0"); // The underscore is not a typo! See Magic bytes
  Homie.setSetupFunction(setupHandler);

  relayNode1.advertise("power").settable(powerOnHandler1);
  relayNode2.advertise("power").settable(powerOnHandler2);
  relayNode3.advertise("power").settable(powerOnHandler3);
  relayNode4.advertise("power").settable(powerOnHandler4);
  relayNode5.advertise("power").settable(powerOnHandler5);


  Homie.disableResetTrigger();
  Homie.onEvent(onHomieEvent); // before Homie.setup()
  Homie.setup();
  return true;
}

void homieLoop() {
  Homie.loop();
  // Homie.reset();
}
