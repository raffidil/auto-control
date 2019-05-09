#include <Adafruit_NeoPixel.h>
#include <Homie.h>
#include <TaskScheduler.h>

#define PIN_KEY1 D3 //pomp
#define PIN_KEY2 D1 //cooler

const int PIN_RELAY1 = D0; //pomp
const int PIN_RELAY2 = D2; //motor
const int PIN_RELAY3 = D5; //speed
const int PIN_RELAY4 = D7; //hallway

const int PIN_LED = D6;

long buttonDebounce = 200;

long long buttonTime1 = 0;
long long buttonTime2 = 0;

bool relayState1 = HIGH; //pomp
bool relayState2 = HIGH; //motor
bool relayState3 = HIGH; //speed
bool relayState4 = HIGH; //hallway

bool lastPinValue1 = HIGH;
bool currentValue1;
bool lastPinValue2 = HIGH;
bool currentValue2;


HomieNode relayNode1("relay1", "relay");
HomieNode relayNode2("relay2", "relay");
HomieNode relayNode3("relay3", "relay");
HomieNode relayNode4("relay4", "relay");


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
class color{
  public:
  uint8_t r;
  uint8_t g;
  uint8_t b;
void set(uint8_t red,uint8_t green, uint8_t blue){
  r=red;
  g=green;
  b=blue;
}
  color(){
    r=0;
    g=0;
    b=0;
  }
};
color lastColor;
void syncColor(){
  if(!relayState1){
        if(!relayState2)
            ledColor(0,255,255); //pomp,motor:teal
        if(relayState2)
            ledColor(255,255,0);//pomp:yellow
    }
    if(relayState1){
        if(relayState2)
            ledColor(lastColor.r,lastColor.g,lastColor.b); //all off:off
        if(!relayState2)
            ledColor(255,130,0); //motor:orange
    }
}


void buttonLoop() {
  currentValue1 = digitalRead(PIN_KEY1);
  currentValue2 = digitalRead(PIN_KEY2);


  if (currentValue1 == HIGH && lastPinValue1 == LOW &&
      millis() - buttonTime1 > buttonDebounce) {

      relayState1 = !relayState1;
      syncColor();

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

      relayState2 = !relayState2;
      syncColor();

    digitalWrite(PIN_RELAY2, relayState2);

    Serial.print("relay2");
    Serial.println(relayState2 ? "OFF" : "ON");

    if (Homie.isConfigured() && Homie.isConnected()) {
      relayNode2.setProperty("power").send(relayState2 ? "OFF" : "ON");
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


  pinMode(PIN_KEY1, INPUT_PULLUP);
  pinMode(PIN_KEY2, INPUT_PULLUP);

  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_RELAY1, relayState1);
  digitalWrite(PIN_RELAY2, relayState2);
  digitalWrite(PIN_RELAY3, relayState3);
  digitalWrite(PIN_RELAY4, relayState4);


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
