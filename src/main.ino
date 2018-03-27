#include <Homie.h>
#include <TaskScheduler.h>

const int PIN_RELAY = D6;
const int PIN_KEY = D3;

int lastPinValue = LOW;
bool relayState = LOW;
int currentValue;

void buttonLoop();
void homieLoop();
bool homieSetup();

Scheduler runner;
Task buttonTask(50, TASK_FOREVER, &buttonLoop, NULL, false);
Task homieTask(10, TASK_FOREVER, &homieLoop, NULL, false, &homieSetup);

HomieNode relayNode("relay1", "relay");
Bounce debouncer = Bounce();

void setupHandler() {
  Serial.println("setupHandler is called" );
  digitalWrite(PIN_RELAY, relayState);
  relayNode.setProperty("power").send("OFF");
}



bool powerOnHandler(const HomieRange& range, const String& value) {
  Serial.println("powerOnHandler is called" );

  if (value != "ON" && value != "OFF") return false;
  Serial.println("powerOnHandler IF is called" );

  relayState = (value == "ON");
  digitalWrite(PIN_RELAY, relayState ? LOW : HIGH);
  relayNode.setProperty("power").send(value);
  Homie.getLogger() << "Power is " << (relayState ? "on" : "off") << endl;

  return true;
}

void onHomieEvent(const HomieEvent& event) {
  switch(event.type) {
    case HomieEventType::STANDALONE_MODE:
      // Do whatever you want when standalone mode is started
      break;
    case HomieEventType::CONFIGURATION_MODE:
      // Do whatever you want when configuration mode is started
      break;
    case HomieEventType::NORMAL_MODE:
      // Do whatever you want when normal mode is started
      break;
    case HomieEventType::OTA_STARTED:
      // Do whatever you want when OTA is started
      break;
    case HomieEventType::OTA_PROGRESS:
      // Do whatever you want when OTA is in progress

      // You can use event.sizeDone and event.sizeTotal
      break;
    case HomieEventType::OTA_FAILED:
      // Do whatever you want when OTA is failed
      break;
    case HomieEventType::OTA_SUCCESSFUL:
      // Do whatever you want when OTA is successful
      break;
    case HomieEventType::ABOUT_TO_RESET:
      // Do whatever you want when the device is about to reset
      break;
    case HomieEventType::WIFI_CONNECTED:
      // Do whatever you want when Wi-Fi is connected in normal mode

      // You can use event.ip, event.gateway, event.mask
      break;
    case HomieEventType::WIFI_DISCONNECTED:
      // Do whatever you want when Wi-Fi is disconnected in normal mode

      // You can use event.wifiReason
      break;
    case HomieEventType::MQTT_READY:
      // Do whatever you want when MQTT is connected in normal mode
      break;
    case HomieEventType::MQTT_DISCONNECTED:
      // Do whatever you want when MQTT is disconnected in normal mode

      // You can use event.mqttReason
      break;
    case HomieEventType::MQTT_PACKET_ACKNOWLEDGED:
      // Do whatever you want when an MQTT packet with QoS > 0 is acknowledged by the broker

      // You can use event.packetId
      break;
    case HomieEventType::READY_TO_SLEEP:
      // After you've called `prepareToSleep()`, the event is triggered when MQTT is disconnected
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  delay(2000);
  pinMode(PIN_RELAY, OUTPUT);
  Serial.println("PIN_RELAY output" );
  pinMode(PIN_KEY, INPUT);
  Serial.println("PIN_KEY input" );

  debouncer.attach(PIN_KEY);
  debouncer.interval(50);

  runner.init();
    runner.addTask(buttonTask);
    Serial.println("button task added");
    runner.addTask(homieTask);
    Serial.println("homie task added");
    delay(100);

    buttonTask.enable();
    Serial.println("buttonTask ENABLED");
    homieTask.enable();
    Serial.println("homieTask ENABLED");
    Serial.println("end of main Setup()");
}

bool homieSetup(){
  Homie_setFirmware("relay", "1.0.0"); // The underscore is not a typo! See Magic bytes
  Homie.setSetupFunction(setupHandler);

  relayNode.advertise("power").settable(powerOnHandler);
  Homie.onEvent(onHomieEvent); // before Homie.setup()
  Homie.setup();
  return true;
}

void homieLoop(){
  Homie.loop();
  //Homie.reset();
}

void buttonLoop() {
  currentValue = digitalRead(PIN_KEY);     //relayNode.setProperty("power").send(relayState ? "ON" : "OFF");

  if (currentValue != lastPinValue) {
    Serial.println("buttonLoop IF called" );
    relayState = !relayState;
     digitalWrite(PIN_RELAY, relayState);
     if(Homie.isConfigured() && Homie.isConnected()){
       relayNode.setProperty("power").send(relayState ? "ON" : "OFF");
       Serial.println("button IF and Homie IF");
     }
  }
  lastPinValue = currentValue;
}

void loop() {
  runner.execute();

}
