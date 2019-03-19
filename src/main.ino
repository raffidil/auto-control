#include <Adafruit_NeoPixel.h>
#include <Homie.h>
#include <TaskScheduler.h>

#define PIN_KEY1 D3
#define PIN_KEY2 D1

const int PIN_RELAY1 = D0;
const int PIN_RELAY2 = D2;
const int PIN_RELAY3 = D5;
const int PIN_RELAY4 = D7;
const int PIN_RELAY5 = D4;

const int PIN_LED = D6;

long buttonDebounce = 200;

long long buttonTime1 = 0;
long long buttonTime2 = 0;

bool relayState1 = HIGH;
bool relayState2 = HIGH;
bool relayState3 = HIGH;
bool relayState4 = HIGH;
bool relayState5 = HIGH;



bool lastPinValue1 = HIGH;
bool currentValue1;
bool lastPinValue2 = HIGH;
bool currentValue2;


HomieNode relayNode1("relay1", "relay");
HomieNode relayNode2("relay2", "relay");
HomieNode relayNode3("relay3", "relay");
HomieNode relayNode4("relay4", "relay");
HomieNode relayNode5("relay5", "relay");



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



  if (currentValue1 == HIGH && lastPinValue1 == LOW &&
      millis() - buttonTime1 > buttonDebounce) {

      relayState1 = !relayState1;

    digitalWrite(PIN_RELAY1, relayState1);

    Serial.print("relay1: ");
    Serial.println(relayState1 ? "OFF" : "ON");

    if (Homie.isConfigured() && Homie.isConnected()) {
      relayNode1.setProperty("power").send(relayState1 ? "OFF" : "ON");
    }

    buttonTime1 = millis();
  }

  if (currentValue2 == HIGH && lastPinValue2 == LOW &&
      millis() - buttonTime2 > buttonDebounce) {
        
     bool nextLightsState= !(relayState2 && relayState3 && relayState4 && relayState5);
      relayState2=nextLightsState;
      relayState3=nextLightsState;
      relayState4=nextLightsState;
      relayState5=nextLightsState;

    digitalWrite(PIN_RELAY2, relayState2);
    digitalWrite(PIN_RELAY3, relayState3);
    digitalWrite(PIN_RELAY4, relayState4);
    digitalWrite(PIN_RELAY5, relayState5);


    Serial.print("relay2: ");
    Serial.println(relayState2 ? "OFF" : "ON");
    Serial.print("relay3: ");
    Serial.println(relayState3 ? "OFF" : "ON");
    Serial.print("relay4: ");
    Serial.println(relayState4 ? "OFF" : "ON");
    Serial.print("relay5: ");
    Serial.println(relayState5 ? "OFF" : "ON");
    if (Homie.isConfigured() && Homie.isConnected()) {
      relayNode2.setProperty("power").send(relayState2 ? "OFF" : "ON");
      relayNode3.setProperty("power").send(relayState3 ? "OFF" : "ON");
      relayNode4.setProperty("power").send(relayState4 ? "OFF" : "ON");
      relayNode5.setProperty("power").send(relayState5 ? "OFF" : "ON");
    }

    buttonTime2 = millis();
  }

  lastPinValue1 = currentValue1;
  lastPinValue2 = currentValue2;
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie.disableLedFeedback(); // before Homie.setup()

  delay(2000);

  pinMode(PIN_RELAY1, OUTPUT);
  pinMode(PIN_RELAY2, OUTPUT);
  pinMode(PIN_RELAY3, OUTPUT);
  pinMode(PIN_RELAY4, OUTPUT);
  pinMode(PIN_RELAY5, OUTPUT);


  pinMode(PIN_KEY1, INPUT_PULLUP);
  pinMode(PIN_KEY2, INPUT_PULLUP);

  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_RELAY1, relayState1);
  digitalWrite(PIN_RELAY2, relayState2);
  digitalWrite(PIN_RELAY3, relayState3);
  digitalWrite(PIN_RELAY4, relayState4);
  digitalWrite(PIN_RELAY5, relayState5);


  Serial.print("lastPinValue1: ");
  Serial.println(lastPinValue1);
  Serial.print("lastPinValue2: ");
  Serial.println(lastPinValue2);

  runner.init();
  runner.addTask(buttonTask);
  runner.addTask(homieTask);
  delay(100);

  buttonTask.enable();
  homieTask.enable();
  Serial.println("end of main Setup()");
}

void loop() { runner.execute(); }
