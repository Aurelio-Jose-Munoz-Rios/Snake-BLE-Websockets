#ifndef BLE_CONTROLLER_H
#define BLE_CONTROLLER_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "Config.h"
#include "Point.h"

class BLEController {
private:
  BLEServer* server;
  BLECharacteristic* rxCharacteristic;
  volatile Direction lastCommand;
  
  static BLEController* instance;
  
  class CommandCallback : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* characteristic) {
      uint8_t* data = characteristic->getData();
      if (characteristic->getLength() > 0) {
        BLEController::instance->setCommand((Direction)data[0]);
      }
    }
  };

public:
  BLEController() : server(nullptr), rxCharacteristic(nullptr), lastCommand(DIR_NONE) {
    instance = this;
  }
  
  void begin() {
    BLEDevice::init(BLE_NAME);
    server = BLEDevice::createServer();
    
    BLEService* service = server->createService(SERVICE_UUID);
    rxCharacteristic = service->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE
    );
    
    rxCharacteristic->addDescriptor(new BLE2902());
    rxCharacteristic->setCallbacks(new CommandCallback());
    
    service->start();
    BLEDevice::startAdvertising();
    
    Serial.println("BLE Controller iniciado");
  }
  
  Direction getCommand() {
    Direction cmd = lastCommand;
    lastCommand = DIR_NONE;
    return cmd;
  }
  
  void setCommand(Direction cmd) {
    lastCommand = cmd;
  }
  
  bool hasCommand() {
    return lastCommand != DIR_NONE;
  }
};

BLEController* BLEController::instance = nullptr;

#endif
