# 🐍 Snake Game ESP32 - Wireless Remote Control

<div align="center">

![ESP32](https://img.shields.io/badge/ESP32-000000?style=for-the-badge&logo=Espressif&logoColor=white)
![Flutter](https://img.shields.io/badge/Flutter-02569B?style=for-the-badge&logo=flutter&logoColor=white)
![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)

**Control remoto inalámbrico del clásico juego Snake usando BLE, WiFi y WebSocket**

[Características](#características) • [Hardware](#hardware) • [Instalación](#instalación) • [Uso](#uso) • [Arquitectura](#arquitectura)

</div>

---

## 📖 Descripción

Implementación moderna del clásico juego **Snake** con arquitectura IoT distribuida. El juego se controla desde un smartphone vía **Bluetooth Low Energy (BLE)**, la lógica del juego se ejecuta en un **ESP32**, y la visualización en tiempo real se renderiza en una **computadora usando Python/Pygame**, todo comunicado por **WebSocket**.

### 🎮 Características Principales

- ✨ **Control remoto BLE** - Interfaz D-Pad estilo PlayStation en Flutter
- 🎵 **Música arcade** - 3 melodías temáticas (Intro, Juego, Game Over)
- 🖥️ **Visualización en tiempo real** - Renderizado fluido a 30 FPS en Python
- ⚡ **Multitarea FreeRTOS** - Aprovecha los 2 núcleos del ESP32
- 🔧 **Arquitectura modular OOP** - Código limpio y mantenible
- 🌐 **Comunicación WebSocket** - Latencia ultra-baja (<150ms)
- 📱 **Multiplataforma** - Flutter soporta Android/iOS

---

## 🎯 Características Técnicas

### Sistema de Control
- **D-Pad circular** con diseño gaming profesional
- **Comunicación BLE** de un solo byte por comando
- **Sin latencia perceptible** (<50ms BLE)
- **Reconexión automática** en caso de pérdida de señal

### Motor de Juego
- **Tablero:** 20x15 celdas
- **Lógica Snake clásica:** crecimiento, colisiones, puntuación
- **Velocidad ajustable:** 150ms por frame (configurable)
- **Detección de colisiones:** paredes y auto-colisión

### Sistema de Audio
- **3 melodías temáticas:**
  - 🎵 Intro: Inspirada en Pac-Man (alegre)
  - 🎵 Juego: Basada en Tetris (acción)
  - 🎵 Game Over: Melodía descendente (triste)
- **Efecto de sonido** al comer comida
- **Reproducción asíncrona** en Core 0 del ESP32

### Arquitectura de Software
- **Programación Orientada a Objetos**
- **Patrón de diseño:** Separación de responsabilidades
- **Modular:** 7 archivos organizados por funcionalidad
- **FreeRTOS:** Gestión de tareas paralelas
- **Mutex/Semáforos:** Sincronización segura entre tareas

---

## 🛠️ Hardware Necesario

### Componentes Principales

| Componente | Especificación | Cantidad |
|------------|----------------|----------|
| **ESP32** | ESP32-WROOM-32 o ESP32-WROVER | 1 |
| **Buzzer** | Buzzer pasivo 5V | 1 |
| **Smartphone** | Android 6.0+ o iOS 10+ | 1 |
| **Computadora** | Windows/Linux/macOS | 1 |
| **Cable USB** | Micro-USB o USB-C (según ESP32) | 1 |

### Conexiones

