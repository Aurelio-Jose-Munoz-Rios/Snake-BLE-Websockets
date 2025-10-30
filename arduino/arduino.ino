#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

// Configuración WiFi y WebSocket
const char* ssid = "FamiliaMuri";
const char* password = "F4m1l14Mur!";
const char* websocket_server = "192.168.1.173"; // IP de tu PC
const uint16_t websocket_port = 8765;

WebSocketsClient webSocket;

// BLE UUIDs
#define BLE_NAME "ESP32-CONTROL"
#define SERVICE_UUID "fc96f65e-318a-4001-84bd-77e9d12af44b"
#define CHARACTERISTIC_UUID_RX "04d3552e-b9b3-4be6-a8b4-aa43c4507c4d"

BLECharacteristic* pCharacteristicRX = NULL;

// Variable compartida entre núcleos (protegida)
volatile int lastCommand = 0;
SemaphoreHandle_t commandMutex;

// Callback BLE
class RXCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    uint8_t* data = pCharacteristic->getData();
    int size = pCharacteristic->getLength();
    
    if (size >= 1) {
      xSemaphoreTake(commandMutex, portMAX_DELAY);
      lastCommand = data[0];
      xSemaphoreGive(commandMutex);
      
      Serial.printf("BLE recibido: %d\n", data[0]);
    }
  }
};

// Tarea para BLE (Core 0)
void taskBLE(void *pvParameters) {
  BLEDevice::init(BLE_NAME);
  BLEServer* pServer = BLEDevice::createServer();
  BLEService* pService = pServer->createService(SERVICE_UUID);
  
  pCharacteristicRX = pService->createCharacteristic(
    CHARACTERISTIC_UUID_RX,
    BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristicRX->addDescriptor(new BLE2902());
  pCharacteristicRX->setCallbacks(new RXCallback());
  
  pService->start();
  BLEDevice::startAdvertising();
  Serial.println("BLE iniciado en Core 0");
  
  // Mantener tarea viva
  while(true) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Tarea para WebSocket (Core 1)
void taskWebSocket(void *pvParameters) {
  // Conectar WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
  
  // Iniciar WebSocket
  webSocket.begin(websocket_server, websocket_port, "/");
  Serial.println("WebSocket iniciado en Core 1");
  
  while(true) {
    webSocket.loop();
    
    // Enviar comando si hay uno nuevo
    if (lastCommand != 0) {
      xSemaphoreTake(commandMutex, portMAX_DELAY);
      int cmd = lastCommand;
      lastCommand = 0;
      xSemaphoreGive(commandMutex);
      
      String mensaje = String(cmd);
      webSocket.sendTXT(mensaje);
      Serial.printf("WebSocket enviado: %s\n", mensaje.c_str());
    }
    
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  
  // Crear mutex para proteger variable compartida
  commandMutex = xSemaphoreCreateMutex();
  
  // Crear tarea BLE en Core 0
  xTaskCreatePinnedToCore(
    taskBLE,      // Función
    "BLE_Task",   // Nombre
    10000,        // Stack size
    NULL,         // Parámetros
    1,            // Prioridad
    NULL,         // Handle
    0             // Core 0
  );
  
  // Crear tarea WebSocket en Core 1
  xTaskCreatePinnedToCore(
    taskWebSocket,
    "WebSocket_Task",
    10000,
    NULL,
    1,
    NULL,
    1             // Core 1
  );
}

void loop() {
  // Loop vacío, todo se maneja en las tareas
}
