# ESP32-ESPNOW-remote-control

*******************Abstract*********************
This project uses the ESPNOW protocol to act as a remote control system.
The transmitter uses the built-in ADC on the ESP32 to read the input from a set of potentiometers and transmits that data to the reciever module using ESPNOW.
The reciever uses the packets sent from the transmitter to control the output of an Adafruit PCA9685 PWM servo driver.

