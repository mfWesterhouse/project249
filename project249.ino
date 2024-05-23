#include <WiFi.h>
#include <Adafruit_BMP085.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define WLAN_SSID       "WR3005N3-757E"
#define WLAN_PASS       "70029949"


#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                
#define AIO_USERNAME  "Tamtap"
#define AIO_KEY       "aio_ozIw67GZ2KKiwjN7Uvz5HDQeiuY0"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");

Adafruit_MQTT_Publish level = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/level");

Adafruit_MQTT_Subscribe sw1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/sw1");

Adafruit_MQTT_Subscribe sw2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/sw2");

void MQTT_connect();

const int led1 = 18;
const int led2 = 19;

float p;
float q;

String stringOne, stringTwo;

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);

  Serial.println(F("Adafruit MQTT demo"));

  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  mqtt.subscribe(&sw1);

  mqtt.subscribe(&sw2);

  if (!bmp.begin()) {
    Serial.println("BMP180 Sensor not found ! ! !");
    while (1) {}
  }
}

uint32_t x = 0;

void loop() {


  p = bmp.readPressure();
  q = bmp.readTemperature();
  Serial.println(p);
  Serial.println(q);
  delay(100);

  MQTT_connect();



  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &sw1) {

      stringOne = (char *)sw1.lastread;
      Serial.print(F("stringOne: "));
      Serial.println(stringOne);

      if (stringOne == "ON") {
        digitalWrite(led1, HIGH);
      }
      if (stringOne == "OFF") {
        digitalWrite(led1, LOW);
      }
    }

    if (subscription == &sw2) {

      stringTwo = (char *)sw2.lastread;
      Serial.print(F("stringTwo: "));
      Serial.println(stringTwo);

      if (stringTwo == "ON") {
        digitalWrite(led2, HIGH);
      }
      if (stringTwo == "OFF") {
        digitalWrite(led2, LOW);
      }

    }
  }


  if (! temperature.publish(q)) {

  } else {

  }

  if (! level.publish(p)) {
    
  } else {
   
  }


}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 

    mqtt.disconnect();
    delay(5000);  
    retries--;
    if (retries == 0) {
      while (1);
    }
  }

}
