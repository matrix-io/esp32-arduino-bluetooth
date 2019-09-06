# ESP32 Bluetooth Mesh Example Arduino Version

![ESP32 MATRIX Voice Standalone](https://github.com/matrix-io/esp32-arduino-ota/blob/master/esp32-2.gif)

## Required Hardware
Before you get started, let's review what you'll need.

- Raspberry Pi 3 (Recommended) or Pi 3 Model B+ (Supported).
- MATRIX Voice ESP32 version - Buy the MATRIX Voice.
- Micro-USB power adapter for Raspberry Pi
- Micro-SD Card (Minimum 8 GB)
- Micro-USB Cable
- A PersonalComputer to SSH into your Raspberry Pi
- Internet connection (Ethernet or WiFi)

## Let's Get Started

The MATRIX Voice ESP32 version has an ESP-WROOM-32. This tiny module allows for standalone applications with WiFi and Bluetooth. We require the Raspberry Pi for the initial setup. 
The environment we're setting up in this guide will end with our PC serving as the development environment for our apps. The apps will then be sent to the Raspberry Pi which will deploy them to the MATRIX Voice for standalone deployment. 

[Here](https://www.hackster.io/matrix-labs/program-over-the-air-on-esp32-matrix-voice-5e76bb) is a guide that goes over installing the ESP32 development environment on your PC, and how to setup the Arduino IDE to support ESP32 development over WiFi.

This guide goes over how to setup two Matrix Voices to communicate via Bluetooth Low Energy, both acting as a server and client.

## Compiling The Code
After having setup your Raspberry Pi and MATRIX Voice, download the esp32-arduino-bluetooth GitHub repository to your PC.
```
git clone https://github.com/matrix-io/esp32-arduino-bluetooth.git   
```

Checkout the arduino branch after entering into the folder.
```
cd esp32-arduino-bluetooth
git checkout arduino
```

Open the esp32-arduino-bluetooth/bluetooth_server/bluetooth_server.ino file in the Arduino IDE. 

Click on Sketch -> Export Compiled Binary. This will create a binary (.bin) file from the arduino code which will then be flashed to the MATRIX Voice through the Raspberry Pi. After following the steps below and uploading the code to one of your  Matrix Voices.

## Deploy the Sketch to the ESP32

Connect your voice to the Raspberry Pi. Go on your Pi's terminal and type the following commands to erase any previous programs. This step must be performed on every re-flashing of the ESP32.
```
voice_esp32_enableesptool.py --chip esp32 --port /dev/ttyS0 --baud 115200 --before default_reset --after hard_reset erase_flashsudo reboot  
```
Go on your PC's terminal and navigate to esp32-arduino-bluetooth/bluetooth. 
```
cd esp32-arduino-bluetooth/bluetooth_server  
```
Once in the directory, edit the deploy_ota.sh file and change the IP to your Pi's IP address in the place of YOUR_PI_IP_HERE. Then run the command below.
```
sh deploy_ota.sh  
```
You will be prompted to enter your Pi's password. Then, this will flash the MATRIX Voice with the current program and you're ready to move on.

Repeat the steps above with the second matrix voice but instead go into the `bluetooth_client/bluetooth_client.ino` file. Compile the sketch to binary as before, hard reset the Voice, and run ```esp32-arduino-bluetooth/bluetooth_client/deploy_ota.sh``` to deploy the client side code.

## Connecting The Devices To Each Other
This program is meant to have two MATRIX Voices communicate with each other through bluetooth low energy to change their leds' colors.

The characteristic created by one of our MATRIX Voices allows the other Voice to write to it, so by powering both at the same time, the Voice with the client side code will change the value held in the characteristic of the Voice with the server side code. The server Voice will switch its leds between blue and red.

You're all set! Feel free to play around with the example and/or hook up more devices to the system. Alternatively, you could also combine the server and client side code into one program so that the communication is bidirectional.
