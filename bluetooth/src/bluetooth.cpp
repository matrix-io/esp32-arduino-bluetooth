#include <Arduino.h>  // This must be included first!

#include "MATRIXVoiceOTA.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "wishbone_bus.h"
#include "everloop.h"
#include "everloop_image.h"

namespace hal = matrix_hal;

// MATRIX LED Vars
static hal::WishboneBus wb;
static hal::Everloop everloop;
static hal::EverloopImage image;

//Define custom bluetooth service ID
//Randomly Generate UUID (Universal Unique ID) at https://www.uuidgenerator.net/
#define LED_CONTROL_SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define COLOR_VALUE_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

//Declare empty bluetooth variable
BLEServer *My_ESP32 = NULL;
BLEService *Led_Control = NULL;
BLECharacteristic *Led_Color = NULL;

void setup()
{
  //begin console printing
  Serial.begin(115200);
  
  //Setup the esp32 as the Bluetooth Server
  Serial.println("Starting BLE work!");
  BLEDevice::init("My_ESP32_Voice");
  My_ESP32 = BLEDevice::createServer();
  
  //create a service for controling the LEDs
  Led_Control = My_ESP32->createService(LED_CONTROL_SERVICE_UUID);
  
  //create a readable/writeable characteristic (i.e. bluetooth variable)
  Led_Color = Led_Control->createCharacteristic(COLOR_VALUE_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  
  // Setup the Everloop
  wb.Init();
  everloop.Setup(&wb);
  
  //set the initial value to blue and start the service
  Led_Color->setValue("blue");
  Led_Control->start();
  
  //start advertising the Voice's Service to other bluetooth devices
  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(LED_CONTROL_SERVICE_UUID);
  advertising->setScanResponse(true);
  advertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  advertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  Serial.println("Color defined! Now you can read/change it in your phone!");
}
void loop()
{
  try
  {
      //convert the string sent to a number if possible
      int value = 0, r = 0, b = 0, g = 0;
      sscanf(Led_Color->getValue().c_str(), "%d", &value);
      if(Led_Color->getValue() == "blue"){
        b = 255;
      }
      else if(Led_Color->getValue() == "red"){
        r = 255;
      }
      else if(Led_Color->getValue() == "green"){
        g = 255;
      }
      else if(value <= 255 && value >= 0){
        r = b = g = value;
      }
      for (hal::LedValue &led : image.leds)
      {
          led.red = r;
          led.green = g;
          led.blue = b;
          led.white = 0;
      }
      everloop.Write(&image);
  }
  
  //catch all exceptions
  catch (...)
  {
    Serial.println("Errors happening");
  }
  delay(2000);
}
