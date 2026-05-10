The NodeMCU Code (Wi-Fi & MQTT logic)

CODE:-
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// --- USER CONFIGURATION START ---
// 1. WiFi Credentials
const char* ssid = "YOUR_WIFI_NAME";       // Enter your Wi-Fi Name
const char* password = "YOUR_WIFI_PASSWORD"; // Enter your Wi-Fi Password

// 2. MQTT Broker Settings
// We use a free public broker for testing. 
// If using Docker on your laptop, put your Laptop's IP Address here (e.g., "192.168.1.15")
const char* mqtt_server = "broker.hivemq.com"; 

// 3. Define Topics (Must match your Node-RED settings)
const char* topic_control = "robot/control"; // Listen for commands here
const char* topic_status = "robot/status";   // Send replies here
// --- USER CONFIGURATION END ---

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Start Serial at 9600 to match the Arduino's speed
  Serial.begin(9600);
  
  // Connect to Wi-Fi
  setup_wifi();
  
  // Setup MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

// --- Function to Connect to Wi-Fi ---
void setup_wifi() {
  delay(10);
  // We print to Serial for debugging, but this also goes to Arduino!
  // Arduino will just ignore lines that don't match "FORWARD", "STOP", etc.
  Serial.println(); 
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    // Do not print "." here as it might confuse Arduino
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// --- Function that runs when a message arrives from Node-RED ---
void callback(char* topic, byte* payload, unsigned int length) {
  // Convert the incoming payload (bytes) to a String
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  // Print the message to Serial 
  // This transmits it via the TX pin directly to the Arduino!
  Serial.println(message);
  
  // Optional: Send a confirmation back to the Dashboard
  // client.publish(topic_status, "Command Received");
}

// --- Function to Reconnect to MQTT if connection drops ---
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect (Client ID must be unique)
    String clientId = "NodeMCU-Robot-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      // Once connected, publish an announcement...
      client.publish(topic_status, "Robot Online");
      // ...and resubscribe to the control topic
      client.subscribe(topic_control);
    } else {
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}