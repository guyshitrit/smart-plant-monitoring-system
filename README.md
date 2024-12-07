# 🌱 Smart Plant Monitoring System

## **Description**
The Smart Plant Monitoring System is an IoT-based solution designed to help plant owners monitor soil moisture levels effortlessly. Using the ESP32 microcontroller, the system reads soil moisture data from sensors, evaluates the soil condition on a scale of 1-10, and sends push notifications to your phone via the Pushover app. Whether it’s time to water your plants, the soil is moist, or there’s too much water, you’ll always stay informed.

---

## **Features**
- 📡 **WiFi Connectivity**: Connects to your home WiFi network.
- 💧 **Soil Moisture Monitoring**: Reads moisture levels from up to 4 sensors.
- 📱 **Push Notifications**: Alerts you when:
  - Soil is too dry (scale 1-2).
  - Soil moisture is ideal (scale 3-7).
  - Soil is oversaturated with water (scale 8-10).
- 🔋 **Power Efficiency**: Enters deep sleep mode to conserve energy, waking up only once every 24 hours to monitor and send updates.
- 🛠️ **Custom Calibration**: Moisture levels are calibrated to match real-world conditions, mapped to a scale of 1-10 for easy interpretation.

---

## **Components**
1. **ESP32 Microcontroller**
2. **Soil Moisture Sensors** (4 sensors for monitoring multiple plants)
3. **Pushover App** (for receiving notifications)
4. **WiFi Connection**

---

## **Setup Instructions**

### **Hardware Setup**
1. Connect soil moisture sensors to the ESP32:
   - **Monstrea Monkey**: ADC pin 34
   - **Pothos**: ADC pin 35
   - **Elephant Ear**: ADC pin 32
   - **Zanzibar Gem**: ADC pin 33
2. Ensure sensors are placed correctly in the soil of your plants.

### **Software Setup**
1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/smart-plant-monitoring.git
   cd smart-plant-monitoring
 2. Install dependencies using PlatformIO or the Arduino IDE.
 3. Update the WiFi credentials in the code:
 4. ```bash
    const char* ssid = "Your_WiFi_SSID";
    const char* password = "Your_WiFi_Password";
 5. Add your Pushover user key and API token:
 6. ```bash
    const char* pushover_user_key = "Your_Pushover_User_Key";
    const char* pushover_api_token = "Your_Pushover_API_Token";
 7. Flash the code to the ESP32 using your IDE.
 8. Place sensors in soil and power on the ESP32.

### **Notification Setup**
1. Install the Pushover app on your smartphone.
2. Link the app to your Pushover account.
3. Test the system to ensure notifications are received.

### **How It Works**

1. The system initializes and connects to **WiFi**.
2. Sensors measure **soil moisture levels**, which are scaled from **1 to 10**.
3. Based on the scale:
   - **1-2**: Notification to water the plant.
   - **3-7**: Ideal moisture; no action needed.
   - **8-10**: Warning about oversaturated soil.
4. Notifications are sent to your phone through the **Pushover app**.
5. The ESP32 enters **deep sleep mode for 24 hours** to conserve power.

## **Future Enhancements**

- Add a **web interface** for real-time monitoring and calibration.
- Expand support for more sensors dynamically.
- Implement **historical moisture data logging**.

