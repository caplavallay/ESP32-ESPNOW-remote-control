// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// declare the adafruit servo driver as myservo
Adafruit_PWMServoDriver myServo = Adafruit_PWMServoDriver();

// Define a data structure
typedef struct struct_message {
  int serv0;
  int serv1;
  int serv2;
  int serv3;
} struct_message;

// Create a structured object
struct_message myData;


// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  // print to serial the recieved data
  Serial.println("Data received: ");
  Serial.print("servo 0 Value: ");
  Serial.println(myData.serv0);
  Serial.print("servo 1 Value: ");
  Serial.println(myData.serv1);
  Serial.print("servo 2 Value: ");
  Serial.println(myData.serv2);
  Serial.print("servo 3 Value: ");
  Serial.println(myData.serv3);

  // write the recieved data to each servo on the pca9685 then set to zero to conserve power
  myServo.setPWM(0, 0, myData.serv0);
  delay(20);
  myServo.setPWM(3, 0, 0);

  myServo.setPWM(1, 0, myData.serv1);
  delay(20);
  myServo.setPWM(0, 0, 0);

  myServo.setPWM(2, 0, myData.serv2);
  //delay(10);
  //myServo.setPWM(2, 0, 0);

  myServo.setPWM(3, 0, myData.serv3);
  delay(20);
  myServo.setPWM(1, 0, 0);

}

void setup() {

  // Set up Serial Monitor
  Serial.begin(115200);

  Serial.print("MAC Address: "); Serial.println(WiFi.macAddress());

  // set up servos according to the adafruit library
  myServo.begin();
  myServo.setPWMFreq(50);
  
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
