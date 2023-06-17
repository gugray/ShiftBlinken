#ifndef LEVEL_PULSER_H
#define LEVEL_PULSER_H

#include <Arduino.h>

#define N_LEVELS 64

struct LevelPulser
{
  enum ChannelMask
  {
    rOn = 1,
    gOn = 2,
    bOn = 4,
  };

  static uint8_t dither0[N_LEVELS];
  static uint8_t dither1[N_LEVELS];
  static uint8_t dither2[N_LEVELS];
  static uint8_t dither3[N_LEVELS];
  static uint8_t dither4[N_LEVELS];
  static uint8_t dither5[N_LEVELS];
  static uint8_t dither6[N_LEVELS];
  static uint8_t dither7[N_LEVELS];
  static uint8_t *ditherParts[8];
  
  volatile static uint8_t currDitherPartIx;
  volatile static uint8_t *currDitherPart;
  volatile static uint8_t cycle;
  volatile uint8_t rLevel = 0;
  volatile uint8_t gLevel = 0;
  volatile uint8_t bLevel = 0;

  static void init();
  static void tick();

  uint8_t getState();
  void setLevels(uint8_t r, uint8_t g, uint8_t b);
};

#endif
