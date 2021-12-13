#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "main.h"
#include "secret.h"
#include "matrix_eyes.h"
#include "leds_body.h"

boolean reacting = false;

long lastMatricesUpdate = 0;
long lastReconnectAttempt = 0;
long lastTwitchInteraction = 0;
long reactionTimer = 0;

WiFiClient espClient;
PubSubClient client(espClient);
MatrixEyes eyes = MatrixEyes();
LEDsBody leds = LEDsBody();

void setup() {
    Serial.begin(9600); 

    wifiConnect();
    
    client.setServer(MQTT_SERVER, 1883);
    client.setCallback(callback);

    lastReconnectAttempt = 0;
    lastTwitchInteraction = millis();

    eyes.setup();
    leds.setup();
}

void loop() {
  	long now = millis();
		if (now - lastMatricesUpdate > 200)
		{
			lastMatricesUpdate = now;
			eyes.update();
		}

    leds.update();

    if (now - lastTwitchInteraction > DODO_TO){
        lastTwitchInteraction = now;
        startDodo();
    }

    if ((now - reactionTimer > REACTION_PERIOD) && reacting) {
        reacting = false;
        reactionTimer = now;
        eyes.setState(IDLE_STATE);
        leds.setState(IDLE_STATE);
    }

    if (!client.connected()) {
        now = millis();
        if (now - lastReconnectAttempt > 5000) {
          Serial.printf("Not connected");
          lastReconnectAttempt = now;

          if (reconnect()) {
            lastReconnectAttempt = 0;
          }
        }
    } else {

        client.loop();
    }
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

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    lastTwitchInteraction = millis();

    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        messageTemp += (char)payload[i];
    }
    Serial.println();

    if (String(topic) == "protopotes/protobonnet/send_love") {
        eyes.setState(IN_LOVE_STATE);
        leds.setState(IN_LOVE_STATE);
        reacting = true;
    } else if (String(topic) == "protopotes/protobonnet/angry_mode") {
        eyes.setState(ANGRY_STATE);
        leds.setState(ANGRY_STATE);
    } else if (String(topic) == "protopotes/protobonnet/start_quizz") {
        startQuizz(payload, length);
    } else if (String(topic) == "protopotes/protobonnet/end_quizz") {
        endQuizz(payload, length);
        reacting = true;
    } else if (String(topic) == "protopotes/protobonnet/shitty_quizz") {
        eyes.setState(SHITTY_FLUTE_TIME_STATE);
        leds.setState(SHITTY_FLUTE_TIME_STATE);
    }
}

boolean reconnect() {
    if (client.connect("ProtoBonnet", MQTT_USER, MQTT_PASSWORD)) {
        client.publish("protopotes/protobonnet", "Salut");

        client.subscribe("protopotes/protobonnet/#");
    }
    return client.connected();
}


void startQuizz(byte* payload, unsigned int length){
    StaticJsonDocument<256> doc;
    deserializeJson(doc, payload, length);

    eyes.setState(QUIZZ_STATE);
    leds.setQuizzLEDs(doc["r"], doc["g"], doc["b"]);
    leds.setState(QUIZZ_STATE);
}

void endQuizz(byte* payload, unsigned int length){
    StaticJsonDocument<256> doc;
    deserializeJson(doc, payload, length);

    if (doc["victory"] == true){
        eyes.setState(HAPPY_STATE);
        leds.setState(IDLE_STATE);
    } else {
        eyes.setState(ANGRY_STATE);
        leds.setState(ANGRY_STATE);
    }
}

void startDodo(){
    eyes.setState(DODO_STATE);
    leds.setState(DODO_STATE);
}