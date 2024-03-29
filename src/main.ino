#include <Adafruit_NeoPixel.h>
#include <Homie.h>
#include <TaskScheduler.h>

#define PIN_KEY1 D3
#define PIN_KEY2 D1
//#define PIN_KEY3 D7

const int PIN_RELAY1 = D5;
const int PIN_RELAY2 = D2;
// const int PIN_RELAY3 = D4;

const int PIN_LED = D6;

bool lastPinValue1 = HIGH;
bool relayState1 = HIGH;
bool currentValue1;

bool lastPinValue2 = HIGH;
bool relayState2 = HIGH;
bool currentValue2;

// bool lastPinValue3 = HIGH;
// bool relayState3 = HIGH;
// bool currentValue3;

HomieNode relayNode1("relay1", "relay");
HomieNode relayNode2("relay2", "relay");
// HomieNode relayNode3("relay3", "relay");

void buttonLoop();
void homieLoop();
bool homieSetup();

Scheduler runner;
Task buttonTask(50, TASK_FOREVER, &buttonLoop, NULL, false);
Task homieTask(10, TASK_FOREVER, &homieLoop, NULL, false, &homieSetup);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN_LED, NEO_GRB + NEO_KHZ800);
void ledColor(uint8_t r, uint8_t g, uint8_t b) {
  strip.setPixelColor(0, r, g, b);
  strip.show();
}

void buttonLoop() {
  currentValue1 = digitalRead(PIN_KEY1);
  currentValue2 = digitalRead(PIN_KEY2);
  // currentValue3 = digitalRead(PIN_KEY3);

  if (currentValue1 != lastPinValue1) {
    relayState1 = !relayState1;
    digitalWrite(PIN_RELAY1, relayState1);
    Serial.print("relay1: ");
    Serial.println(relayState1 ? "OFF" : "ON");
    if (Homie.isConfigured() && Homie.isConnected()) {
      relayNode1.setProperty("power").send(relayState1 ? "OFF" : "ON");
    }
  }

  if (currentValue2 != lastPinValue2) {
    relayState2 = !relayState2;
    digitalWrite(PIN_RELAY2, relayState2);
    Serial.print("relay2: ");
    Serial.println(relayState2 ? "OFF" : "ON");
    if (Homie.isConfigured() && Homie.isConnected()) {
      relayNode2.setProperty("power").send(relayState2 ? "OFF" : "ON");
    }
  }

  // if (currentValue3 != lastPinValue3) {
  //   relayState3 = !relayState3;
  //    digitalWrite(PIN_RELAY3, relayState3);
  //    Serial.print("relay3: ");
  //    Serial.println(relayState3 ? "OFF" : "ON");
  //    if(Homie.isConfigured() && Homie.isConnected()){
  //      relayNode3.setProperty("power").send(relayState3 ? "OFF" : "ON");
  //    }
  // }
  lastPinValue1 = currentValue1;
  lastPinValue2 = currentValue2;
  // lastPinValue3 = currentValue3;
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  delay(2000);
  // relay and button
  pinMode(PIN_RELAY1, OUTPUT);
  pinMode(PIN_RELAY2, OUTPUT);
  // pinMode(PIN_RELAY3, OUTPUT);

  pinMode(PIN_KEY1, INPUT_PULLUP);
  pinMode(PIN_KEY2, INPUT_PULLUP);
  // pinMode(PIN_KEY3, INPUT_PULLUP);

  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_RELAY1, relayState1);
  digitalWrite(PIN_RELAY2, relayState2);
  // digitalWrite(PIN_RELAY3, relayState3);

  // lastPinValue1 = digitalRead(PIN_KEY1);
  // lastPinValue2 = digitalRead(PIN_KEY2);

  Serial.print("lastPinValue1: ");
  Serial.println(lastPinValue1);
  Serial.print("lastPinValue2: ");
  Serial.println(lastPinValue2);
  // Serial.print("lastPinValue3: ");
  // Serial.println(lastPinValue3);

  runner.init();
  runner.addTask(buttonTask);
  runner.addTask(homieTask);
  delay(100);

  buttonTask.enable();
  homieTask.enable();
  Serial.println("end of main Setup()");
}

void loop() { runner.execute(); }
