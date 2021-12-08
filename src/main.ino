#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "main.h"
#include "secret.h"
#include "matrix_eyes.h"
#include "leds_body.h"

long lastMatricesUpdate = 0;

MatrixEyes eyes = MatrixEyes();
LEDsBody leds = LEDsBody();

void setup() {
  Serial.begin(9600); 

  //wifiConnect();

  eyes.setup();
  leds.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  	long now = millis();
		if (now - lastMatricesUpdate > 200)
		{
			lastMatricesUpdate = now;
			eyes.update();
		}

    leds.update();
}

void wifiConnect(){
  Serial.print(F("Connecting to ")); Serial.println(SSID);
  WiFi.begin(SSID, SSID_PWD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print(F("WiFi connected at IP: ")); Serial.println(WiFi.localIP());
}