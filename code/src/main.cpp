#include <Arduino.h>

// #define DATA_PIN A5
// #define LATCH_PIN A4
// #define CLOCK_PIN A3
// #define DATA_MASK (1 << 5)
// #define LATCH_MASK (1 << 4)
// #define CLOCK_MASK (1 << 3)

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

uint8_t val = 0;


void spiSend(uint8_t val1, uint8_t val2)
{
  // Enable SPI, Master; clock rate fcpu/2
  SPCR = (1<<SPE) | (1<<MSTR);
  SPSR = (1<<SPI2X); // Also clears SPIF interrupt flag

  // Start transmission
  SPDR = val1;
  // Wait for transmission complete
  while (!(SPSR & (1 << SPIF)))
    ;
  // Read data register to clear flag again
  val1 = SPDR;

  // Enable SPI, Master; clock rate fcpu/2
  // SPCR = (1<<SPE) | (1<<MSTR);
  // SPSR = (1<<SPI2X); // Also clears SPIF interrupt flag
  // Start transmission
  SPDR = val2;
  // Wait for transmission complete
  while (!(SPSR & (1 << SPIF)))
    ;
  // Read data register to clear flag again
  val2 = SPDR;

  // Latch into place
  LATCH_PORT |= LATCH_MASK;
  LATCH_PORT &= ~LATCH_MASK;

}

void swSend(uint8_t val1, uint8_t val2)
{
  // Bang out bits MSB first
  if ((val & 128) != 0) DATA_PORT |= DATA_MASK;
  else DATA_PORT &= ~DATA_MASK;
  CLOCK_PORT |= CLOCK_MASK;
  CLOCK_PORT &= ~CLOCK_MASK;

  if ((val & 64) != 0) DATA_PORT |= DATA_MASK;
  else DATA_PORT &= ~DATA_MASK;
  CLOCK_PORT |= CLOCK_MASK;
  CLOCK_PORT &= ~CLOCK_MASK;

  if ((val & 32) != 0) DATA_PORT |= DATA_MASK;
  else DATA_PORT &= ~DATA_MASK;
  CLOCK_PORT |= CLOCK_MASK;
  CLOCK_PORT &= ~CLOCK_MASK;

  if ((val & 16) != 0) DATA_PORT |= DATA_MASK;
  else DATA_PORT &= ~DATA_MASK;
  CLOCK_PORT |= CLOCK_MASK;
  CLOCK_PORT &= ~CLOCK_MASK;

  if ((val & 8) != 0) DATA_PORT |= DATA_MASK;
  else DATA_PORT &= ~DATA_MASK;
  CLOCK_PORT |= CLOCK_MASK;
  CLOCK_PORT &= ~CLOCK_MASK;

  if ((val & 4) != 0) DATA_PORT |= DATA_MASK;
  else DATA_PORT &= ~DATA_MASK;
  CLOCK_PORT |= CLOCK_MASK;
  CLOCK_PORT &= ~CLOCK_MASK;

  if ((val & 2) != 0) DATA_PORT |= DATA_MASK;
  else DATA_PORT &= ~DATA_MASK;
  CLOCK_PORT |= CLOCK_MASK;
  CLOCK_PORT &= ~CLOCK_MASK;

  if ((val & 1) != 0) DATA_PORT |= DATA_MASK;
  else DATA_PORT &= ~DATA_MASK;
  CLOCK_PORT |= CLOCK_MASK;
  CLOCK_PORT &= ~CLOCK_MASK;

  DATA_PORT &= ~DATA_MASK;

  LATCH_PORT |= LATCH_MASK;
  LATCH_PORT &= ~LATCH_MASK;
}

char buf[128];

void setup()
{
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  digitalWrite(LATCH_PIN, LOW);

  // Serial.begin(9600);
  // Serial.write("Hello from the board!\n");
}

void loop()
{
  auto tstart = millis();

  // 1:G  2:B  4:R
  uint8_t x = 5;
  for (uint16_t i = 0; i < 4096; ++i)
  {
    spiSend(1, x);
    spiSend(0, x);
  }
  auto elapsed = millis() - tstart;
  sprintf(buf, "Elapsed: %d msec\n", (int)elapsed);
  // Serial.write(buf);
}
