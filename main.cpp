#include <Arduino.h>
#include <DHTesp.h>
#include <HTTPClient.h>
#include <WiFi.h>
#define TIME_TO_SLEEP 600 // in seconds, used if deepsleep = true

// config
const char *ssid_Router = "x";
const char *password_Router = "x";
const char *serverName = "x";
bool useserial = false; // serial for debug
bool deepsleep = true;  // use deep sleep instead of delay()
int sleeptime = 300000; // used if deepsleep = false
int dhtPin = 13;        // data pin

// init
DHTesp dht;

void setup() {
  if (useserial == true) {
    Serial.begin(115200);
    delay(5000); // for serial to come online
  }

  if (deepsleep == true) {
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * 1000000);
  }

  dht.setup(dhtPin, DHTesp::DHT11);
  WiFi.begin(ssid_Router, password_Router);

  if (useserial == true) {
    Serial.println(String("Connecting to ") + ssid_Router);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (useserial == true) {
      Serial.print(".");
    }
  }

  if (useserial == true) {
    Serial.println("\nConnected, IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {

  // (re)connect to wifi
  if (WiFi.status() != WL_CONNECTED) {

    WiFi.reconnect();

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      if (useserial == true) {
        Serial.println(String("Connecting to ") + ssid_Router);
        Serial.print(".");
      }
    }
  }

  if (useserial == true) {
    Serial.println("\nConnected, IP address: ");
    Serial.println(WiFi.localIP());
  }

  // measure
  TempAndHumidity newValues = dht.getTempAndHumidity();

  float temp = newValues.temperature;
  float humidity = newValues.humidity;

  // http
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // request
  String httpRequestData =
      "temp=" + String(temp) + "&humidity=" + String(humidity);
  int httpResponseCode = http.POST(httpRequestData);

  // response
  String response = http.getString();

  // serial debug
  if (useserial == true) {
    Serial.println("Temperature:" + String(temp) +
                   " "
                   "Humidity:" +
                   String(humidity));

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.println(response);
  }

  // disconnect wifi
  WiFi.disconnect();
  if (useserial == true) {
    Serial.println("Disconnected");
  }

  // delay
  if (deepsleep == true) {
    esp_deep_sleep_start();
  } else {
    delay(sleeptime);
  }
}
