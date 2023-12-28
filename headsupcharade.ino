#include <Arduboy2.h>
#include <stdlib.h>
#include "words.h"

Arduboy2 arduboy;

enum GameState { START, PLAY, END };
GameState gameState = START;
int score = 0;
unsigned long startTime;
long remainingTime;
const unsigned long timerValues[] = {5000, 10000, 20000, 30000, 45000, 60000};
unsigned long wordTime;
const char *const *currentWordList;
int currentWordListSize;
int difficultySelection = 0;
int timerSelection = 2;
int wordIndex = 0;
int shuffledIndices[20];
bool timerSelectionMode = false;
bool inOptionsMenu = false;
bool wordCountSelectionMode = false;
const int wordCounts[] = {1, 2, 3, 4, 5, 6, 10, 20, 30, 50};
int wordCountSelection = 5; 

#define MAX_WORD_LENGTH 45
#define FONT_WIDTH 6
#define FONT_HEIGHT 8
#define MAX_LINES 4
char wordBuffer[MAX_WORD_LENGTH + 1];

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
      handleGameScreenInput(false);
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
  arduboy.print(F("Heads-Up Charades"));

  arduboy.setCursor(0, 16);
  arduboy.print(F("1: Easy"));
  arduboy.setCursor(0, 26);
  arduboy.print(F("2: Medium"));
  arduboy.setCursor(0, 36);
  arduboy.print(F("3: Difficult"));
  arduboy.setCursor(0, 46);
  arduboy.print(F("4: Hard"));
  arduboy.setCursor(0, 56);
  arduboy.print(F("5: Actions"));

  
  arduboy.fillRect(108, 56, 28, 9, WHITE);
  arduboy.setTextColor(BLACK);
  arduboy.setCursor(110, 57);
  arduboy.print(timerValues[timerSelection] / 1000);
  arduboy.print(F("s"));
  arduboy.setTextColor(WHITE);

  
  arduboy.fillRect(108, 36, 28, 9, WHITE);
  arduboy.setTextColor(BLACK);
  arduboy.setCursor(110, 37);
  arduboy.print(wordCounts[wordCountSelection]);
  arduboy.setTextColor(WHITE);

  
  if (inOptionsMenu) {
    if (timerSelectionMode) {
      arduboy.fillCircle(106, 60, 3, WHITE); 
    } else if (wordCountSelectionMode) {
      arduboy.fillCircle(106, 40, 3, WHITE); 
    }
  } else {
    if (difficultySelection < 5) {
      arduboy.fillRect(0, 16 + difficultySelection * 10, 8, 8, WHITE); 
    } else {
      arduboy.drawRect(0, 16, 128, 64, WHITE); 
    }
  }
}

void handleStartScreenInput() {
  if (inOptionsMenu) {
    
    if (arduboy.justPressed(LEFT_BUTTON)) {
      if (timerSelectionMode || wordCountSelectionMode) {
        timerSelectionMode = false;
        wordCountSelectionMode = false;
      } else {
        inOptionsMenu = false; 
      }
    } else if (arduboy.justPressed(RIGHT_BUTTON)) {
      if (!timerSelectionMode && !wordCountSelectionMode) {
        timerSelectionMode = true; 
      }
    }

    if (timerSelectionMode) {
      if (arduboy.justPressed(UP_BUTTON) && timerSelection > 0) timerSelection--;
      else if (arduboy.justPressed(DOWN_BUTTON) && timerSelection < (sizeof(timerValues) / sizeof(timerValues[0])) - 1) timerSelection++;
    } else if (wordCountSelectionMode) {
      if (arduboy.justPressed(UP_BUTTON) && wordCountSelection > 0) wordCountSelection--;
      else if (arduboy.justPressed(DOWN_BUTTON) && wordCountSelection < (sizeof(wordCounts) / sizeof(wordCounts[0])) - 1) wordCountSelection++;
    }
  } else {
    
    if (arduboy.justPressed(UP_BUTTON)) {
      if (difficultySelection > 0) difficultySelection--;
      else difficultySelection = 5;
    } else if (arduboy.justPressed(DOWN_BUTTON)) {
      if (difficultySelection < 5) difficultySelection++;
      else difficultySelection = 0;
    } else if (arduboy.justPressed(RIGHT_BUTTON)) {
      if (difficultySelection == 5) inOptionsMenu = true; 
    }

    if (arduboy.justPressed(A_BUTTON)) {
      switch (difficultySelection) {
        case 0:
          currentWordList = wordsEasy;
          currentWordListSize = sizeof(wordsEasy) / sizeof(wordsEasy[0]);
          break;
        case 1:
          currentWordList = wordsMedium;
          currentWordListSize = sizeof(wordsMedium) / sizeof(wordsMedium[0]);
          break;
        case 2:
          currentWordList = wordsDifficult;
          currentWordListSize = sizeof(wordsDifficult) / sizeof(wordsDifficult[0]);
          break;
        case 3:
          currentWordList = wordsHard;
          currentWordListSize = sizeof(wordsHard) / sizeof(wordsHard[0]);
          break;
        case 4:
          currentWordList = wordsActions;
          currentWordListSize = sizeof(wordsActions) / sizeof(wordsActions[0]);
          break;
      }
      currentWordListSize = min(currentWordListSize, wordCounts[wordCountSelection]);
      wordTime = timerValues[timerSelection];
      startGame();
    }
  }
}

void startGame() {
  score = 0;
  wordIndex = 0;
  startTime = millis();
  gameState = PLAY;
  shuffleWords();
}

void shuffleWords() {
  for (int i = 0; i < currentWordListSize; i++) {
    shuffledIndices[i] = i;
  }
  for (int i = 0; i < currentWordListSize - 1; i++) {
    int j = i + random(currentWordListSize - i);
    int temp = shuffledIndices[i];
    shuffledIndices[i] = shuffledIndices[j];
    shuffledIndices[j] = temp;
  }
}

void drawGameScreen() {
  arduboy.setTextSize(1);
  arduboy.setCursor(0, 0);
  arduboy.print(F("Score: "));
  arduboy.print(score);

  remainingTime = (wordTime - (millis() - startTime)) / 1000;
  if (remainingTime <= 0) {
    gameState = END;
    return;
  }

  arduboy.setCursor(arduboy.width() - 6 * FONT_WIDTH, 0);
  arduboy.print(remainingTime);

  if (wordIndex < currentWordListSize) {
    PGM_P wordPtr = (PGM_P)pgm_read_word(&(currentWordList[shuffledIndices[wordIndex]]));
    strcpy_P(wordBuffer, wordPtr);
    wordBuffer[MAX_WORD_LENGTH] = '\0';

    int charCount = strlen(wordBuffer);
    int numLines = (charCount + (arduboy.width() / FONT_WIDTH - 1)) / (arduboy.width() / FONT_WIDTH);
    numLines = min(numLines, MAX_LINES);

    int yPos = arduboy.height() / 2 - numLines * FONT_HEIGHT / 2;
    int lineStartIndex = 0;

    for (int line = 0; line < numLines; ++line) {
      int lineLength = 0;
      while (lineStartIndex + lineLength < charCount &&
             wordBuffer[lineStartIndex + lineLength] != '\n' &&
             lineLength < (arduboy.width() / FONT_WIDTH)) {
        lineLength++;
      }

      arduboy.setCursor(0, yPos + line * FONT_HEIGHT);
      for (int i = 0; i < lineLength; ++i) {
        arduboy.write(wordBuffer[lineStartIndex + i]);
      }

      lineStartIndex += lineLength;
      if (wordBuffer[lineStartIndex] == '\n') {
        lineStartIndex++;
      }
      while (wordBuffer[lineStartIndex] == ' ' && lineStartIndex < charCount) {
        lineStartIndex++;
      }
    }
  } else {
    gameState = END;
  }
}

void handleGameScreenInput(bool timeOut) {
  if (arduboy.justPressed(A_BUTTON) || timeOut) {
    if (timeOut) {
      score--;
    } else {
      score++;
    }
    nextWord();
  } else if (arduboy.justPressed(B_BUTTON)) {
    score--;
    nextWord();
  }
}

void nextWord() {
  wordIndex++;
  if (wordIndex >= currentWordListSize) {
    gameState = END;
  } else {
    startTime = millis();
  }
}

void drawEndScreen() {
  arduboy.setTextSize(1);
  arduboy.setCursor(20, 0);
  arduboy.print(F("Game Over"));
  arduboy.setCursor(0, 20);
  arduboy.print(F("Final Score: "));
  arduboy.print(score);
  arduboy.setCursor(0, 40);
  arduboy.print(F("Press A to Play Again"));
}

void handleEndScreenInput() {
  if (arduboy.justPressed(A_BUTTON)) {
    gameState = START;
  }
}
