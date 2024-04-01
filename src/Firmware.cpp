// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/hw-reference/esp32/get-started-devkitc.html
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "newGK91";
const char* password = "33445566778899";
const char* mqtt_server = "192.168.68.112"; // ip address mqtt server dimana mosquitto di run 
const char* mqtt_username = "admin";
const char* mqtt_password = "admin";

const int ledPin = 27; // GPIO pin untuk LED
const int buttonPin = 13; // GPIO pin untuk button
unsigned long lastDebounceTime = 0; // Variabel untuk debounce button
unsigned long debounceDelay = 200; // Delay debounce

// config project
long saldo = 108000;




WiFiClient espClient;
PubSubClient client(espClient);

int freq = 10; // Frekuensi awal LED (Hz)
unsigned long previousMillis = 0; // Variabel untuk menyimpan waktu sebelumnya
// funciton setup wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages, karena client tidak menerima apa apa di kosongkan
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && (millis() - lastDebounceTime) > debounceDelay) {
    freq++; // Tambah frekuensi
    lastDebounceTime = millis();
    Serial.print("freq : ");
    Serial.println(freq);
  }

  // Kalkulasi interval waktu berdasarkan frekuensi
  unsigned long interval = 1000 / freq;

  // Toggle LED
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; 
    digitalWrite(ledPin, !digitalRead(ledPin)); // Toggle LED
  }

  // Kirim data NIM-frekuensi ke server MQTT pada interval tertentu (2 sec)
  static unsigned long lastPublishTime = 0;
  if (currentMillis - lastPublishTime >= 2000) {
    lastPublishTime = currentMillis;
    String data = "13519008 : " + String(freq);
    client.publish("data", data.c_str());
    Serial.println(data.c_str());
  }

}
