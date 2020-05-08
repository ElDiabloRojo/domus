#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

Servo servo;

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* mqttServer = "MQTT_IP_ADDRESS";
const int mqttPort = 1883;

const char* message;

const int IN1 = 0;
const int IN2 = 2;
const int ENA = 4;
const int IN3 = 12;
const int IN4 = 13;
const int ENB = 14;

WiFiClient espClient;
PubSubClient client(espClient);
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    readmessage((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}

void readmessage(char message){
  switch(message){
    case 'O': 
      Serial.println(message);
      outtake(); 
    break;
    case 'I': 
      Serial.println(message);
      intake(); 
    break;
    case 'D':
      Serial.println(message);
      disable();
    break;
    case 'S':
      Serial.println(message);
      open_window();
    break;
    case 'C':
      Serial.println(message);
      close_window();
    break;
    default: 
      Serial.println("no message received"); 
    break;
  }
}

void close_window() {
  servo.write(0);
}

void open_window(){
  servo.write(180);
}

void outtake() {
  analogWrite(ENA, 1023); 
  analogWrite(ENB, 1023); 
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void intake() {
  analogWrite(ENA, 1023); 
  analogWrite(ENB, 1023); 
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void disable() {
  analogWrite(ENA, 0); 
  analogWrite(ENB, 0); 
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setup() {

  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENB, OUTPUT);

  servo.attach(D1);
  servo.write(0);
 
  Serial.begin(9600);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client")) {
 
      Serial.println("connected");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.subscribe("topic/fans");
}
 
void loop() {
  client.loop();
}
