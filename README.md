# Run
- Add `config.json` to `data/homie/`
- `pio run --target uploadfs`
- `pio run --target upload`


To set config file, make `config.json` in `data/homie` folder and use example below

`{
  "name": "Relay",
  "device_id": "control",
  "wifi": {
    "ssid": "WIFI_SSID ",
    "password": "WIFI_PASSWORD"
  },
  "mqtt": {
    "host": "MQTT_HOST_IP",
    "base_topic": "/home/",
    "port": 1883
  },
  "ota": {
    "enabled": false
  }
}`
