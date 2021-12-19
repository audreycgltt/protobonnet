
#include <FastLED.h>
#include "leds_body.h"
#include "main.h"

bool isOn = false;

LEDsBody::LEDsBody(){
}

void LEDsBody::setup(){
    FastLED.addLeds<WS2812B, LEDS_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(brightness);

    setupLovePalette();
	setupZenPalette();
}

void LEDsBody::setState(int newState){
	if (newState == SHITTY_FLUTE_TIME_STATE){
		FastLED.setBrightness(255);
	} else {
		FastLED.setBrightness(brightness);
	}
    previousState = currentState;
    currentState = newState;
}

void LEDsBody::setQuizzLEDs(int r, int g, int b){
    quizzRed = r;
    quizzGreen = g;
    quizzBlue = b;
}

void LEDsBody::setNormalBrightness(int newBrightness){
    brightness = newBrightness;
    FastLED.setBrightness(brightness);
}

void LEDsBody::update(){
    switch(currentState){
        case IDLE_STATE:
            idleRainbow();
            break;
        case IN_LOVE_STATE:
            static uint8_t startIndex = 0;
            startIndex = startIndex + 1;
            twoGradient(startIndex, lovePalette);
            FastLED.delay(10);
            break;
        case ANGRY_STATE:
            fire2012();
            break;
        case QUIZZ_STATE:
            fillRBG();
            break;
		case SHITTY_FLUTE_TIME_STATE:
			horribleStrobeEffet();
			break;
        case DODO_STATE:
        	static uint8_t startIndexZ = 0;
            startIndexZ = startIndexZ + 1;
            twoGradient(startIndexZ, zenPalette);
            FastLED.delay(10);
            break;
        default:
            idleRainbow();
            break;
    }
    FastLED.show();
}

void LEDsBody::idleRainbow(){
    uint8_t thisHue = beat8(10,255);                    
  
    fill_rainbow(leds, NUM_LEDS, thisHue, 10);  
}

void LEDsBody::fillRBG(){

    fill_solid(leds, NUM_LEDS, CRGB(quizzRed, quizzGreen, quizzBlue));
}

void LEDsBody::twoGradient(uint8_t colorIndex, CRGBPalette16 palette){
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( palette, colorIndex, 150, LINEARBLEND);
        colorIndex += 3;
    }
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

void LEDsBody::setupLovePalette(){
    CRGB pink = CHSV( HUE_PINK, 255, 255);
    CRGB red  = CHSV( HUE_RED, 255, 255);

    lovePalette = CRGBPalette16(
                                   pink,  pink,  pink, pink,
                                   red, red, red, red,
                                   pink,  pink,  pink, pink, 
                                   red, red, red, red );
}

void LEDsBody::setupZenPalette(){
	CRGB green = CHSV( HUE_GREEN, 255, 255);
    CRGB turquoise  = CHSV( HUE_BLUE, 255, 255);
	CRGB blue = CHSV(HUE_PURPLE, 255, 255);
	CRGB black = CRGB::Black;

	zenPalette = CRGBPalette16(
                                   green,  green,  green, turquoise,
                                   green, turquoise, blue, blue,
                                   turquoise,  blue,  blue, black, 
                                   blue, blue, black, green );
}

void LEDsBody::horribleStrobeEffet(){
	long now = millis();
	if (now - lastStrobeEffect > 30) {
		fill_solid(leds, NUM_LEDS, isOn ? CRGB::Black : CRGB::White );
		isOn = !isOn;
		lastStrobeEffect = now;
  }
}