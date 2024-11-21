#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_sleep.h>

// Replace with your network credentials
const char* ssid     = "**********************";  // Enter Your_WiFi_SSID
const char* password = "**********************";  // Enter Your_WiFi_Password

// Replace with your Pushover User Key and API Token
const char* pushover_user_key = "**********************";  // Enter Your_Pushover_User_Key
const char* pushover_api_token = "**********************"; // Enter Your_Pushover_API_Token

// Sensor details
// ADC pin connected to the sensor
const int MonstreaPin = 34, PothosPin = 35 , ElephantPin = 32, ZanzibarGemPin=33;

//34=MONSTERA MONKEY, 35=Pothos, 32=Elephant Ear 33= Zanzibar Gem
int MonstreaValue = 0, PothosValue=0, ElephantValue=0,ZanzibarGemValue=0;       // Variable to store sensor value

 String MonstreaMessage = "",PothosMessage = "",ElephantMessage = "",notificationMessage= "",ZanzibarGemMessage = "";

const String PLANT_NAMES[] = {"Monstrea Monkey", "Pothos", "Elephant Ear", "Zanzibar Gem"};

const int SLEEP_INTERVAL = 24UL * 60 * 60 * 1000; // 24 hours in milliseconds


int scaleHumidity(int sensorValue) {
    return constrain(map(sensorValue, 600, 2500, 10, 1), 1, 10); // ACD range: 0-4095 but after Calibration with the actual situation range is: 600-2500
}

// Function prototype
String getSoilConditionMessage(int sensorValue, const String& plantName);


void setup() {

  // Start the Serial Monitor
  Serial.begin(115200);
  delay(1000);  // Allow time for Serial Monitor to initialize

  // Attempt to connect to WiFi
  Serial.println("Starting WiFi connection process...");
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");

  int wifiAttempts = 0;  // Keep track of connection attempts
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) { // Try for 20 seconds
    delay(1000);
    Serial.print(".");
    wifiAttempts++;
  }

  // Check if connection was successful
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("Connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("Failed to connect to WiFi. Restarting ESP32...");
    delay(5000); // Wait for 5 seconds before resetting
    ESP.restart();  // Restart the ESP32
  }

  // Init sensor
  pinMode(MonstreaPin,INPUT);
  pinMode(PothosPin,INPUT);
  pinMode(ElephantPin,INPUT);
  pinMode(ZanzibarGemPin, INPUT);

}

void sendPushoverNotification(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Pushover API URL
    http.begin("https://api.pushover.net/1/messages.json");

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Create the HTTP POST request payload
    String httpRequestData = "token=" + String(pushover_api_token)
                           + "&user=" + String(pushover_user_key)
                           + "&message=" + message;

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    // Print the response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    // End HTTP connection
    http.end();
  }
}

void loop() {
  
  // Reset sensor values
    MonstreaValue = 0;
    PothosValue = 0;
    ElephantValue = 0;
    ZanzibarGemValue = 0;

  // Read and average sensor values
  delay(10000); // deley for init the sensors - 10 seconds
  for (int i = 0; i < 25; i++) {
    MonstreaValue += analogRead(MonstreaPin);
    PothosValue += analogRead(PothosPin);
    ElephantValue += analogRead(ElephantPin);
    ZanzibarGemValue += analogRead(ZanzibarGemPin);
    delay(200);  // (5 seconds total)
  }

  MonstreaValue /= 25;
  PothosValue /= 25;
  ElephantValue /= 25;
  ZanzibarGemValue /= 25;
 

// Print the sensor value to the Serial Monitor
Serial.print("Monstrea Monkey Humidity Sensor Value: ");
Serial.println(MonstreaValue);
Serial.print("Pothos Humidity Sensor Value: ");
Serial.println(PothosValue);
Serial.print("Elephant Ear Humidity Sensor Value: ");
Serial.println(ElephantValue);
Serial.print("Zanzibar-Gem Ear Humidity Sensor Value: ");
Serial.println(ZanzibarGemValue);

// map scale peint:
Serial.print("\nScale print: \n");
Serial.print("Monstrea Monkey Humidity Level: ");
Serial.println(scaleHumidity(MonstreaValue));
Serial.print("Pothos Humidity Level: ");
Serial.println(scaleHumidity(PothosValue));
Serial.print("Elephant Ear Humidity Level: ");
Serial.println(scaleHumidity(ElephantValue));
Serial.print("Zanzibar-Gem Humidity Level: ");
Serial.println(scaleHumidity(ZanzibarGemValue));

 // Generate condition messages
  MonstreaMessage = getSoilConditionMessage(MonstreaValue, "Monstrea Monkey");
  PothosMessage = getSoilConditionMessage(PothosValue, "Pothos");
  ElephantMessage = getSoilConditionMessage(ElephantValue, "Elephant Ear");
  ZanzibarGemMessage = getSoilConditionMessage(ZanzibarGemValue, "Zanzibar Gem");

// Combine all messages into a single notification
  notificationMessage = MonstreaMessage + "\n\n" + PothosMessage + "\n\n" + ElephantMessage + "\n\n";  // + ZanzibarGemMessage;

  // Send the notification only if there is a significant change in values
  static String lastNotificationMessage = "";
  if (notificationMessage != lastNotificationMessage) {
    sendPushoverNotification(notificationMessage);
    lastNotificationMessage = notificationMessage;  // Update the last sent message
  } else {
    Serial.println("No significant change in soil moisture. Notification not sent.");
  }
    // Turn off WiFi to save power   
    Serial.println("Turning off WiFi...");
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

   // Configure deep sleep for 24-Hours 
    Serial.println("Entering deep sleep for 24 hours...");
    esp_sleep_enable_timer_wakeup(SLEEP_INTERVAL); 
    esp_deep_sleep_start();
}

// Set thresholds for action based on the sensor value
String getSoilConditionMessage(int sensorValue, const String& plantName) {
    int scaledValue = scaleHumidity(sensorValue); // Map sensor value to scale of 1 to 10

    String message = plantName + " Humidity Level: " + String(scaledValue) + "/10\n";
    if (scaledValue >= 1 && scaledValue <= 2) {
        message += "Time to water! Soil is dry.";
    } else if (scaledValue >= 8 && scaledValue <= 10)
        message += "Too much water! Soil is oversaturated.";
     else 
        message += "Great! Soil is moist.";
        
    return message;
}
