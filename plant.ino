#include <WiFi.h>
#include <String>
#include <PubSubClient.h>

int _moisture,sensor_analog;
const int sensor_pin = A0;

const char* ssid = "IQ WiFi 8004DC";
const char* password = "1912DBC000415";


bool eventToBeReported = false;
const char* mqtt_server = "broker.hivemq.com";

/**************TOPICS*************/
const String user = "jsharmy1/";
const String plant_name = "bunny/";
const String topic_moisture = "moisture";
const String topic_eventToBeReported = "eventToBeReported";
/**************TOPICS*************/

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[1024];
int value = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  if (String(topic) == (user + plant_name + topic_eventToBeReported)) {
    if(messageTemp == "true"){
           eventToBeReported = true;
    }
    else if(messageTemp == "false"){
          eventToBeReported = false;
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
     String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe((user + plant_name + topic_eventToBeReported).c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  sensor_analog = analogRead(sensor_pin);
  _moisture = ( 100 - ( (sensor_analog/4095.00) * 100 ) );
  Serial.println(_moisture);

  if(eventToBeReported) {
  char msg[1024]={0};
  itoa(_moisture ,msg,10);
  client.publish((user + plant_name + topic_moisture).c_str(),msg,true);
  }
  delay(2000);
 
}
