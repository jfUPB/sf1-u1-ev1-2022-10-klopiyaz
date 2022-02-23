#include "SSD1306Wire.h"

#define BOMB_OUT 25
#define LED_COUNT 26
#define UP_BTN 13
#define DOWN_BTN 32
#define ARM_BTN 33

// Selecciona uno según tu display.
SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_64_48);

void taskBomb() {
  enum class Estado_de_bomba {INIT, WAITING_PRESS, WAITING_RELEASE, BOMBA_ACTIVA, BOMBA_DESACTIVA};
  static Estado_de_bomba estadodebomba =  Estado_de_bomba::INIT;
  static uint8_t bombCounter;
  static uint8_t button;

  static uint32_t tiempoReferencia;

  switch (estadodebomba) {
    case Estado_de_bomba::INIT: {

        Serial.begin(115200);
        pinMode(UP_BTN, INPUT_PULLUP);
        pinMode(DOWN_BTN, INPUT_PULLUP);
        pinMode(ARM_BTN, INPUT_PULLUP);
        pinMode(BOMB_OUT, OUTPUT);
        pinMode(LED_COUNT, OUTPUT);

        display.init();
        display.setContrast(255);
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_LEFT);
        display.setFont(ArialMT_Plain_16);


        bombCounter = 20;
        display.clear();
        display.drawString(10, 20, String(bombCounter));
        display.display();

        Serial.println("Estado_de_bomba::INIT");

        estadodebomba =  Estado_de_bomba::WAITING_PRESS;
        break;
      }

    case Estado_de_bomba::WAITING_PRESS: {
        if (  digitalRead(UP_BTN) == LOW ) {
          tiempoReferencia = millis();
          estadodebomba =  Estado_de_bomba::WAITING_RELEASE;
          button = UP_BTN;
        }
        else if (  digitalRead(DOWN_BTN) == LOW ) {
          tiempoReferencia = millis();
          estadodebomba =  Estado_de_bomba::WAITING_RELEASE;
          button = DOWN_BTN;
        }
        break;
      }

    case Estado_de_bomba::WAITING_RELEASE: {

        if ( (millis() - tiempoReferencia) > 250 ) {

          switch (button) {
            case UP_BTN: {
                if (  digitalRead(UP_BTN) == HIGH ) {
                  if (bombCounter < 60 ) {
                    bombCounter++;
                  }
                  estadodebomba =  Estado_de_bomba::WAITING_PRESS;
                  Serial.println(bombCounter);
                }

                break;
              }
            case DOWN_BTN: {
                if (  digitalRead(DOWN_BTN) == HIGH ) {
                  if (bombCounter > 10 ) {
                    bombCounter--;
                  }
                  estadodebomba =  Estado_de_bomba::WAITING_PRESS;
                  Serial.println(bombCounter);
                }

                break;
              }
          }
          
        }

        break;
      }




    default: {
        Serial.println("Error");
        break;
      }
  }

}

void setup() {
  taskBomb();
}

void loop() {
  taskBomb();
}
