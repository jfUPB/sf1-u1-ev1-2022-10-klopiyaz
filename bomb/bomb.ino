#include "SSD1306Wire.h"

#define BOMB_OUT 25
#define LED_COUNT 26
#define UP_BTN 13
#define DOWN_BTN 32
#define ARM_BTN 33

bool erBTNS = false;
uint8_t erBTNSData = 0;

// Selecciona uno según tu display.
SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_64_48);
void btnsTask() {
  if (digitalRead(UP_BTN) == LOW) {
    erBTNS = true;
    erBTNSData = UP_BTN;
  } else if (digitalRead(DOWN_BTN) == LOW) {
    erBTNS = true;
    erBTNS = DOWN_BTN;
  } else if (digitalRead(ARM_BTN) == LOW) {
    erBTNS = true;
    erBTNS = ARM_BTN;
  }
}

void taskBomb() {
  enum class Estado_de_bomba {INIT, WAITING_PRESS, WAITING_RELEASE, EXPLOTO, BOMBA_ACTIVA, BOMBA_DESACTIVA};
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


        bombCounter = 9;
        display.clear();
        display.drawString(10, 20, String(bombCounter));
        display.display();

        Serial.println("Estado_de_bomba::INIT");

        estadodebomba =  Estado_de_bomba::WAITING_PRESS;
        break;
      }

    case Estado_de_bomba::WAITING_PRESS: {
        if (digitalRead(UP_BTN) == LOW ) {
          tiempoReferencia = millis();
          estadodebomba =  Estado_de_bomba::WAITING_RELEASE;
          button = UP_BTN;
        }
        else if (  digitalRead(DOWN_BTN) == LOW ) {
          tiempoReferencia = millis();
          estadodebomba =  Estado_de_bomba::WAITING_RELEASE;
          button = DOWN_BTN;
        } else if (digitalRead(ARM_BTN) == LOW) {
          tiempoReferencia = millis();
          estadodebomba = Estado_de_bomba::WAITING_RELEASE;
          button = ARM_BTN;
        }
        break;
      }

    case Estado_de_bomba::WAITING_RELEASE: {
        if ((millis() - tiempoReferencia) > 100) {
          switch (button) {
            case UP_BTN: {
                if (digitalRead(UP_BTN) == HIGH ) {
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
            case ARM_BTN: {
                if (digitalRead(ARM_BTN) == HIGH) {
                  uint8_t currentMillis = millis();
                  static uint8_t previousMillis = 0;
                  if (currentMillis - previousMillis >= bombCounter) {
                    previousMillis = currentMillis;
                    bombCounter --;
                    digitalWrite(LED_COUNT, HIGH);
                    delay(1000);
                    digitalWrite(LED_COUNT, LOW);
                    Serial.println(bombCounter);
                  }

                  if (bombCounter == 0) {
                    estadodebomba =  Estado_de_bomba::EXPLOTO;
                  }
                }

              }
              break;
          }
        }

      }

      break;

    case Estado_de_bomba::BOMBA_ACTIVA: {

        break;
      }
    case Estado_de_bomba::EXPLOTO: {
        Serial.println("BOOM!!");
        break;
      }
    case Estado_de_bomba::BOMBA_DESACTIVA: {

        break;
      }

    default: {
        Serial.println("Error");
        break;
      }
  }
}
void lcdTask() {
}

void setup() {
  taskBomb();
}

void loop() {
  taskBomb();
  btnsTask();
  lcdTask();
}
