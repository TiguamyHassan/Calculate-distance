
#include <LedControl.h>

LedControl lc = LedControl(12, 10, 11, 1);

int snakeX[64], snakeY[64];
int snakeLength = 3;
int foodX, foodY;
int dirX = 1, dirY = 0;
int nextDirX = 1, nextDirY = 0;
bool gameOver = false;
int score = 0;
unsigned long lastMove = 0;
int speed = 200;


const int buzzerPin = 2;

void generateFood();
void moveSnake();
void drawDisplay();
void resetGame();
void playEatSound();
void playGameOverSound();

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  
  randomSeed(analogRead(A5));
  
  int startX = 4, startY = 4;
  for(int i = 0; i < 3; i++) {
    snakeX[i] = startX - i;
    snakeY[i] = startY;
  }
  
  generateFood();
}

void loop() {
  int joyX = analogRead(A0);
  int joyY = analogRead(A1);
  
  if(joyX < 200 && dirX != 1) { 
    nextDirX = -1; 
    nextDirY = 0; 
  }
  else if(joyX > 800 && dirX != -1) { 
    nextDirX = 1; 
    nextDirY = 0; 
  }
  else if(joyY < 200 && dirY != 1) { 
    nextDirX = 0; 
    nextDirY = -1; 
  }
  else if(joyY > 800 && dirY != -1) { 
    nextDirX = 0; 
    nextDirY = 1; 
  }
  
  if(millis() - lastMove > speed) {
    moveSnake();
    lastMove = millis();
  }
  
  drawDisplay();
  
  if(gameOver) {
    playGameOverSound();
    
    for(int b = 0; b < 3; b++) {
      lc.clearDisplay(0);
      delay(300);
      for(int i = 0; i < 8; i++) {
        lc.setLed(0, i, i, true);
        lc.setLed(0, i, 7 - i, true);
      }
      delay(300);
    }
    lc.clearDisplay(0);
    delay(1000);
    resetGame();
  }
}

void moveSnake() {
  if((nextDirX == 1 && dirX == -1) || (nextDirX == -1 && dirX == 1) ||
     (nextDirY == 1 && dirY == -1) || (nextDirY == -1 && dirY == 1)) {
    nextDirX = dirX;
    nextDirY = dirY;
  }
  
  dirX = nextDirX;
  dirY = nextDirY;
  
  int newX = snakeX[0] + dirX;
  int newY = snakeY[0] + dirY;
  
  if(newX < 0) newX = 7;
  if(newX >= 8) newX = 0;
  if(newY < 0) newY = 7;
  if(newY >= 8) newY = 0;
  

  for(int i = 1; i < snakeLength; i++) {
    if(snakeX[i] == newX && snakeY[i] == newY) {
      gameOver = true;
      return;
    }
  }
  
 
  for(int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i-1];
    snakeY[i] = snakeY[i-1];
  }
  
  snakeX[0] = newX;
  snakeY[0] = newY;
  
  if(newX == foodX && newY == foodY) {
    snakeLength++;
    score++;
    playEatSound();  
    generateFood();
    if(speed > 80) speed -= 5;
  }
}

void generateFood() {
  bool valid;
  int attempts = 0;
  do {
    valid = true;
    foodX = random(8);
    foodY = random(8);
    for(int i = 1; i < snakeLength; i++) {
      if(snakeX[i] == foodX && snakeY[i] == foodY) {
        valid = false;
        break;
      }
    }
    attempts++;
    if(attempts > 100) {
      gameOver = true;
      return;
    }
  } while(!valid);
}

void drawDisplay() {
  lc.clearDisplay(0);
  
  for(int i = 1; i < snakeLength; i++) {
    lc.setLed(0, snakeY[i], snakeX[i], true);
  }
  if(snakeLength > 0) {
    lc.setLed(0, snakeY[0], snakeX[0], true);
  }
  
  lc.setLed(0, foodY, foodX, true);
}

void resetGame() {
  snakeLength = 3;
  int startX = 4, startY = 4;
  for(int i = 0; i < 3; i++) {
    snakeX[i] = startX - i;
    snakeY[i] = startY;
  }
  dirX = 1; dirY = 0;
  nextDirX = 1; nextDirY = 0;
  gameOver = false;
  score = 0;
  speed = 200;
  generateFood();
  lc.clearDisplay(0);
}

void playEatSound() {
  
  for(int i = 0; i < 3; i++) {
    tone(buzzerPin, 500 + i * 100, 50);
    delay(60);
  }
  noTone(buzzerPin);
}

void playGameOverSound() {

  for(int i = 0; i < 5; i++) {
    tone(buzzerPin, 800 - i * 100, 100);
    delay(110);
  }
  noTone(buzzerPin);
}
