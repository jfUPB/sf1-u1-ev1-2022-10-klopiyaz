#include "SSD1306Wire.h"
#define BOMB_OUT 25
#define LED_COUNT 26
#define UP_BTN 13
#define DOWN_BTN 32
#define ARM_BTN 33

SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);
bool evBtns = false;
uint8_t evBtnsData = 0;

void bombtask();
void btnsTask();

void setup() {
  Serial.begin(115200);
  bombtask();
  btnsTask();
}
void btnsTask() {
  enum class BtnsStates {INIT, WAITING_PRESS , WAITING_STABLE, WAITING_RELEASE};
  static BtnsStates btnsState =  BtnsStates::INIT;
  static uint32_t referenceTime;
  const uint32_t STABLETIMEOUT = 10;
  static uint8_t lastBtn = 0;

  switch (btnsState) {
    case BtnsStates::INIT: {
        pinMode(ARM_BTN, INPUT_PULLUP);
        pinMode(DOWN_BTN, INPUT_PULLUP);
        pinMode(UP_BTN, INPUT_PULLUP);
        btnsState = BtnsStates::WAITING_PRESS;
        break;
      }
    case BtnsStates::WAITING_PRESS: {

        if (digitalRead(UP_BTN) == LOW) {
          referenceTime = millis();
          lastBtn = UP_BTN;
          btnsState = BtnsStates::WAITING_STABLE;

        }
        else if (digitalRead(DOWN_BTN) == LOW) {
          lastBtn = DOWN_BTN;
          referenceTime = millis();
          btnsState = BtnsStates::WAITING_STABLE;

        }
        else if (digitalRead(ARM_BTN) == LOW) {
          lastBtn = ARM_BTN;
          referenceTime = millis();
          btnsState = BtnsStates::WAITING_STABLE;
        }

        break;
      }
    case BtnsStates::WAITING_STABLE: {
        if (lastBtn == UP_BTN) {
          if (digitalRead(UP_BTN) == HIGH) {
            btnsState = BtnsStates::WAITING_PRESS;
          }
          else if ( (millis() - referenceTime) >= STABLETIMEOUT) {
            btnsState = BtnsStates::WAITING_RELEASE;
          }
        }
        else if (lastBtn == DOWN_BTN) {
          if (digitalRead(DOWN_BTN) == HIGH) {
            btnsState = BtnsStates::WAITING_PRESS;
          }
          else if ( (millis() - referenceTime) >= STABLETIMEOUT) {
            btnsState = BtnsStates::WAITING_RELEASE;
          }
        }
        else if (lastBtn == ARM_BTN) {
          if (digitalRead(ARM_BTN) == HIGH) {
            btnsState = BtnsStates::WAITING_PRESS;
          }
          else if ( (millis() - referenceTime) >= STABLETIMEOUT) {
            btnsState = BtnsStates::WAITING_RELEASE;
          }
        }
        break;
      }
    case BtnsStates::WAITING_RELEASE: {

        if (lastBtn == UP_BTN) {
          if (digitalRead(UP_BTN) == HIGH) {
            evBtns = true;
            evBtnsData = UP_BTN;
            btnsState = BtnsStates::WAITING_PRESS;
            Serial.println("UP_BTN");
          }
        }
        else if (lastBtn == DOWN_BTN) {
          if (digitalRead(DOWN_BTN) == HIGH) {
            evBtns = true;
            evBtnsData = DOWN_BTN;
            btnsState = BtnsStates::WAITING_PRESS;
            Serial.println("DOWN_BTN");
          }
        }
        else if (lastBtn == ARM_BTN) {
          if (digitalRead(ARM_BTN) == HIGH) {
            evBtns = true;
            evBtnsData = ARM_BTN;
            btnsState = BtnsStates::WAITING_PRESS;
            Serial.println("ARM_BTN");
          }
        }

        break;
      }
    default:
      Serial.println("Error");
      break;
  }
}
void bombtask() {
  enum class BombStates {INIT, CONFIG, ARMED};
  static BombStates bombState =  BombStates::INIT;
  static uint8_t counter = 20;

  switch (bombState) {

    case BombStates::INIT: {


        bombState = BombStates::CONFIG;

        break;
      }
    case BombStates::CONFIG: {


        break;
      }
    case BombStates::ARMED: {

      default: {
        }
        break;
      }
  }
}
void disarmTask(uint8_t *vecTryData, uint8_t *vecTrueData, uint8_t vecLengthData, bool *res) {
  for (uint8_t i = 0; i < vecLengthData; i++) {
    if (vecTrueData[i] == vecTryData[i]) {
      *res = true;
    }
    else {
      *res = false;
      break;
    }
  }
}
void loop() {
  bombtask();
  btnsTask();
}
