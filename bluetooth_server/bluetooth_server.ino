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
#define LED_CONTROL_SERVICE_UUID "1feed041-ff7b-4aee-a9af-be4944e100f0"
#define COLOR_VALUE_UUID "1114f0e7-7f16-4660-9cdb-1557cfcdef3c"
//Declare empty bluetooth variable
BLEServer *My_ESP32 = NULL;
BLEService *Led_Control = NULL;
BLECharacteristic *Led_Color = NULL;
class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string value = pCharacteristic->getValue();
        if (value == "blue")
        {
            for (hal::LedValue &led : image.leds)
            {
                led.red = 0;
                led.green = 0;
                led.blue = 20;
                led.white = 0;
            }
            everloop.Write(&image);
        }
        else if (value == "red")
        {
            for (hal::LedValue &led : image.leds)
            {
                led.red = 20;
                led.green = 0;
                led.blue = 0;
                led.white = 0;
            }
            everloop.Write(&image);
        }
    }
};
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
    Led_Color->setCallbacks(new MyCallbacks());
    // Setup the Everloop
    wb.Init();
    everloop.Setup(&wb);
    //set the initial value to blue and start the service
    Led_Color->setValue("red");
    Led_Control->start();
    //start advertising the Voice's Service to other bluetooth devices
    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(LED_CONTROL_SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    advertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}
void loop()
{
    delay(2000);
}
