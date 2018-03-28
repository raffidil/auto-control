#include <Homie.h>
#include <TaskScheduler.h>
#include <Adafruit_NeoPixel.h>

#define PIN_BUTTON_PUMP D3
#define PIN_BUTTON_MOTOR D1
#define PIN_BUTTON_SPEED D7

long buttonDebounce = 200;

const int PIN_RELAY_PUMP = D5;
const int PIN_RELAY_MOTOR = D2;
const int PIN_RELAY_SPEED = D0;

const int PIN_LED = D6;

bool lastPinValue1 = HIGH;
bool pumpRelayState = HIGH;
bool currentValue1;
long long buttonTime1 = 0;

bool lastPinValue2 = HIGH;
bool motorRelayState = HIGH;
bool currentValue2;
long long buttonTime2 = 0;

bool lastPinValue3 = HIGH;
bool speedRelayState = HIGH;
bool currentValue3;
long long buttonTime3 = 0;

struct color{
  uint8_t r=91;
  uint8_t g=92;
  uint8_t b=93;
};

color lastLedColor;

HomieNode relayNode1("coolerpump", "relay");
HomieNode relayNode2("coolermotor", "relay");
HomieNode relayNode3("coolerspeed", "relay");

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
void lastLedColorSave(color *c,uint8_t red, uint8_t green, uint8_t blue){
  c->r=red;
  c->g=green;
  c->b=blue;
}


void buttonLoop() {
  currentValue1 = digitalRead(PIN_BUTTON_PUMP);
  currentValue2 = digitalRead(PIN_BUTTON_MOTOR);
  currentValue3 = digitalRead(PIN_BUTTON_SPEED);

 if (currentValue1 == HIGH && lastPinValue1 == LOW && millis() - buttonTime1 > buttonDebounce) {
   pumpRelayState = !pumpRelayState;
        digitalWrite(PIN_RELAY_PUMP, pumpRelayState);

        if(pumpRelayState == LOW && motorRelayState == HIGH){
          ledColor(255, 255, 0);
        }
        if(pumpRelayState == LOW && motorRelayState == LOW){
          ledColor(0, 100, 100);
        }
        if(pumpRelayState == HIGH && motorRelayState == LOW){
          ledColor(255, 90, 0);
        }
        if(pumpRelayState == HIGH && motorRelayState == HIGH){
          ledColor(lastLedColor.r,lastLedColor.g,lastLedColor.b);
        }

        Serial.print("pump is: ");
        Serial.println(pumpRelayState ? "OFF" : "ON");
        if(Homie.isConfigured() && Homie.isConnected()){
          relayNode1.setProperty("power").send(pumpRelayState ? "OFF" : "ON");
        }
   buttonTime1 = millis();
 }

 if (currentValue2 == HIGH && lastPinValue2 == LOW && millis() - buttonTime2 > buttonDebounce) {
   motorRelayState = !motorRelayState;
        digitalWrite(PIN_RELAY_MOTOR, motorRelayState);

        if(motorRelayState == LOW && pumpRelayState == HIGH){
          ledColor(255, 90, 0);
        }
        if(motorRelayState == LOW && pumpRelayState == LOW){
          ledColor(0, 100, 100);
        }
        if(motorRelayState == HIGH && pumpRelayState == LOW){
          ledColor(255, 255, 0);
        }
        if(motorRelayState == HIGH && pumpRelayState == HIGH){
          ledColor(lastLedColor.r,lastLedColor.g,lastLedColor.b);
        }

        Serial.print("motor is: ");
        Serial.println(motorRelayState ? "OFF" : "ON");
        if(Homie.isConfigured() && Homie.isConnected()){
          relayNode2.setProperty("power").send(motorRelayState ? "OFF" : "ON");
        }
   buttonTime2 = millis();
 }

 if (currentValue3 == HIGH && lastPinValue3 == LOW && millis() - buttonTime1 > buttonDebounce) {
   speedRelayState = !speedRelayState;
        digitalWrite(PIN_RELAY_SPEED, speedRelayState);
        Serial.print("speed is: ");
        Serial.println(speedRelayState ? "SLOW" : "FAST");
        if(Homie.isConfigured() && Homie.isConnected()){
          relayNode3.setProperty("power").send(speedRelayState ? "SLOW" : "FAST");
        }
   buttonTime3 = millis();
 }


 lastPinValue1 = currentValue1;
 lastPinValue2 = currentValue2;
 lastPinValue3 = currentValue3;
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  delay(2000);
  //relay and button
  pinMode(PIN_RELAY_PUMP, OUTPUT);
  pinMode(PIN_RELAY_MOTOR, OUTPUT);
  pinMode(PIN_RELAY_SPEED, OUTPUT);

  pinMode(PIN_BUTTON_PUMP, INPUT_PULLUP);
  pinMode(PIN_BUTTON_MOTOR, INPUT_PULLUP);
  pinMode(PIN_BUTTON_SPEED, INPUT_PULLUP);

  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_RELAY_PUMP, pumpRelayState);
  digitalWrite(PIN_RELAY_MOTOR, motorRelayState);
  digitalWrite(PIN_RELAY_SPEED, speedRelayState);

  // lastPinValue1 = digitalRead(PIN_BUTTON_PUMP);
  // lastPinValue2 = digitalRead(PIN_BUTTON_MOTOR);

  Serial.print("lastPinValue1: ");
  Serial.println(lastPinValue1);
  Serial.print("lastPinValue2: ");
  Serial.println(lastPinValue2);
  Serial.print("lastPinValue3: ");
  Serial.println(lastPinValue3);

  Serial.println(lastLedColor.r);
  Serial.println(lastLedColor.g);
  Serial.println(lastLedColor.b);
  lastLedColorSave(&lastLedColor,50,51,52);
  Serial.println(lastLedColor.r);
  Serial.println(lastLedColor.g);
  Serial.println(lastLedColor.b);


  runner.init();
    runner.addTask(buttonTask);
    runner.addTask(homieTask);
    delay(100);

    buttonTask.enable();
    homieTask.enable();
    Serial.println("end of main Setup()");
}

void loop() {
  runner.execute();

}
