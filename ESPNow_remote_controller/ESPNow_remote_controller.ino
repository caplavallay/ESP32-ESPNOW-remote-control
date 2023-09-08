// Include Libraries
#include <esp_now.h>
#include <WiFi.h>

// define the limits of sent servo angle data
#define SERVOMIN 155
#define SERVOMAX 459
 
// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0x08, 0xB6, 0x1F, 0x3D, 0x19, 0x14};
 
// Define a data structure
typedef struct struct_message {
  int serv0;
  int serv1;
  int serv2;
  int serv3;
  } struct_message;
 
// Create a structured object
struct_message myData;
 
// Peer info
esp_now_peer_info_t peerInfo;
 
// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status == ESP_NOW_SEND_SUCCESS) {
    analogWrite(17, 100);
    analogWrite(16, 0);
  }else{
    analogWrite(16, 100);
    analogWrite(17, 0);
  }

}
 
void setup() {
  
  // Set up Serial Monitor
  Serial.begin(115200);
 
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set analog read resolution
  analogReadResolution(8);
 
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
 
  // Create test data
 
  // read analog input
  int elevator = map(analogRead(36), 0, 255, SERVOMAX, SERVOMIN); // assigned to pins on control hats
  int ailerons = map(analogRead(39), 0, 255, SERVOMAX, SERVOMIN);
  int throttle = map(analogRead(32), 0, 255, 105, 459);
  int rudder = map(analogRead(33), 0, 255, SERVOMIN+30, SERVOMAX+30);
  
  // Format structured data
  myData.serv0 = elevator;
  myData.serv1 = ailerons;
  myData.serv2 = throttle;
  myData.serv3 = rudder;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
    Serial.printf("Elevator position = %d\n",elevator);
    Serial.printf("Aileron position = %d\n",ailerons);
    Serial.printf("Throttle position = %d\n",throttle);
    Serial.printf("Rudder position = %d\n",rudder);
  }
  else {
    Serial.println("Sending error");
  }
  delay(100);
}
