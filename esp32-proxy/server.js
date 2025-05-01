const express = require('express');
const bodyParser = require('body-parser');
const axios = require('axios');
const cors = require('cors');

const app = express();
const port = 8080;


const ESP32_IP = 'http://192.168.29.236:8080';//esp32 ip address

app.use(cors());
app.use(bodyParser.json());

// Proxy POST /control
app.post('/control', async (req, res) => {
  try {
    const response = await axios.post(`${ESP32_IP}/control`, req.body, {
      headers: { 'Content-Type': 'application/json' },
    });
    console.log('Command forwarded to ESP32:', req.body);
    res.status(200).send('Forwarded to ESP32');
  } catch (error) {
    console.error('Error forwarding to ESP32:', error.message);
    res.status(500).send('Failed to forward to ESP32');
  }
});

// Optional health check
app.get('/', (req, res) => {
  res.send('ESP32 Proxy Server Running');
});

app.listen(port, () => {
  console.log(`Proxy server listening on http://localhost:${port}`);
});
