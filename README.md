<img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/colored.png" width="100%">

# V.I.C.T.O.R.

**Vanguard Infrastructure Command & Threat Observation Relay**

## Executive Summary

During catastrophic natural disasters—such as high-magnitude earthquakes, Category 5 cyclones, or severe flooding—governments and disaster response agencies rely on decentralized emergency shelters, medical outposts, and backup power grids. However, a critical failure in modern disaster management is the "Information Blackout." Once a storm hits, central command loses visibility of these remote shelters. They do not know if a shelter's roof is collapsing, if a secondary storm front is approaching, or if survivors are trapped inside compromised structures. 

**V.I.C.T.O.R.** is engineered to solve this. It is an autonomous, edge-to-cloud IoT disaster management system. Operating as a decentralized digital sentinel, it provides zero-latency situational awareness to disaster commanders. By leveraging edge computing and cloud-native serverless architecture, it translates raw environmental and kinematic data into actionable, life-saving intelligence.

---

## 1. Core Architecture

The V.I.C.T.O.R. network abandons traditional monolithic server design in favor of a highly scalable, decoupled, three-tier cloud architecture:

### Tier 1: The Edge Compute Node (Hardware)
An ESP32-based microcontroller acts as the physical sentinel. Rather than blindly transmitting raw data (which consumes critical bandwidth during telecom network strain), the node utilizes **Edge Computing**. It calculates structural stress algorithms and pressure differentials locally on its own CPU. It only transmits JSON payloads via HTTP POST when necessary, ensuring extreme efficiency.

### Tier 2: The Data Broker API (Cloud Backend)
A stateless, non-blocking Node.js/Express REST API hosted on Render. This layer acts as the high-throughput ingestion engine. It receives the encrypted telemetry payloads, verifies device heartbeats, and securely queues remote execution commands (such as a remote hardware reboot).

### Tier 3: The Command Interface (Frontend)
A custom-engineered HTML5/JavaScript dashboard. Designed for enterprise Command & Control environments, it features dynamic state management, automated threat-level visualizations via Chart.js, and an integrated Generative AI simulation panel that translates raw telemetry into military-style Situation Reports (SITREP).

---

## 2. Hardware and Sensor Deep-Dive

![Hardware Setup](Hardware/Hardware.jpeg)

The physical edge node is built upon a standard ESP32 development board, integrated with three highly specialized sensors. Each sensor was chosen specifically for disaster-triage capabilities.

### Sensor 1: MPU-6050 (6-Axis Kinematic IMU)
* **Function:** Structural Triage and Seismic Activity Detection.
* **The Science:** The MPU-6050 utilizes Micro-Electro-Mechanical Systems (MEMS) to track acceleration across the X, Y, and Z axes. 
* **The Application:** We bolted this sensor mathematically to the structural integrity of the shelter. If an earthquake hits, or cyclonic wind-shear begins to tear the roof apart, the accelerometer detects the violent sway. 
* **Edge Logic:** The onboard CPU calculates the absolute vector force. If `abs(X) > 5.0 m/s²` or `abs(Y) > 5.0 m/s²`, the system instantly triggers a `[CRITICAL] Structural Kinematic Anomaly` alert, warning commanders that the building is nearing collapse.

### Sensor 2: BMP280 (Barometric Pressure & Thermal Probe)
* **Function:** Hyper-Local Severe Weather Forecasting.
* **The Science:** The BMP280 is an absolute barometric pressure sensor, sensitive enough to measure pressure changes equivalent to a change in altitude of just a few centimeters. 
* **The Application:** Standard weather radar is broad and often delayed. V.I.C.T.O.R. monitors the atmosphere directly above the shelter. 
* **Edge Logic:** A rapid drop in barometric pressure below `1000 hPa` is the definitive meteorological indicator of an approaching cyclonic eye or severe storm front. When this threshold is breached, the dashboard locks into a `[WARNING] Rapid Barometric Pressure Drop` state, preparing the shelter for imminent impact.

### Sensor 3: HC-SR501 (Passive Infrared / PIR)
* **Function:** Passive Trapped-Survivor Detection.
* **The Science:** The PIR sensor features dual pyroelectric sensors that detect blackbody radiation (infrared heat) emitted by biological entities. 
* **The Application:** Using cameras inside a shelter consumes massive bandwidth and violates privacy. Furthermore, cameras fail in pitch-black scenarios (e.g., power grid failure). The PIR sensor operates in total darkness. If a shelter suffers a partial collapse, rescue teams do not have to dig blindly. The PIR sensor scans the rubble for thermal movement, allowing the dashboard to flash `[INFO] Human Life-Signs Detected`, directing rescue helicopters to the exact coordinates of living survivors.

### Exact Wiring & Pin Mapping

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
* SDO -> ESP32 GND (Forcing address 0x76 to prevent I2C collision with the MPU6050)

**3. HC-SR501 PIR (Digital Input)**
* VCC -> ESP32 VIN (5V rail required for stable pyroelectric operation)
* GND -> ESP32 GND
* OUT -> ESP32 GPIO 4

---

## 3. Generative AI Synthesis Integration

A human commander overseeing 50 different V.I.C.T.O.R. nodes cannot mathematically process 150 different telemetry charts simultaneously. To solve the cognitive load problem, the dashboard features an **Automated AI Synthesis Engine**.

Simulating the capabilities of Amazon Bedrock, the system ingests the raw arrays of kinematics, atmospheric pressure, and life-sign data, and outputs a synthesized, plain-text Situation Report (SITREP). 
* *Example Output:* "Analysis of the real-time telemetry streams indicates highly stable conditions. Perimeter life-sign scanners detect zero unauthorized movement. Kinematic sensors detect only baseline micro-vibrations..."
This allows non-technical government officials to make immediate logistical decisions.

---

## 4. System Deployment Guide

To replicate and deploy the V.I.C.T.O.R. network, follow standard continuous deployment protocols:

### Phase A: Backend Cloud Deployment
1. Clone this repository to your local machine.
2. Initialize a new Web Service on Render.com.
3. Point the deployment pipeline to the `Backend/` directory. Render will automatically execute `npm install` and run `server.js` based on the provided `package.json`.
4. Secure the live `.onrender.com` URL provided by the platform.

### Phase B: Edge Node Firmware Flashing
1. Open `ESP32/ESP32_Render.ino` via the Arduino IDE.
2. Inject your local 2.4GHz WiFi credentials into the `ssid` and `password` variables.
3. Update the `SERVER_URL_TELEMETRY` and `SERVER_URL_COMMAND` macros with your live Render URL.
4. Compile the C++ firmware and flash it to the ESP32 via USB-UART. 

### Phase C: Command Center Initialization
1. Navigate to the `Frontend/` directory.
2. Update the `BASE_URL` constant within `index.html` (approx. line 360) to match your backend host.
3. Open `index.html` in a standard web browser to initialize the WebSocket/Polling connection to the cloud tier. The dashboard will transition from `System Offline` to `Operational` within 2,000 milliseconds of receiving the first telemetry payload.

<img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/colored.png" width="100%">
