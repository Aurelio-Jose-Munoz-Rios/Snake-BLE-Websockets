#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "Config.h"
#include "Point.h"

class SnakeGame {
private:
  Point snake[MAX_SNAKE_LENGTH];
  int length;
  Point food;
  Point direction;
  int score;
  bool isGameOver;
  bool isStarted;

public:
  SnakeGame() : length(3), score(0), isGameOver(false), isStarted(false) {
    direction = Point(1, 0);
  }
  
  void init() {
    length = 3;
    score = 0;
    isGameOver = false;
    isStarted = false;
    direction = Point(1, 0);
    
    snake[0] = Point(GAME_WIDTH/2, GAME_HEIGHT/2);
    snake[1] = Point(GAME_WIDTH/2 - 1, GAME_HEIGHT/2);
    snake[2] = Point(GAME_WIDTH/2 - 2, GAME_HEIGHT/2);
    
    placeFood();
  }
  
  void start() {
    isStarted = true;
  }
  
  bool hasStarted() const { return isStarted; }
  bool hasEnded() const { return isGameOver; }
  int getScore() const { return score; }
  
  void setDirection(Direction dir) {
    switch(dir) {
      case DIR_UP:
        if (direction.y != 1) direction = Point(0, -1);
        break;
      case DIR_DOWN:
        if (direction.y != -1) direction = Point(0, 1);
        break;
      case DIR_LEFT:
        if (direction.x != 1) direction = Point(-1, 0);
        break;
      case DIR_RIGHT:
        if (direction.x != -1) direction = Point(1, 0);
        break;
    }
  }
  
  bool update() {
    if (!isStarted || isGameOver) return false;
    
    // Mover cuerpo
    for (int i = length - 1; i > 0; i--) {
      snake[i] = snake[i-1];
    }
    
    // Mover cabeza
    snake[0].x += direction.x;
    snake[0].y += direction.y;
    
    // Colisión con paredes
    if (snake[0].x < 0 || snake[0].x >= GAME_WIDTH || 
        snake[0].y < 0 || snake[0].y >= GAME_HEIGHT) {
      isGameOver = true;
      return false;
    }
    
    // Colisión consigo misma
    for (int i = 1; i < length; i++) {
      if (snake[0].equals(snake[i])) {
        isGameOver = true;
        return false;
      }
    }
    
    // Comió comida
    if (snake[0].equals(food)) {
      length++;
      score += 10;
      placeFood();
      return true; // Indica que comió
    }
    
    return false;
  }
  
  String toJSON() const {
    String json = "{\"type\":\"game\",\"snake\":[";
    for (int i = 0; i < length; i++) {
      json += "[" + String(snake[i].x) + "," + String(snake[i].y) + "]";
      if (i < length - 1) json += ",";
    }
    json += "],\"food\":[" + String(food.x) + "," + String(food.y) + "],";
    json += "\"score\":" + String(score) + ",";
    json += "\"gameOver\":" + String(isGameOver ? "true" : "false") + "}";
    return json;
  }

private:
  void placeFood() {
    food = Point(random(0, GAME_WIDTH), random(0, GAME_HEIGHT));
  }
};

#endif
