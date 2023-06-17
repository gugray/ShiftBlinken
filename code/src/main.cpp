#include "main.h"
#include <math.h>

char buf[128];
uint8_t currRow = 0;
LevelPulser row0[2];
LevelPulser row1[2];

void spiSend(uint8_t val1, uint8_t val2, uint8_t val3)
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

  SPDR = val2;
  // Wait for transmission complete
  while (!(SPSR & (1 << SPIF)))
    ;
  // Read data register to clear flag again
  val2 = SPDR;

  SPDR = val3;
  // Wait for transmission complete
  while (!(SPSR & (1 << SPIF)))
    ;
  // Read data register to clear flag again
  val3 = SPDR;

  // Latch into place
  LATCH_PORT |= LATCH_MASK;
  LATCH_PORT &= ~LATCH_MASK;

}

ISR(TIMER2_OVF_vect)
{
  LevelPulser *row = (currRow == 0) ? row0 : row1;
  uint8_t byte0 = 0;
  uint8_t byte1 = (currRow == 0) ? 1 : 8;
  uint8_t byte2 = 0;

  // RGB state for left LED
  uint8_t state = row[0].getState();
  if ((state & LevelPulser::rOn) != 0) byte1 |= 32;
  if ((state & LevelPulser::gOn) != 0) byte1 |= 128;
  if ((state & LevelPulser::bOn) != 0) byte1 |= 64;
  
  // RGB state for right LED
  state = row[1].getState();
  if ((state & LevelPulser::rOn) != 0) byte2 |= 1;
  if ((state & LevelPulser::gOn) != 0) byte2 |= 4;
  if ((state & LevelPulser::bOn) != 0) byte2 |= 2;

  // Transmit
  spiSend(byte0, byte1, byte2);

  currRow = (currRow + 1) % 2;
  if (currRow == 0) LevelPulser::tick();
}


void setup()
{
  LevelPulser::init();

  uint8_t x = 1;
  row0[0].setLevels(x, 0, 0);
  row0[1].setLevels(0, x, 0);
  row1[0].setLevels(0, 0, x);
  row1[1].setLevels(x, x, x);

  digitalWrite(LATCH_PIN, LOW);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  
  // Set up Timer2
  // Prescaler: 16_000_000 / 8 = 2_000_000
  // Overflow: 2_000_000 / 256 = 7812.5 (interrupt fires this many times per second)
  TCCR2A = 0;
  TCCR2B = (1<<CS21); // Prescaler 8
  TIMSK2 = (1<<TOIE2); // Overflow interrupt enable
  ASSR = 0; // Not asynchronous 

  // Serial.begin(9600);
  // Serial.write("Hello from the board!\n");
}

void loop()
{
  float t = millis();
  uint8_t v1 = (sin(t * 0.0011) + 1.0) * 8.0;
  uint8_t v2 = (sin(t * 0.0017) + 1.0) * 8.0;
  uint8_t v3 = (sin(t * 0.0027) + 1.0) * 8.0;
  row0[0].setLevels(v1, v2, v3);
  row0[1].setLevels(v2, v1, v3);
  row1[0].setLevels(v3, v1, v2);
  row1[1].setLevels(v2, v3, v1);
  delay(10);
  return;

  // auto tstart = millis();

  // On the left (first byte)
  // R=32  G=128  B=64
  // On the right (second byte)
  // R=1   G=2    B=4
  spiSend(0, 8 | 128, 0);


  // auto elapsed = millis() - tstart;
  // sprintf(buf, "Elapsed: %d msec\n", (int)elapsed);
  // Serial.write(buf);
}
