# AnalogRead_Radar_Wifi_V3
esp32 to netpie

Arduino ESP32-CAM
https://netpie.io/

![Screenshot 2024-05-14 114802](https://github.com/QuadTinnakon/AnalogRead_Radar_Wifi_V3/assets/9403558/b8498803-cc7a-4aad-b8c8-91e96ff43567)

![Screenshot 2024-05-14 133144](https://github.com/QuadTinnakon/AnalogRead_Radar_Wifi_V3/assets/9403558/a4ccb563-c21b-4c46-9efa-15bed258026f)

โดย Device Schema จะประกาศในรูปแบบ JSON มีลักษณะดังนี้
```js
{
    "additionalProperties": false,
    "properties": {
        "power": {
            "operation": {
                "store": {
                    "ttl": "30d"
                },
                "transform": {
                    "expression": "($.power * 1.8) + 32"
                }
            },
            "type": "number"
        },
       "voltage": {
              "operation": {
                "store": {
                  "ttl": "7m"
                }
              },
              "type": "number"
            },
    "Ampere": {
              "operation": {
                "store": {
                  "ttl": "7m"
                }
              },
              "type": "number"
            },
        "Time": {
              "operation": {
                "store": {
                  "ttl": "7m"
                }
              },
              "type": "number"
            }
    }
}
```
![263079](https://github.com/QuadTinnakon/AnalogRead_Radar_Wifi_V3/assets/9403558/86190ba9-919c-4014-8a4b-e368bd6e396b)
