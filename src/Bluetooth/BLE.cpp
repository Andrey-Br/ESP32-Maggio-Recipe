#include "BLE.h"


#ifdef _ANDROID_CHEESE_USE_BLE_


bool bluetothConected = false;
BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pChr_INPUT , *pChr_OUTPUT, *pChr_PID ; 

//CallBack сервера
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    bluetothConected = true;
    
  };
  
  void onDisconnect(BLEServer* pServer) override {
    bluetothConected = false;
    pServer->getAdvertising()->start();
  };

};

//CallBack Входных данных
class MyCallbacks_INPUT: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std :: string rxValue = pCharacteristic->getValue();
    String msg = rxValue.c_str();
    decode(msg);   
  }
};


// Инициализация Bluetoth
void bluetothInit(){
  BLEDevice::init("Maggio Cheese");

  pServer = BLEDevice::createServer();
  
  // создаем BLE-сервер:
  // *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Создаем BLE-сервис:
  pService = pServer->createService(UUID_SERVICE);
  

  // Создаем BLE-характеристики:
  pChr_INPUT = pService->createCharacteristic(
    UUID_pChr_INPUT,
    BLECharacteristic::PROPERTY_WRITE);

  pChr_OUTPUT = pService->createCharacteristic(
    UUID_pChr_OUTPUT,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_INDICATE);

  pChr_PID = pService->createCharacteristic(
    UUID_pChr_PID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY |
    BLECharacteristic::PROPERTY_INDICATE);
  
  pChr_INPUT->addDescriptor(new BLE2902());
  pChr_OUTPUT->addDescriptor(new BLE2902());
  pChr_PID->addDescriptor(new BLE2902());


  pChr_INPUT->setCallbacks(new MyCallbacks_INPUT());

  // Запускаем сервис:
  pService->start();

  // Начинаем рассылку оповещений:
  pServer->getAdvertising()->start();
};

// Обработка Bluetoth
void bluetothTick() {
};

// Отправить по Bluetoth
void bluetothSend(String s){
  
  pChr_OUTPUT->setValue(s.c_str());
  pChr_OUTPUT->notify();
  pChr_OUTPUT->indicate();
};


#endif

