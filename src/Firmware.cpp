// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/hw-reference/esp32/get-started-devkitc.html
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

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
const char *serverAddress = "192.168.68.110"; // Change this to your server's IP address
const int serverPort = 3000;

// config project
long saldo = -1;
long transactionAmount = -1;

String userNIM = "";
String transactionAmountString = "";

unsigned long previousMillis = 0;
const long screenInterval = 100; // Refresh screenInterval in milliseconds

// State
// 0 -> on init + Connecting to Network
// 1 -> Idle
// 2 -> Setting User  
// 3 -> Transaction

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

// void reconnect() {
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");
//     if (client.connect("ESP32Client", mqtt_username, mqtt_password)) {
//       Serial.println("connected");
//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.print(" try again in 5 seconds");
//       delay(5000);
//     }
//   }
// }

void refreshScreen(){
    // Print the character to serial monitor
  screenString = "STATE : " + String(STATE)  + "\n"  + "NIM : " + userNIM  +"\n" + "Saldo : " + String(saldo)+"\n"+ "Transaction Amount  : " + transactionAmountString+"\n" ;
  Serial.println(screenString);
}

void requestSaldo(const char *nim) {
  if (espClient.connect(serverAddress, serverPort)) {
    Serial.println("Connected to server");

    // Create JSON object
    DynamicJsonDocument doc(100);
    doc["User"] = nim;

    // Serialize JSON to string
    String jsonString;
    serializeJson(doc, jsonString);

    // Send HTTP POST request with JSON payload
    HTTPClient http;
    http.begin(espClient, serverAddress, serverPort, "/saldo");
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonString);
    if (httpResponseCode > 0) {
      // Serial.print("HTTP Response code: ");
      // Serial.println(httpResponseCode);
      String response = http.getString();
      // Serial.print("Response: ");
      // Serial.println(response);
      
      // Parse JSON response
      DynamicJsonDocument responseJson(100);
      DeserializationError error = deserializeJson(responseJson, response);
      if (!error) {
        int tempsaldo = responseJson["saldo"];
        saldo = tempsaldo;
        String message = responseJson["message"];
        // Serial.print("Saldo: ");
        // Serial.println(saldo);
        // Serial.print("Message: ");
        // Serial.println(message);
      } else {
        // Serial.println("Failed to parse JSON response");
      }
    } else {
      // Serial.print("Error on HTTP request: ");
      // Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("Connection failed");
  }
}


void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  STATE = 1;
}

void loop() {
  
  // if (!client.connected()) {
  //   reconnect();
  // }
  client.loop();
  // State Change from Idle
  if (STATE == 1){
    char customKey = customKeypad.getKey();
    
    if (customKey == 'A'){
      STATE = 2;
    }
    if(customKey == 'B'){
      STATE = 3;
    }
  // Handle Define User
  }else if(STATE == 2){
    userNIM = "";
    while (true) {
      char key = customKeypad.getKey(); // Read a key from the keypad
      
      if (key) { // Check if a key is pressed
          if (key == '#') { // If '#' key is pressed, exit the loop
              const char* nimCharArray = userNIM.c_str();
              requestSaldo(nimCharArray);
              STATE = 1;
              break;
          } else { // Otherwise, append the key to the input string
              userNIM += key;
              refreshScreen();
          }
      }
    }  
    
  // Transaction
  }else if(STATE == 3){
    transactionAmount = 0;
    transactionAmountString = "";
    while (true) {
      char key = customKeypad.getKey(); // Read a key from the keypad
      
      if (key) { // Check if a key is pressed
          if (key == '#') { // If '#' key is pressed, exit the loop
              STATE = 1;
              // TODO : Handle Transaction Logic Here 
              if (espClient.connect(serverAddress, serverPort)) {
                // Serial.println("Connected to server");

                // Create JSON object
                DynamicJsonDocument doc(200);
                doc["User"] = userNIM;

                doc["Amount"] = transactionAmountString;

                // Serialize JSON to string
                String jsonString;
                serializeJson(doc, jsonString);

                // Send HTTP POST request with JSON payload
                HTTPClient http;
                http.begin(espClient, serverAddress, serverPort, "/transaction");
                http.addHeader("Content-Type", "application/json");
                int httpResponseCode = http.POST(jsonString);
                if (httpResponseCode > 0) {
                  // TODO : Handle Transaction Status Here
                  // Serial.print("HTTP Response code: ");
                  // Serial.println(httpResponseCode);
                  String response = http.getString();
                  
                  // Serial.print("Response: ");
                  // Serial.println(response);
                  
                } else {
                  // Serial.print("Error on HTTP request: ");
                  // Serial.println(httpResponseCode);
                }
                http.end();

                delay(5000); // Send data every 5 seconds
              } else {
                // Serial.println("Connection failed");
              }

              break;
          } else { // Otherwise, append the key to the input string
              transactionAmountString += key;
              refreshScreen();
          }
      }
    } 
    const char* nimCharArray = userNIM.c_str();
    requestSaldo(nimCharArray);
    transactionAmount = 0;
    transactionAmountString = "";
    STATE = 1;
  };
  unsigned long currentMillis = millis(); // Get the current time
  
  if (currentMillis - previousMillis >= screenInterval) { // Check if it's time to refresh
      previousMillis = currentMillis; // Save the last time the screen was refreshed
      refreshScreen(); // Refresh the screen
  }
}
