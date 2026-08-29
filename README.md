<img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/colored.png" width="100%">

<h1 align="center">V.I.C.T.O.R.</h1>
<h3 align="center">Vanguard Infrastructure Command & Threat Observation Relay</h3>

<p align="center">
  <img src="https://img.shields.io/badge/Domain-Disaster%20Management%20%26%20IoT-blue?style=for-the-badge" alt="Domain">
  <img src="https://img.shields.io/badge/Hardware-ESP32%20%7C%20MEMS%20IMU-orange?style=for-the-badge" alt="Hardware">
  <img src="https://img.shields.io/badge/Backend-Node.js%20%7C%20Express-brightgreen?style=for-the-badge" alt="Backend">
  <img src="https://img.shields.io/badge/Frontend-HTML5%20%7C%20Chart.js-yellow?style=for-the-badge" alt="Frontend">
  <img src="https://img.shields.io/badge/Architecture-Edge%20Computing-purple?style=for-the-badge" alt="Architecture">
</p>

## Executive Summary

During catastrophic natural disasters, emergency shelters and critical government infrastructure require continuous monitoring. A primary failure point in standard disaster management is the loss of ground-level structural and environmental data when telecom networks are strained.

V.I.C.T.O.R. is an edge-to-cloud IoT telemetry system. It provides remote monitoring capabilities for decentralized emergency shelters. By utilizing edge computing and a highly scalable serverless backend, the system aggregates environmental and kinematic data, formatting it into actionable status reports for remote administrators.

---

## 1. System Architecture

The network utilizes a decoupled, three-tier architecture:

### Tier 1: The Edge Compute Node (Hardware)
An ESP32 microcontroller interfaces with environmental and kinematic sensors. To reduce payload overhead, the node utilizes edge computing, processing structural stress algorithms and pressure differentials locally. It transmits JSON payloads via HTTP POST solely when defined environmental thresholds are met or during standard polling intervals.

### Tier 2: The Data Broker API (Backend)
A stateless Node.js/Express REST API. This layer serves as the data ingestion endpoint. It parses incoming telemetry payloads, verifies device heartbeats, and manages the remote execution queue for hardware commands.

### Tier 3: The Command Interface (Frontend)
A web-based HTML5/JavaScript dashboard. It features strict state management, time-series data visualization via Chart.js, and an automated threat assessment board that categorizes incoming data against predefined safety thresholds.

---

## 2. Hardware and Sensor Data

![Hardware Setup](Hardware/Hardware.jpeg)

The edge node utilizes a standard ESP32 development board connected to three primary sensors. 

### Sensor 1: MPU-6050 (6-Axis Kinematic IMU)
* **Function:** Structural Triage and Seismic Activity Monitoring.
* **Mechanism:** The MPU-6050 utilizes Micro-Electro-Mechanical Systems (MEMS) to track acceleration across the X, Y, and Z axes. 
* **Application:** Mounted to the primary support structure of a shelter, the accelerometer measures physical displacement. 
* **Edge Logic:** The microcontroller calculates the absolute vector force. If `abs(X) > 5.0 m/s²` or `abs(Y) > 5.0 m/s²`, the system triggers a structural kinematic anomaly alert, indicating potential threshold failure of the building's support structure.

### Sensor 2: BMP280 (Barometric Pressure & Thermal Probe)
* **Function:** Localized Meteorological Tracking.
* **Mechanism:** An absolute barometric pressure sensor capable of measuring high-resolution atmospheric variance.
* **Application:** Monitors the atmosphere directly at the shelter location to detect approaching low-pressure systems.
* **Edge Logic:** A persistent drop in barometric pressure below `1000 hPa` is a meteorological indicator of a severe cyclonic front. When this threshold is breached, the telemetry payload flags a barometric warning state.

### Sensor 3: HC-SR501 (Passive Infrared / PIR)
* **Function:** Passive Human Presence Detection.
* **Mechanism:** Dual pyroelectric sensors detect blackbody radiation (infrared heat) emitted by biological entities.
* **Application:** Operates independent of ambient light and requires extremely low bandwidth compared to visual camera feeds. In the event of a structural collapse, the sensor scans the immediate vicinity for thermal movement, allowing response teams to confirm human presence within the sector prior to excavation operations.

### Hardware Pin Mapping

**1. MPU6050 (I2C Address: 0x68)**
* VCC -> ESP32 3.3V
* GND -> ESP32 GND
* SDA -> ESP32 GPIO 21
* SCL -> ESP32 GPIO 22

**2. BMP280 (I2C Address: 0x76)**
* VCC -> ESP32 3.3V
* GND -> ESP32 GND
* SDA -> ESP32 GPIO 21
* SCL -> ESP32 GPIO 22
* SDO -> ESP32 GND (Forced address configuration)

**3. HC-SR501 PIR (Digital Input)**
* VCC -> ESP32 VIN (5V rail)
* GND -> ESP32 GND
* OUT -> ESP32 GPIO 4

---

## 3. Automated Telemetry Aggregation

To reduce cognitive load on remote administrators monitoring multiple nodes simultaneously, the frontend dashboard implements an automated data parsing function. 

Instead of requiring operators to manually interpret raw X/Y/Z arrays and hPa decimals, the system aggregates the numerical telemetry and outputs a formatted, plain-text Situation Report (SITREP). This function uses deterministic logic to convert safe threshold values into standardized clearance messages (e.g., "Kinematic sensors detect only baseline micro-vibrations"), streamlining the logistical decision-making process.

---

## 4. Deployment Initialization

### Phase A: Backend Deployment
1. Clone this repository.
2. Initialize a new Web Service on Render (or equivalent Node.js hosting environment).
3. Connect the repository, targeting the `Backend/` directory. 
4. The deployment environment will execute `npm install` and run `server.js` automatically.

### Phase B: Edge Node Configuration
1. Open `ESP32/ESP32_Render.ino` via the Arduino IDE.
2. Input target 2.4GHz WiFi credentials into the `ssid` and `password` variables.
3. Update `SERVER_URL_TELEMETRY` and `SERVER_URL_COMMAND` with the backend host URL.
4. Compile and flash the firmware to the ESP32 via USB-UART. 

### Phase C: Dashboard Initialization
1. Navigate to the `Frontend/` directory.
2. Update the `BASE_URL` constant within `index.html` to match the backend host.
3. Open `index.html` in a web browser. The dashboard will initialize polling and synchronize with the edge node telemetry.

<img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/colored.png" width="100%">
