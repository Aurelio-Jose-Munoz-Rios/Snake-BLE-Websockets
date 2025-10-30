# 🐍 Snake Game ESP32 — Control BLE + Visualización WebSocket

Controla el clásico juego Snake desde tu teléfono (Flutter + BLE), ejecuta la lógica en un ESP32 y visualiza en tiempo real en tu PC con Python (WebSocket + Pygame). Arquitectura modular, OOP y multitarea con FreeRTOS, más música arcade en 3 estados.

---

## ✨ Características

- Control remoto desde Flutter por BLE con D-Pad estilo consola.
- Lógica del juego en ESP32 (C++), modular y orientada a objetos (clases BLEController, WebSocketManager, MusicPlayer, SnakeGame).
- Visualización en PC con Python y Pygame vía WebSocket.
- Música arcade: Intro (alegre), In-Game (persecución), Game Over (triste) con reproducción asíncrona.
- Multitarea con FreeRTOS y uso de los 2 núcleos del ESP32 (Core 0: BLE + música, Core 1: WiFi/WebSocket + juego).
- Baja latencia extremo a extremo (<150 ms) y arquitectura escalable/mantenible.

---

## 🧱 Arquitectura

Smartphone (Flutter BLE) → ESP32 (BLE + Juego + WiFi) → PC (Python WebSocket + Pygame)

- Core 0: BLE + MusicPlayer (FreeRTOS).
- Core 1: WiFi/WebSocket + Lógica de juego + Loop principal.

---

## 🛠️ Hardware

| Componente       | Detalle                                  | Cantidad |
|------------------|-------------------------------------------|----------|
| ESP32 DevKit     | ESP32-WROOM-32 o ESP32-WROVER             | 1        |
| Buzzer pasivo    | 5V, conectado a GPIO 25 (configurable)    | 1        |
| Smartphone       | Android 6+ o iOS 10+                      | 1        |
| PC               | Windows/Linux/macOS                       | 1        |
| Red WiFi         | 2.4 GHz (ESP32 no soporta 5 GHz)          | 1        |
| Cable USB        | Micro‑USB/USB‑C según placa               | 1        |

Conexión del buzzer:
ESP32 GPIO 25 → Buzzer (+)
ESP32 GND → Buzzer (−)

---

## 📦 Dependencias y Librerías

### Arduino/ESP32

- Core ESP32 (2.0.14 o superior)
  - URL Boards Manager: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
- Librerías:
  - WebSockets by Markus Sattler (2.4.0+)
  - BLE (incluida en el core ESP32)
  - WiFi (incluida en el core ESP32)

Ajustes recomendados en Arduino IDE:
- Board: ESP32 Dev Module
- CPU Freq: 240 MHz (WiFi/BT)
- Flash Mode: QIO, 80 MHz
- Flash Size: 4 MB
- Partition Scheme: Huge APP (3MB No OTA/1MB SPIFFS)
- Upload Speed: 921600

### Flutter (control BLE)

- SDK Flutter estable (3.x+)
- Dependencias en pubspec.yaml:
  dependencies:
    flutter:
      sdk: flutter
    flutter_blue_plus: ^1.14.0
- Permisos Android (AndroidManifest.xml):
  uses-permission BLUETOOTH, BLUETOOTH_ADMIN, BLUETOOTH_SCAN, BLUETOOTH_CONNECT, ACCESS_FINE_LOCATION
- Permisos iOS (Info.plist):
  NSBluetoothAlwaysUsageDescription = “Esta app usa Bluetooth para controlar el juego Snake”

### Python (visualización)

- Python 3.8+
- Librerías:
  pip install pygame==2.5.2
  pip install websockets==13.0
- Alternativa con requirements.txt:
  pygame>=2.5.2
  websockets>=13.0

---

## 📁 Estructura del repositorio

.
├── arduino/
│   └── SnakeGame/
│       ├── SnakeGame.ino
│       ├── Config.h
│       ├── Point.h
│       ├── BLEController.h
│       ├── WebSocketManager.h
│       ├── MusicPlayer.h
│       └── SnakeGame.h
├── flutter_app/
│   ├── lib/
│   │   ├── main.dart
│   │   ├── screens/
│   │   │   ├── scan_screen.dart
│   │   │   └── device_screen.dart
│   │   └── utils/
│   └── pubspec.yaml
├── python_server/
│   ├── snake_game.py
│   └── requirements.txt
├── docs/
│   ├── architecture.md
│   ├── API.md
│   └── images/
├── LICENSE
└── README.md

---

## ⚙️ Configuración rápida

1) ESP32 (Arduino)
- Edita arduino/SnakeGame/Config.h:
  #define WIFI_SSID "TuWiFi"
  #define WIFI_PASSWORD "TuPassword"
  #define WS_HOST "192.168.1.100"   // IP de tu PC
  #define WS_PORT 8765
  #define BUZZER_PIN 25
- Compila y sube el sketch.
- Selecciona partición “Huge APP (3MB No OTA/1MB SPIFFS)”.

2) Python (PC)
- En la carpeta python_server:
  pip install -r requirements.txt
  python snake_game.py
- Verifica en consola: “Servidor WebSocket corriendo en puerto 8765”.

3) Flutter (Smartphone)
- En la carpeta flutter_app:
  flutter pub get
  flutter run
- Conéctate al dispositivo BLE “ESP32-SNAKE” y usa el D‑Pad.

---

## 🕹️ Uso

- Al iniciar, la PC muestra “waiting”; en ESP32 suena la melodía de intro.
- Presiona cualquier dirección en el D‑Pad → el juego comienza.
- Durante el juego suena la melodía de persecución en loop.
- Comer comida suma +10 puntos y reproduce un “beep”.
- Colisión → melodía de game over, envío de puntaje y retorno a “waiting”.

Controles enviados por BLE (un byte):
1 = Arriba, 2 = Abajo, 3 = Izquierda, 4 = Derecha.

---

## 🔧 Configuraciones

- Velocidad del juego (ms por frame) en Config.h:
  #define GAME_SPEED 150
- Tamaño del tablero en Config.h:
  #define GAME_WIDTH 20
  #define GAME_HEIGHT 15
- Asegura en python_server/snake_game.py:
  WIDTH = 600, HEIGHT = 450, CELL_SIZE = 30  → Debe cumplirse WIDTH == GAME_WIDTH * CELL_SIZE.

---

## 🧪 Troubleshooting

- “Sketch too big”: usa partición “Huge APP (3MB No OTA/1MB SPIFFS)”.
- WebSocket no conecta:
  - Verifica IP de tu PC (Windows: ipconfig).
  - Asegura misma red WiFi para ESP32 y PC.
  - Permite el puerto TCP 8765 en el firewall.
- Flutter no detecta el ESP32:
  - Revisa permisos Bluetooth y ubicación.
  - Reinicia Bluetooth del teléfono.
- Sonido distorsionado:
  - Usa buzzer pasivo y GND correcto.
  - Sube prioridad de la tarea de música si es necesario.

---

## 📈 Rendimiento típico

| Métrica                 | Valor     |
|-------------------------|-----------|
| Latencia BLE            | 20–50 ms  |
| Latencia WiFi/WebSocket | 20–50 ms  |
| FPS (Pygame)            | 30 FPS    |
| RAM usada (ESP32)       | ~200 KB   |
| Flash usada (ESP32)     | ~1.2 MB   |

---

## 🗺️ Roadmap

- [ ] Modo multijugador (2 serpientes).
- [ ] Obstáculos dinámicos.
- [ ] Power‑ups y niveles.
- [ ] Ranking local/online.
- [ ] Skins/temas visuales.
- [ ] Integración con pantallas OLED/ILI9341.

---

## 🤝 Contribuir

1. Fork del repo.
2. Crea rama: feature/mi‑mejora.
3. Commit: feat: agrega X.
4. Pull Request con descripción clara.

Estándares: código modular y OOP, documentar funciones públicas, actualizar README si afecta al usuario final.

---

## 📄 Licencia

MIT. Ver LICENSE.

---

## 👤 Autor

GitHub: @tu-usuario  
Email: tu-email@ejemplo.com
