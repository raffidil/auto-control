bool powerOnHandlerPump(const HomieRange &range, const String &value) {

  if (value != "ON" && value != "OFF")
    return false;

  if (value == "ON") {
    pumpRelayState = LOW;
  } else {
    pumpRelayState = HIGH;
  }

  digitalWrite(PIN_RELAY_PUMP, pumpRelayState);

  if (pumpRelayState == LOW && motorRelayState == HIGH) {
    ledColor(255, 255, 0);
  }
  if (pumpRelayState == LOW && motorRelayState == LOW) {
    ledColor(0, 100, 100);
  }
  if (pumpRelayState == HIGH && motorRelayState == LOW) {
    ledColor(255, 90, 0);
  }
  if (pumpRelayState == HIGH && motorRelayState == HIGH) {
    ledColor(lastLedColor.r, lastLedColor.g, lastLedColor.b);
  }

  relayNode1.setProperty("power").send(value);
  Homie.getLogger() << "Pump is: " << value << endl;

  return true;
}

bool powerOnHandlerMotor(const HomieRange &range, const String &value) {

  if (value != "ON" && value != "OFF")
    return false;

  if (value == "ON") {
    motorRelayState = LOW;
  } else {
    motorRelayState = HIGH;
  }

  digitalWrite(PIN_RELAY_MOTOR, motorRelayState);

  if (motorRelayState == LOW && pumpRelayState == HIGH) {
    ledColor(255, 90, 0);
  }
  if (motorRelayState == LOW && pumpRelayState == LOW) {
    ledColor(0, 100, 100);
  }
  if (motorRelayState == HIGH && pumpRelayState == LOW) {
    ledColor(255, 255, 0);
  }
  if (motorRelayState == HIGH && pumpRelayState == HIGH) {
    ledColor(lastLedColor.r, lastLedColor.g, lastLedColor.b);
  }

  relayNode2.setProperty("power").send(value);
  Homie.getLogger() << "Motor is: " << value << endl;

  return true;
}

bool powerOnHandlerSpeed(const HomieRange &range, const String &value) {

  if (value != "SLOW" && value != "FAST")
    return false;

  if (value == "FAST") {
    speedRelayState = LOW;
  } else {
    speedRelayState = HIGH;
  }

  digitalWrite(PIN_RELAY_SPEED, speedRelayState);
  relayNode3.setProperty("power").send(value);
  Homie.getLogger() << "Motor Speed is: " << value << endl;

  return true;
}
