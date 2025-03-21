const express = require('express');
const mqtt = require('mqtt');
const app = express();
const port = 3000;

const mqtt_client = mqtt.connect('mqtt://your-local-broker:1883'); // Local broker or AWS IoT
const topic_sub = 'smarthome/data';
const topic_pub = 'smarthome/commands';

app.use(express.static('public'));

mqtt_client.on('connect', () => {
  mqtt_client.subscribe(topic_sub);
  console.log('Connected to MQTT');
});

mqtt_client.on('message', (topic, message) => {
  console.log(`Received: ${message.toString()}`);
});

app.get('/control', (req, res) => {
  const command = req.query.state; // e.g., ?state=ON
  mqtt_client.publish(topic_pub, command);
  res.send(`Sent command: ${command}`);
});

app.listen(port, () => {
  console.log(`Web interface running at http://localhost:${port}`);
});
