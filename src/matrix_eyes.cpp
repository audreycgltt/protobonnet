#include <LedControl.h>
#include "matrix_eyes.h"
#include "anim.h"
#include "main.h"

MatrixEyes::MatrixEyes(){
}

void MatrixEyes::setup(){
  int devices=display.getDeviceCount();
  for(int address=0;address<devices;address++) {
    display.shutdown(address,false);
    display.setIntensity(address,3);
    display.clearDisplay(address);
  }
}

void MatrixEyes::setState(int newState){
    previousState = currentState;
    currentState = newState;
}

void MatrixEyes::update(){
    if (frameCounter < 999){
        switch(currentState){
            case IDLE_STATE:
                currentFrameSizeAnimation = sizeof(IDLE_BLINK)/8;
                runAnimation(IDLE_BLINK, IDLE_BLINK, sizeof(IDLE_BLINK)/8);
                break;
            case IN_LOVE_STATE:
                runAnimation(IN_LOVE, IN_LOVE, sizeof(IN_LOVE)/8);
                break;
            case ANGRY_STATE:
                runAnimation(ANGRY_BLINK_LEFT, ANGRY_BLINK_RIGHT, sizeof(ANGRY_BLINK_RIGHT)/8);
                break;
            case HAPPY_STATE:
                runAnimation(HAPPY_FACE, HAPPY_FACE, sizeof(HAPPY_FACE)/8);
                break;
            case SHITTY_FLUTE_TIME_STATE:
                runAnimation(GETTING_MAD_HYPNOTIK, GETTING_MAD_HYPNOTIK, sizeof(GETTING_MAD_HYPNOTIK)/8);
                break;
            case QUIZZ_STATE:
                runAnimation(QUIZZ_TIME, QUIZZ_TIME, sizeof(QUIZZ_TIME)/8);
                break;
            case DODO_STATE:
                runAnimation(DODO_TIME, DODO_TIME, sizeof(DODO_TIME)/8);
                break;
            default:
                runAnimation(IDLE_BLINK, IDLE_BLINK, sizeof(IDLE_BLINK)/8);
                break;
        }
    }
}

void MatrixEyes::runAnimation(uint64_t left_eye_anim[], uint64_t right_eye_anim[], int size){
    displayImage(left_eye_anim[frameCounter], right_eye_anim[frameCounter]);
    if (++frameCounter >= size ) {
      frameCounter = 0;
    }
}

void MatrixEyes::displayImage(uint64_t left_eye, uint64_t right_eye) {
  for (int i = 0; i < 8; i++) {
    byte leftRow = (left_eye >> i * 8) & 0xFF;
    byte rightRow = (right_eye >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      display.setLed(0, i, j, bitRead(leftRow, j));
      display.setLed(1, i, j, bitRead(rightRow, j));
    }
  }
}


