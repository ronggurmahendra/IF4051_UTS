// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/hw-reference/esp32/get-started-devkitc.html
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 12, 14, 27}; /* connect to the row pinouts of the keypad */
byte colPins[COLS] = {26, 25, 33, 32}; /* connect to the column pinouts of the keypad */

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
String screenString = ""; // Declare a string variable to hold the input characters

const char* ssid = "newGK91";
const char* password = "33445566778899";
const char* mqtt_server = "192.168.68.112"; // ip address mqtt server dimana mosquitto di run 
const char* mqtt_username = "admin";
const char* mqtt_password = "admin";

// config project
long saldo = 108000;
// State
// 0 -> on init + Connecting to Network
// 1 -> Idle
// 2 -> Insert NIM 
// 3 -> Insert Saldo
// 4 -> Transaction Succsess
// 5 -> Transaction Failed
int STATE = 0;


WiFiClient espClient;
PubSubClient client(espClient);



// funciton setup wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.print("");
  Serial.print("WiFi connected\n");
  Serial.print("IP address: ");
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
      Serial.print(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  // if (!client.connected()) {
  //   reconnect();
  // }
  // client.loop();
  char customKey = customKeypad.getKey();
  
  if (customKey){
      screenString += customKey; // Append the character to the string
        
  }
  // Print the character to serial monitor
  Serial.print(screenString);
}
