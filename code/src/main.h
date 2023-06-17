#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "level_pulser.h"

// Using SS pin for latch. This must be OUTPUT for SPI master to work!
#define LATCH_PIN 10
#define LATCH_PORT PORTB
#define LATCH_MASK (1 << 2)

// Clock on SCK
#define CLOCK_PIN 13
#define CLOCK_PORT PORTB
#define CLOCK_MASK (1 << 5)

// Data on MOSI
#define DATA_PIN 11
#define DATA_PORT PORTB
#define DATA_MASK (1 << 3)

extern uint8_t currRow;
extern LevelPulser row0[2];
extern LevelPulser row1[2];

void spiSend(uint8_t val1);


#endif
