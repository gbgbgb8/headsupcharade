#include <Arduboy2.h>
#include "words.h"

Arduboy2 arduboy;

enum GameState { START, PLAY, END };
GameState gameState = START;

enum SelectionState { MODE, TURNS, TIMER };
SelectionState selectionState = MODE;

const char* gameModes[] = {"Easy", "Medium", "Hard", "Difficult", "Actions"};
const int numGameModes = 5;
int gameModeSelection = 0;

const int turnOptions[] = {1, 2, 4, 6, 10, 15, 25, 50, 99};
const int numTurnOptions = 9;
int turnOptionSelection = 3; 

const int timerOptions[] = {5, 10, 20, 30, 60};
const int numTimerOptions = 5;
int timerOptionSelection = 1; 

int score;
unsigned long startTime;
int remainingTurns;
PGM_P const *currentWordList;
int currentWordIndex = 0;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
  arduboy.clear();
  randomSeed(arduboy.generateRandomSeed());
}

void loop() {
  if (!arduboy.nextFrame()) {
    return;
  }
  arduboy.pollButtons();
  arduboy.clear();

  switch (gameState) {
    case START:
      drawStartScreen();
      handleStartScreenInput();
      break;
    case PLAY:
      drawGameScreen();
      handleGameScreenInput();
      break;
    case END:
      drawEndScreen();
      handleEndScreenInput();
      break;
  }

  arduboy.display();
}

void drawStartScreen() {
  arduboy.setTextSize(1);
  arduboy.setCursor(10, 0);
  arduboy.print(F("Heads-Up Charade"));

  for (int i = 0; i < numGameModes; i++) {
    if (i == gameModeSelection && selectionState == MODE) {
      arduboy.setCursor(0, 10 + i * 10);
      arduboy.print(F("> ")); 
    } else {
      arduboy.setCursor(10, 10 + i * 10);
    }
    arduboy.print(gameModes[i]);
  }

  arduboy.setTextColor(WHITE);
  arduboy.setCursor(70, 10);
  arduboy.print(F("Turns: "));
  arduboy.print(turnOptions[turnOptionSelection]);
  if (selectionState == TURNS) {
    arduboy.setCursor(60, 10);
    arduboy.print(F(">")); 
  }

  arduboy.setCursor(70, 30);
  arduboy.print(F("Timer: "));
  arduboy.print(timerOptions[timerOptionSelection]);
  arduboy.print(F("s"));
  if (selectionState == TIMER) {
    arduboy.setCursor(60, 30);
    arduboy.print(F(">")); 
  }
}

void handleStartScreenInput() {
  if (selectionState == MODE) {
    if (arduboy.justPressed(UP_BUTTON) && gameModeSelection > 0) {
      gameModeSelection--;
    } else if (arduboy.justPressed(DOWN_BUTTON) && gameModeSelection < numGameModes - 1) {
      gameModeSelection++;
    } else if (arduboy.justPressed(RIGHT_BUTTON)) {
      selectionState = TURNS;
    } else if (arduboy.justPressed(A_BUTTON)) {
      startGame();
    }
  } else if (selectionState == TURNS) {
    if (arduboy.justPressed(UP_BUTTON)) {
      selectionState = TIMER;
    } else if (arduboy.justPressed(DOWN_BUTTON)) {
      selectionState = TIMER;
    } else if (arduboy.justPressed(A_BUTTON) && turnOptionSelection < numTurnOptions - 1) {
      turnOptionSelection++;
    } else if (arduboy.justPressed(B_BUTTON) && turnOptionSelection > 0) {
      turnOptionSelection--;
    } else if (arduboy.justPressed(LEFT_BUTTON)) {
      selectionState = MODE;
    }
  } else if (selectionState == TIMER) {
    if (arduboy.justPressed(UP_BUTTON)) {
      selectionState = TURNS;
    } else if (arduboy.justPressed(DOWN_BUTTON)) {
      selectionState = TURNS;
    } else if (arduboy.justPressed(A_BUTTON) && timerOptionSelection < numTimerOptions - 1) {
      timerOptionSelection++;
    } else if (arduboy.justPressed(B_BUTTON) && timerOptionSelection > 0) {
      timerOptionSelection--;
    } else if (arduboy.justPressed(LEFT_BUTTON)) {
      selectionState = MODE;
    }
  }
}

void startGame() {
  switch (gameModeSelection) {
    case 0: currentWordList = wordsEasy; break;
    case 1: currentWordList = wordsMedium; break;
    case 2: currentWordList = wordsDifficult; break;
    case 3: currentWordList = wordsHard; break;
    case 4: currentWordList = wordsActions; break;
  }
  currentWordIndex = 0;
  score = 0;
  remainingTurns = turnOptions[turnOptionSelection];
  startTime = millis();
  gameState = PLAY;
}

void drawGameScreen() {
  arduboy.setTextSize(1);
  arduboy.setCursor(0, 0);
  arduboy.print(F("Score: "));
  arduboy.print(score);

  long remainingTime = timerOptions[timerOptionSelection] - (millis() - startTime) / 1000;
  if (remainingTime <= 0) {
    nextWord();
  }

  arduboy.setCursor(80, 0);
  arduboy.print(remainingTime);

  arduboy.setCursor(0, 30);
  char buffer[30];
  strcpy_P(buffer, (char*)pgm_read_word(&(currentWordList[currentWordIndex])));
  arduboy.print(buffer);
}

void nextWord() {
  remainingTurns--;
  if (remainingTurns <= 0) {
    gameState = END;
  } else {
    currentWordIndex = (currentWordIndex + 1) % 6; 
    startTime = millis();
  }
}

void handleGameScreenInput() {
  if (arduboy.justPressed(A_BUTTON)) {
    score++;
    nextWord();
  }

  if (arduboy.justPressed(B_BUTTON)) {
    nextWord();
  }
}

void drawEndScreen() {
  arduboy.setTextSize(1);
  arduboy.setCursor(20, 20);
  arduboy.print(F("Game Over"));

  arduboy.setCursor(20, 40);
  arduboy.print(F("Score: "));
  arduboy.print(score);

  arduboy.setCursor(20, 50);
  arduboy.print(F("Press A to Restart"));
}

void handleEndScreenInput() {
  if (arduboy.justPressed(A_BUTTON)) {
    gameState = START;
  }
}
