#ifndef CONFIG_H
#define CONFIG_H

// WiFi
#define WIFI_SSID "FamiliaMuri"
#define WIFI_PASSWORD "F4m1l14Mur!"

// WebSocket
#define WS_HOST "192.168.1.173"
#define WS_PORT 8765

// BLE
#define BLE_NAME "ESP32-SNAKE"
#define SERVICE_UUID "fc96f65e-318a-4001-84bd-77e9d12af44b"
#define CHARACTERISTIC_UUID_RX "04d3552e-b9b3-4be6-a8b4-aa43c4507c4d"

// Hardware
#define BUZZER_PIN 25

// Juego
#define GAME_WIDTH 20
#define GAME_HEIGHT 15
#define GAME_SPEED 150
#define MAX_SNAKE_LENGTH 100

// Música
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_REST 0

#endif
