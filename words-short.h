#include <avr/pgmspace.h>

const char word_1[] PROGMEM = "lip";
const char word_2[] PROGMEM = "baseball";
const char word_3[] PROGMEM = "toothbrush";
const char word_4[] PROGMEM = "backpack";
const char word_5[] PROGMEM = "shoulder";
const char word_6[] PROGMEM = "finger";

const char word_7[] PROGMEM = "trumpet";
const char word_8[] PROGMEM = "salute";
const char word_9[] PROGMEM = "headache";
const char word_10[] PROGMEM = "button";
const char word_11[] PROGMEM = "paddle";
const char word_12[] PROGMEM = "soap";

const char word_13[] PROGMEM = "owl";
const char word_14[] PROGMEM = "gingerbread man";
const char word_15[] PROGMEM = "rainbow";
const char word_16[] PROGMEM = "hook";
const char word_17[] PROGMEM = "softball";
const char word_18[] PROGMEM = "pet";

const char word_19[] PROGMEM = "evolution";
const char word_20[] PROGMEM = "gratitude";
const char word_21[] PROGMEM = "negotiate";
const char word_22[] PROGMEM = "oil";
const char word_23[] PROGMEM = "lifestyle";
const char word_24[] PROGMEM = "level";

const char word_25[] PROGMEM = "paddling in a canoe";
const char word_26[] PROGMEM = "operating a jackhammer";
const char word_27[] PROGMEM = "climbing Mount Everest";
const char word_28[] PROGMEM = "delivering mail";
const char word_29[] PROGMEM = "raking leaves";
const char word_30[] PROGMEM = "feeding the ducks";

PGM_P const wordsEasy[] PROGMEM = {
  word_1, word_2, word_3, word_4, word_5, word_6
};

PGM_P const wordsMedium[] PROGMEM = {
  word_7, word_8, word_9, word_10, word_11, word_12
};

PGM_P const wordsDifficult[] PROGMEM = {
  word_13, word_14, word_15, word_16, word_17, word_18
};

PGM_P const wordsHard[] PROGMEM = {
  word_19, word_20, word_21, word_22, word_23, word_24
};

PGM_P const wordsActions[] PROGMEM = {
  word_25, word_26, word_27, word_28, word_29, word_30
};
