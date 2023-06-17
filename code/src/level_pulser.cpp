#include "level_pulser.h"

uint8_t LevelPulser::dither0[N_LEVELS];
uint8_t LevelPulser::dither1[N_LEVELS];
uint8_t LevelPulser::dither2[N_LEVELS];
uint8_t LevelPulser::dither3[N_LEVELS];
uint8_t LevelPulser::dither4[N_LEVELS];
uint8_t LevelPulser::dither5[N_LEVELS];
uint8_t LevelPulser::dither6[N_LEVELS];
uint8_t LevelPulser::dither7[N_LEVELS];

uint8_t *LevelPulser::ditherParts[8];

volatile uint8_t LevelPulser::currDitherPartIx = 0;
volatile uint8_t *LevelPulser::currDitherPart = dither0;

volatile uint8_t LevelPulser::cycle = 0;

uint64_t calcDither(uint8_t level)
{
  uint64_t res = 0;
  float step = (float)N_LEVELS / (float)level;
  float cumul = 0;
  uint8_t cnt = 0;
  for (uint8_t i = 0; i < N_LEVELS; ++i)
  {
    res *= 2;
    cumul += 1;
    if (cumul > step && cnt < level)
    {
      cumul -= step;
      cnt += 1;
      res += 1;
    }
    else if (i == 0 && cnt < level)
    {
      cnt += 1;
      res += 1;
    }
  }
  return res;
}

void LevelPulser::init()
{
  for (uint8_t i = 0; i < N_LEVELS; ++i)
  {
    uint64_t d = calcDither(i);
    dither0[i] = (uint8_t)(d & 0xff);
    d >>= 8;
    dither1[i] = (uint8_t)(d & 0xff);
    d >>= 8;
    dither2[i] = (uint8_t)(d & 0xff);
    d >>= 8;
    dither3[i] = (uint8_t)(d & 0xff);
    d >>= 8;
    dither4[i] = (uint8_t)(d & 0xff);
    d >>= 8;
    dither5[i] = (uint8_t)(d & 0xff);
    d >>= 8;
    dither6[i] = (uint8_t)(d & 0xff);
    d >>= 8;
    dither7[i] = (uint8_t)(d & 0xff);
  }
  ditherParts[0] = dither0;
  ditherParts[1] = dither1;
  ditherParts[2] = dither2;
  ditherParts[3] = dither3;
  ditherParts[4] = dither4;
  ditherParts[5] = dither5;
  ditherParts[6] = dither6;
  ditherParts[7] = dither7;
}

void LevelPulser::tick()
{
  cycle = (cycle + 1) % 8;
  if (cycle == 0)
  {
    currDitherPartIx = (currDitherPartIx + 1) % 8;
    currDitherPart = ditherParts[currDitherPartIx];
  }
}

uint8_t LevelPulser::getState()
{
  uint8_t res = 0;
  uint8_t mask = (1 << cycle);
  if ((currDitherPart[rLevel] & mask) != 0) res |= rOn;
  if ((currDitherPart[gLevel] & mask) != 0) res |= gOn;
  if ((currDitherPart[bLevel] & mask) != 0) res |= bOn;

  return res;
}

void LevelPulser::setLevels(uint8_t r, uint8_t g, uint8_t b)
{
  rLevel = r;
  gLevel = g;
  bLevel = b;
}

