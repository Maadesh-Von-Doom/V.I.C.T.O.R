const express = require('express');
const cors = require('cors');

const app = express();
app.use(cors());
app.use(express.json());

// In-Memory Database (Since we are skipping DynamoDB for this Render test)
let latestTelemetry = {};
let pendingCommand = "NONE";
let lastSeen = 0;

// 1. ESP32 sends data here
app.post('/api/telemetry', (req, res) => {
    latestTelemetry = req.body;
    lastSeen = Date.now();
    console.log("Received Telemetry:", latestTelemetry);
    res.json({ status: 'success' });
});

// 2. Web Dashboard fetches data from here
app.get('/api/data', (req, res) => {
    // If we haven't heard from ESP32 in 10 seconds, mark as offline
    const isOnline = (Date.now() - lastSeen) < 10000; 
    res.json({ 
        isOnline: isOnline, 
        latestSensorData: latestTelemetry 
    });
});

// 3. Web Dashboard triggers a reboot here
app.post('/api/reboot', (req, res) => {
    pendingCommand = "REBOOT";
    console.log("Reboot command queued by Web Dashboard.");
    res.json({ status: 'command_queued' });
});

// 4. ESP32 checks for commands here
app.get('/api/command', (req, res) => {
    const cmd = pendingCommand;
    pendingCommand = "NONE"; // Clear it so we don't reboot endlessly
    res.json({ command: cmd });
});

// Health check for Render
app.get('/', (req, res) => {
    res.send('V.I.C.T.O.R. Backend is running.');
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`V.I.C.T.O.R Server running on port ${PORT}`);
});
