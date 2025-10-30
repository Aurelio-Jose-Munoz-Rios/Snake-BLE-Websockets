#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include "Config.h"

enum MusicTrack {
  TRACK_INTRO,
  TRACK_GAME,
  TRACK_GAMEOVER,
  TRACK_NONE
};

class MusicPlayer {
private:
  int buzzerPin;
  MusicTrack currentTrack;
  int currentNote;
  unsigned long lastNoteTime;
  SemaphoreHandle_t mutex;
  
  // Melodía Intro (Pac-Man)
  static const int introNotes[];
  static const int introDurations[];
  static const int introLength;
  
  // Melodía Juego (Tetris)
  static const int gameNotes[];
  static const int gameDurations[];
  static const int gameLength;
  
  // Melodía Game Over
  static const int gameoverNotes[];
  static const int gameoverDurations[];
  static const int gameoverLength;

public:
  MusicPlayer(int pin) : buzzerPin(pin), currentTrack(TRACK_NONE), 
                         currentNote(0), lastNoteTime(0) {
    mutex = xSemaphoreCreateMutex();
  }
  
  void begin() {
    pinMode(buzzerPin, OUTPUT);
  }
  
  void setTrack(MusicTrack track) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    if (track != currentTrack) {
      currentTrack = track;
      currentNote = 0;
      noTone(buzzerPin);
    }
    xSemaphoreGive(mutex);
  }
  
  void playEffect(int frequency, int duration) {
    tone(buzzerPin, frequency, duration);
  }
  
  void update() {
    xSemaphoreTake(mutex, portMAX_DELAY);
    MusicTrack track = currentTrack;
    xSemaphoreGive(mutex);
    
    unsigned long currentTime = millis();
    
    switch(track) {
      case TRACK_INTRO:
        playMelody(introNotes, introDurations, introLength, true);
        break;
      case TRACK_GAME:
        playMelody(gameNotes, gameDurations, gameLength, true);
        break;
      case TRACK_GAMEOVER:
        playMelody(gameoverNotes, gameoverDurations, gameoverLength, false);
        break;
      case TRACK_NONE:
        noTone(buzzerPin);
        break;
    }
  }
  
  static void taskFunction(void* param) {
    MusicPlayer* player = (MusicPlayer*)param;
    while(true) {
      player->update();
      vTaskDelay(30 / portTICK_PERIOD_MS);
    }
  }

private:
  void playMelody(const int* notes, const int* durations, int length, bool loop) {
    if (currentNote >= length) {
      if (loop) {
        currentNote = 0;
        delay(500);
      } else {
        noTone(buzzerPin);
        return;
      }
    }
    
    if (notes[currentNote] != NOTE_REST) {
      tone(buzzerPin, notes[currentNote]);
      delay(1000 / durations[currentNote]);
      noTone(buzzerPin);
    }
    currentNote++;
    delay(30);
  }
};

// Definición de melodías
const int MusicPlayer::introNotes[] = {
  NOTE_B4, NOTE_B5, NOTE_F5, NOTE_D5, NOTE_B5, NOTE_F5, NOTE_D5, NOTE_C5,
  NOTE_B4, NOTE_B5, NOTE_F5, NOTE_D5, NOTE_B5, NOTE_F5, NOTE_D5, NOTE_D5
};
const int MusicPlayer::introDurations[] = {
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};
const int MusicPlayer::introLength = 16;

const int MusicPlayer::gameNotes[] = {
  NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4,
  NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
  NOTE_C5, NOTE_A4, NOTE_A4
};
const int MusicPlayer::gameDurations[] = {
  4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 4, 4, 4, 4, 4
};
const int MusicPlayer::gameLength = 19;

const int MusicPlayer::gameoverNotes[] = {
  NOTE_C5, NOTE_G4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_D4, NOTE_C4
};
const int MusicPlayer::gameoverDurations[] = {
  4, 4, 4, 8, 8, 4, 4, 4, 4, 2
};
const int MusicPlayer::gameoverLength = 10;

#endif
