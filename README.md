# Arduino movement sensor mqtt

[![Compile Sketch](https://github.com/jehy/arduino-movement-sensor-mqtt/actions/workflows/compile-sketch.yaml/badge.svg)](https://github.com/jehy/arduino-came-reader/actions/workflows/compile-sketch.yaml)
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.me/jehyrus)

Simple movement sensor + MQTT sketch for Arduino.

Sample config for Home Assistant:

```yaml
binary_sensor:
  - platform: mqtt
  unique_id: esp-move-1
  name: "ESP move 1"
  state_topic: "esp/home/move/1"
  payload_on: "1"
  payload_off: "0"
  device_class: "motion"
  qos: 0
```
