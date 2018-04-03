
bool powerOnHandler1(const HomieRange &range, const String &value) {

  if (value != "ON" && value != "OFF")
    return false;

  if (value == "ON") {
    relayState1 = LOW;
  } else {
    relayState1 = HIGH;
  }

  digitalWrite(PIN_RELAY1, relayState1);
  relayNode1.setProperty("power").send(value);
  Homie.getLogger() << "Power1 is " << value << endl;

  return true;
}

bool powerOnHandler2(const HomieRange &range, const String &value) {

  if (value != "ON" && value != "OFF")
    return false;

  if (value == "ON") {
    relayState2 = LOW;
  } else {
    relayState2 = HIGH;
  }

  digitalWrite(PIN_RELAY2, relayState2);
  relayNode2.setProperty("power").send(value);
  Homie.getLogger() << "Power2 is " << value << endl;

  return true;
}

bool powerOnHandler3(const HomieRange &range, const String &value) {

  if (value != "ON" && value != "OFF")
    return false;

  if (value == "ON") {
    relayState3 = LOW;
  } else {
    relayState3 = HIGH;
  }

  digitalWrite(PIN_RELAY3, relayState3);
  relayNode3.setProperty("power").send(value);
  Homie.getLogger() << "Power3 is " << value << endl;

  return true;
}
