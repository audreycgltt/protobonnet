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
    switch(currentState){
        case IDLE_STATE:
            runAnimation(IDLE_BLINK, IDLE_BLINK, sizeof(IDLE_BLINK)/8);
            break;
        case IN_LOVE_STATE:
            runAnimation(IN_LOVE, IN_LOVE, sizeof(IN_LOVE)/8);
            break;
        case ANGRY_STATE:
            break;
        case HAPPY_STATE:
            break;
        case SHITTY_FLUTE_TIME_STATE:
            break;
        case FALLING_ASLEEP_STATE:
            break;
        default:
            break;
    }
}

void MatrixEyes::loveMode(){

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
    byte rightRow = (left_eye >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      display.setLed(0, i, j, bitRead(leftRow, j));
      display.setLed(1, i, j, bitRead(rightRow, j));
    }
  }
}


