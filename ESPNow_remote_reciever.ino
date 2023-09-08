/*
  ESP-NOW Demo - Receive
  esp-now-demo-rcv.ino
  Reads data from Initiator
  
  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/

// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// Define a data structure
typedef struct struct_message {
  int serv1;
  int serv2;
  int serv3;
  int serv4;
} struct_message;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Create a structured object
struct_message myData;


// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println("Data received: ");
  Serial.print("servo 1 Value: ");
  Serial.println(myData.serv1);
  servo1.write(myData.serv1);
  Serial.print("servo 2 Value: ");
  Serial.println(myData.serv2);
  servo2.write(myData.serv2);
  Serial.print("servo 3 Value: ");
  Serial.println(myData.serv3);
  servo3.write(myData.serv3);
  Serial.print("servo 4 Value: ");
  Serial.println(myData.serv4);
  servo4.write(myData.serv4);
}

void setup() {

  // Set up Serial Monitor
  Serial.begin(115200);

  Serial.print("MAC Address: "); Serial.println(WiFi.macAddress());

	servo1.setPeriodHertz(50);    // standard 50 hz servo
	servo1.attach(17, 1000, 2000); // attaches the servo on pin 17 to the servo object  
  servo2.setPeriodHertz(50);    // standard 50 hz servo
	servo2.attach(25, 1000, 2000); // attaches the servo on pin 16 to the servo object 
  servo3.setPeriodHertz(50);    // standard 50 hz servo
	servo3.attach(32, 1000, 2000); // attaches the servo on pin 32 to the servo object  
  servo4.setPeriodHertz(50);    // standard 50 hz servo
	servo4.attach(33, 1000, 2000); // attaches the servo on pin 33 to the servo object 
  
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {}