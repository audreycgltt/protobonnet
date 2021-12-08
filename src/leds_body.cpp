
#include <FastLED.h>
#include "leds_body.h"
#include "main.h"

LEDsBody::LEDsBody(){
}

void LEDsBody::setup(){
    FastLED.addLeds<WS2812B, LEDS_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(150);
}

void LEDsBody::setState(int newState){
    previousState = currentState;
    currentState = newState;
}

void LEDsBody::update(){
    switch(currentState){
        case IDLE_STATE:
            idleRainbow();
            break;
        case IN_LOVE_STATE:
            break;
        case ANGRY_STATE:
            break;
        case TEST_STATE:
            twoGradient();
            break;
        default:
            break;
    }
    FastLED.show();
}

void LEDsBody::idleRainbow(){
    uint8_t thisHue = beat8(10,255);                     // A simple rainbow march.
  
    fill_rainbow(leds, NUM_LEDS, thisHue, 10);  
}

void LEDsBody::fillRBG(int r, int g, int b){
    fill_solid( leds, NUM_LEDS, CRGB(r,g,b));
}

void LEDsBody::twoGradient(){
  uint8_t startpos = 0;
  int endpos = NUM_LEDS-1;
  CRGB rgbval(50,0,500);
  CHSV hsvval(100,255,200);

  //fill_gradient_RGB(leds, startpos, 0x000011, endpos, 0x110000);   // You can mix and match long values and CRGB values. Remember, endpos goes up to NUM_LEDS-1
  fill_gradient(leds, startpos, CHSV(50, 255,255) , endpos, CHSV(150,255,255), SHORTEST_HUES);
}

// Fire2012 by Mark Kriegsman
void LEDsBody::fire2012(){
  static byte heat[NUM_LEDS];

    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }

    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}


