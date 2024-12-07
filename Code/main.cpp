#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_sleep.h>

// Replace with your network credentials
const char* ssid     = "*********************";  // Enter Your_WiFi_SSID
const char* password = "*********************";  // Enter Your_WiFi_Password

// Replace with your Pushover User Key and API Token
const char* pushover_user_key = "*********************";  // Enter Your_Pushover_User_Key
const char* pushover_api_token = "*********************"; // Enter Your_Pushover_API_Token

// Sensor details
// ADC pin connected to the sensor
const int MonstreaPin = 34, PothosPin = 35 , ElephantPin = 32, ZanzibarGemPin=33;

//34=MONSTERA MONKEY, 35=Pothos, 32=Elephant Ear 33= Zanzibar Gem
int MonstreaValue = 0, PothosValue=0, ElephantValue=0,ZanzibarGemValue=0;       // Variable to store sensor value

 String MonstreaMessage = "",PothosMessage = "",ElephantMessage = "",notificationMessage= "",ZanzibarGemMessage = "";

const String PLANT_NAMES[] = {"Monstrea Monkey", "Pothos", "Elephant Ear", "Zanzibar Gem"};

// RTC memory to store the sleep cycle counter
RTC_DATA_ATTR int sleepCounter = 0;  // Counter to track sleep cycles

const int TOTAL_CYCLES = 24;         // Total cycles for 24 hours (1 hour per cycle)
const uint64_t SLEEP_INTERVAL = 60ULL * 60 * 1000000;  // 1 hour in microseconds


// Function prototypes
void connectToWiFi();
int scaleHumidity(int sensorValue);
String getSoilConditionMessage(int sensorValue, const String& plantName);
void sendPushoverNotification(String message);


void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  delay(1000);

  // Check sleep cycle
  if (sleepCounter < TOTAL_CYCLES) {
    Serial.printf("Current sleep cycle: %d\n", sleepCounter + 1);
    sleepCounter++;  // Increment the sleep counter
    Serial.println("Entering deep sleep for 1 hour...");
    esp_sleep_enable_timer_wakeup(SLEEP_INTERVAL);  // Sleep for 1 hour
    esp_deep_sleep_start();
  } else {
    Serial.println("Completed 24-hour cycle. Performing task...");
    sleepCounter = 0;  // Reset counter
  }

  // Connect to WiFi
  connectToWiFi();

  // Initialize sensor pins
  pinMode(MonstreaPin, INPUT);
  pinMode(PothosPin, INPUT);
  pinMode(ElephantPin, INPUT);
  pinMode(ZanzibarGemPin, INPUT);
}
void loop() {
  
  // Reset sensor values
  MonstreaValue = PothosValue = ElephantValue = ZanzibarGemValue = 0;

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
 

// Print sensor values
Serial.printf("Monstrea Monkey Humidity: %d\n", MonstreaValue);
Serial.printf("Pothos Humidity: %d\n", PothosValue);
Serial.printf("Elephant Ear Humidity: %d\n", ElephantValue);
Serial.printf("Zanzibar-Gem Humidity: %d\n", ZanzibarGemValue);

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

    // Send notification
  sendPushoverNotification(notificationMessage);
  
    // Disconnect WiFi to save power
  Serial.println("Turning off WiFi...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  // Enter deep sleep for the next cycle
  Serial.println("Entering deep sleep for 1 hour...");
  esp_sleep_enable_timer_wakeup(SLEEP_INTERVAL);
  esp_deep_sleep_start();


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
void connectToWiFi() {
  Serial.println("Starting WiFi connection process...");
  WiFi.begin(ssid, password);
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    delay(1000);
    Serial.print(".");
    wifiAttempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
    Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\nFailed to connect to WiFi. Restarting...");
    delay(5000);
    ESP.restart();
  }
}
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
int scaleHumidity(int sensorValue) {
    return constrain(map(sensorValue, 600, 2500, 10, 1), 1, 10); // ACD range: 0-4095 but after Calibration with the actual situation range is: 600-2500
}