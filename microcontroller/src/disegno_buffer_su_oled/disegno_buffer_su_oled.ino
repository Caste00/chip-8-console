#include <Arduino.h>
#include <U8g2lib.h>

#ifndef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifndef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

unsigned long lastFrame = 0;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); 
/*  In pratica U8G2 è il nome della libreria
    SSD1306 è il controller del displa
    poi c'è la dimensione 128 x 64 pixels
    NONAME perchè di nessuna marca conosciuta
    F sta per frame buffer
    HW_I2C è hardware i2c cioè come comunica
    U8G2_R0 è la rotazione (nessuna, R1 90 gradi, R2 180 e R3 270)
    l'ultimo indica che non c'è un pin di reset
*/
void setup() {
  u8g2.begin();
  // Chip8 chip8;
}

void loop() {
  unsigned long now = millis();
  if (now - lastFrame >= 16) {
    lastFrame = now;

    chip8.tickTimer();
    for (int i = 0; i < 5; i++) {
      uint16_t opcode = chip8.fetch();
      chip8.execute(opcode);
    }
    const auto buffer = chip8.get_display();

    u8g2.clearBuffer();     // Pulisce la memoria interna
    u8g2.drawBitmap(0, 0, 128/8, 64, buffer);
    u8g2.sendBuffer();
  }
}