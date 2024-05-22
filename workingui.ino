//#include <ESP8266WiFi.h>
//#include <PubSubClient.h>
//#include <time.h>
//#include <TZ.h>
//#include <FS.h>
//#include <LittleFS.h>
//#include <CertStoreBearSSL.h>
//
//// Update these with values suitable for your network.
//const char* ssid = "185";  // SSID
//const char* password = "d9127b31ae";  // Password
//const char* mqtt_server = "b29ed685441749cd86432c8680b3fb1a.s2.eu.hivemq.cloud";
//
//// Random array of values
//float bellCurveValues[30] = {
//  0.004, 0.008, 0.014, 0.024, 0.039, 0.061, 0.092, 0.135, 0.192, 0.264,
//  0.353, 0.455, 0.566, 0.682, 0.796, 0.896, 0.974, 1.020, 1.030, 1.000,
//  0.932, 0.835, 0.716, 0.586, 0.454, 0.331, 0.225, 0.141, 0.078, 0.039
//};
//
//int currentIndex = 0;
//unsigned long lastPublishTime = 0;
//const unsigned long publishInterval = 3000; // 3 seconds
//
//BearSSL::CertStore certStore;
//WiFiClientSecure espClient;
//PubSubClient * client;
//#define MSG_BUFFER_SIZE (500)
//char msg[MSG_BUFFER_SIZE];
//bool start_experiment = false;
//unsigned long startTime = 0;
//
//void setup_wifi() {
//  delay(10);
//  Serial.println();
//  Serial.print("Connecting to WiFi...");
//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//
//  randomSeed(micros());
//  Serial.println("WiFi connected");
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
//}
//
//void setDateTime() {
//  configTime(TZ_Europe_Berlin, "pool.ntp.org", "time.nist.gov");
//  Serial.print("Waiting for NTP time sync: ");
//  time_t now = time(nullptr);
//  while (now < 8 * 3600 * 2) {
//    delay(100);
//    Serial.print(".");
//    now = time(nullptr);
//  }
//  Serial.println();
//  struct tm timeinfo;
//  gmtime_r(&now, &timeinfo);
//  Serial.printf("%s %s", tzname[0], asctime(&timeinfo));
//}
//
//void callback(char* topic, byte* payload, unsigned int length) {
//  Serial.print("Message arrived [");
//  Serial.print(topic);
//  Serial.print("] ");
//  for (int i = 0; i < length; i++) {
//    Serial.print((char)payload[i]);
//  }
//  Serial.println();
//
//  if (strcmp(topic, "controlTopic") == 0) {
//    if (strncmp((char*)payload, "start", length) == 0) {
//      start_experiment = true;
//      startTime = millis();
//      currentIndex = 0;
//      lastPublishTime = millis();
//      Serial.println("Started experiment");
//    }
//  }
//}
//
//void reconnect() {
//  while (!client->connected()) {
//    Serial.print("Attempting MQTT connection...");
//    String clientId = "ESP8266Client - MyClient";
//    if (client->connect(clientId.c_str(), "vadrev", "Qwerty@123")) {
//      Serial.println("connected");
//      client->subscribe("controlTopic");  // Subscribe to controlTopic
//    } else {
//      Serial.print("failed, rc=");
//      Serial.print(client->state());
//      Serial.println(" try again in 5 seconds");
//      delay(5000);
//    }
//  }
//}
//
//void setup() {
//  Serial.begin(9600);
//  LittleFS.begin();
//  setup_wifi();
//  setDateTime();
//  pinMode(LED_BUILTIN, OUTPUT);
//
// 
//
//  int numCerts = certStore.initCertStore(LittleFS, PSTR("/certs.idx"), PSTR("/certs.ar"));
//  Serial.printf("Number of CA certs read: %d\n", numCerts);
//  if (numCerts == 0) {
//    Serial.printf("No certs found. Did you run certs-from-mozilla.py and upload the LittleFS directory before running?\n");
//    return;
//  }
//
//  BearSSL::WiFiClientSecure *bear = new BearSSL::WiFiClientSecure();
//  bear->setCertStore(&certStore);
//
//  client = new PubSubClient(*bear);
//
//  client->setServer(mqtt_server, 8883);
//  client->setCallback(callback);
//}
//
//void loop() {
//  if (!client->connected()) {
//    reconnect();
//  }
//  client->loop();
//
//  if (client->connected() && start_experiment) {
//    
//    digitalWrite(LED_BUILTIN, HIGH);
//    unsigned long now = millis();
//    if (now - lastPublishTime >= publishInterval) {
//      snprintf(msg, MSG_BUFFER_SIZE, "Value: %f", bellCurveValues[currentIndex]);
//      Serial.print("Publishing value: ");
//      Serial.println(msg);
//      client->publish("experiment", msg);
//      digitalWrite(LED_BUILTIN, LOW);
//
//      currentIndex++;
//      if (currentIndex >= 30) {
//        digitalWrite(LED_BUILTIN, HIGH);
//        currentIndex = 0; // Reset index or stop experiment based on your requirement
//        start_experiment = false;
//        Serial.println("Experiment finished");
//      }
//      lastPublishTime = now;
//    }
//  }
//}


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include <TZ.h>
#include <FS.h>
#include <LittleFS.h>
#include <CertStoreBearSSL.h>

// Update these with values suitable for your network.
const char* ssid = "185";  // SSID
const char* password = "d9127b31ae";  // Password
const char* mqtt_server = "b29ed685441749cd86432c8680b3fb1a.s2.eu.hivemq.cloud";

// Random array of values
float bellCurveValues[30] = {
  0.004, 0.008, 0.014, 0.024, 0.039, 0.061, 0.092, 0.135, 0.192, 0.264,
  0.353, 0.455, 0.566, 0.682, 0.796, 0.896, 0.974, 1.020, 1.030, 1.000,
  0.932, 0.835, 0.716, 0.586, 0.454, 0.331, 0.225, 0.141, 0.078, 0.039
};

int currentIndex = 0;
unsigned long lastPublishTime = 0;
const unsigned long publishInterval = 3000; // 3 seconds

BearSSL::CertStore certStore;
WiFiClientSecure espClient;
PubSubClient *client;
#define MSG_BUFFER_SIZE (500)
char msg[MSG_BUFFER_SIZE];
bool start_experiment = false;
unsigned long startTime = 0;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setDateTime() {
  configTime(TZ_Europe_Berlin, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(100);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println();
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.printf("%s %s", tzname[0], asctime(&timeinfo));
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (strcmp(topic, "controlTopic") == 0) {
    if (strncmp((char*)payload, "start", length) == 0) {
      start_experiment = true;
      startTime = millis();
      currentIndex = 0;
      lastPublishTime = millis();
      Serial.println("Started experiment");
    }
  }
}

void reconnect() {
  while (!client->connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client - MyClient";
    if (client->connect(clientId.c_str(), "vadrev", "Qwerty@123")) {
      Serial.println("connected");
      client->subscribe("controlTopic");  // Subscribe to controlTopic
    } else {
      Serial.print("failed, rc=");
      Serial.print(client->state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  LittleFS.begin();
  setup_wifi();
  setDateTime();
  pinMode(LED_BUILTIN, OUTPUT);

  int numCerts = certStore.initCertStore(LittleFS, PSTR("/certs.idx"), PSTR("/certs.ar"));
  Serial.printf("Number of CA certs read: %d\n", numCerts);
  if (numCerts == 0) {
    Serial.printf("No certs found. Did you run certs-from-mozilla.py and upload the LittleFS directory before running?\n");
    return;
  }

  BearSSL::WiFiClientSecure *bear = new BearSSL::WiFiClientSecure();
  bear->setCertStore(&certStore);

  client = new PubSubClient(*bear);

  client->setServer(mqtt_server, 8883);
  client->setCallback(callback);
}

void loop() {
  if (!client->connected()) {
    reconnect();
  }
  client->loop();

  if (client->connected() && start_experiment) {
    unsigned long now = millis();
    if (now - lastPublishTime >= publishInterval) {
      snprintf(msg, MSG_BUFFER_SIZE, "Value: %f", bellCurveValues[currentIndex]);
      Serial.print("Publishing value: ");
      Serial.println(msg);
      client->publish("experiment", msg);

      digitalWrite(LED_BUILTIN, LOW); // Turn on the LED
      delay(100); // Keep the LED on for 100 milliseconds
      digitalWrite(LED_BUILTIN, HIGH); // Turn off the LED

      currentIndex++;
      if (currentIndex >= 30) {
        currentIndex = 0; // Reset index or stop experiment based on your requirement
        start_experiment = false;
        Serial.println("Experiment finished");
      }
      lastPublishTime = now;
    }
  }
}
