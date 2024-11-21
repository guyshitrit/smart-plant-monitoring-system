🌱 Smart Plant Monitoring System
Description
The Smart Plant Monitoring System is an IoT-based solution designed to help plant owners monitor soil moisture levels effortlessly. Using the ESP32 microcontroller, the system reads soil moisture data from sensors, evaluates the soil condition on a scale of 1-10, and sends push notifications to your phone via the Pushover app. Whether it’s time to water your plants, the soil is moist, or there’s too much water, you’ll always stay informed.

Features
📡 WiFi Connectivity: Connects to your home WiFi network.
💧 Soil Moisture Monitoring: Reads moisture levels from up to 4 sensors.
📱 Push Notifications: Alerts you when:
Soil is too dry (scale 1-2).
Soil moisture is ideal (scale 3-7).
Soil is oversaturated with water (scale 8-10).
🔋 Power Efficiency: Enters deep sleep mode to conserve energy, waking up only once every 24 hours to monitor and send updates.
🛠️ Custom Calibration: Moisture levels are calibrated to match real-world conditions, mapped to a scale of 1-10 for easy interpretation.

Components
ESP32 Microcontroller
Soil Moisture Sensors (4 sensors for monitoring multiple plants)
Pushover App (for receiving notifications)
WiFi Connection

Setup Instructions
Hardware Setup
Connect soil moisture sensors to the ESP32:
Monstrea Monkey: ADC pin 34
Pothos: ADC pin 35
Elephant Ear: ADC pin 32
Zanzibar Gem: ADC pin 33
Ensure sensors are placed correctly in the soil of your plants.

Software Setup
Clone this repository:
bash
Copy code
git clone https://github.com/your-username/smart-plant-monitoring.git
cd smart-plant-monitoring
Install dependencies using PlatformIO or the Arduino IDE.
Update the WiFi credentials in the code:
cpp
Copy code
const char* ssid = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";
Add your Pushover user key and API token:
cpp
Copy code
const char* pushover_user_key = "Your_Pushover_User_Key";
const char* pushover_api_token = "Your_Pushover_API_Token";
Flash the code to the ESP32 using your IDE.
Place sensors in soil and power on the ESP32.
Notification Setup
Install the Pushover app on your smartphone.
Link the app to your Pushover account.
Test the system to ensure notifications are received.

How It Works
The system initializes and connects to WiFi.
Sensors measure soil moisture levels, which are scaled from 1 to 10.
Based on the scale:
1-2: Notification to water the plant.
3-7: Ideal moisture; no action needed.
8-10: Warning about oversaturated soil.
Notifications are sent to your phone through the Pushover app.
The ESP32 enters deep sleep mode for 24 hours to conserve power.

Images
📸 (Attach images of your hardware setup and notifications received on the app.)

Future Enhancements
Add a web interface for real-time monitoring and calibration.
Expand support for more sensors dynamically.
Implement historical moisture data logging.
License
This project is open-source under the MIT License. Feel free to contribute and improve the system!
