#include "Config.h"
#include "Point.h"
#include "BLEController.h"
#include "WebSocketManager.h"
#include "MusicPlayer.h"
#include "SnakeGame.h"

// Objetos principales
BLEController bleController;
WebSocketManager wsManager;
MusicPlayer musicPlayer(BUZZER_PIN);
SnakeGame game;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  
  Serial.println("\n=== Snake Game ESP32 ===");
  
  // Inicializar componentes
  musicPlayer.begin();
  bleController.begin();
  wsManager.begin();
  game.init();
  
  // Crear tarea de música en Core 0
  xTaskCreatePinnedToCore(
    MusicPlayer::taskFunction,
    "Music_Task",
    4096,
    &musicPlayer,
    1,
    NULL,
    0
  );
  
  // Configurar música inicial
  musicPlayer.setTrack(TRACK_INTRO);
  
  // Enviar estado inicial
  delay(1000);
  wsManager.sendWaiting();
  
  Serial.println("Sistema iniciado. Esperando comandos...");
}

void loop() {
  wsManager.loop();
  
  // Procesar comandos BLE
  Direction cmd = bleController.getCommand();
  if (cmd != DIR_NONE) {
    if (!game.hasStarted()) {
      game.start();
      musicPlayer.setTrack(TRACK_GAME);
      Serial.println("¡Juego iniciado!");
    }
    game.setDirection(cmd);
  }
  
  // Actualizar juego
  if (game.hasStarted() && !game.hasEnded()) {
    bool ate = game.update();
    
    if (ate) {
      musicPlayer.playEffect(NOTE_C6, 50);
    }
    
    wsManager.sendJSON(game.toJSON());
    
    if (game.hasEnded()) {
      Serial.println("Game Over! Score: " + String(game.getScore()));
      musicPlayer.setTrack(TRACK_GAMEOVER);
      wsManager.sendGameOver(game.getScore());
      delay(3000);
      
      // Reiniciar
      game.init();
      musicPlayer.setTrack(TRACK_INTRO);
      wsManager.sendWaiting();
      Serial.println("Sistema reiniciado");
    }
    
    delay(GAME_SPEED);
  } else {
    delay(100);
  }
}
