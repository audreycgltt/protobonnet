
#include "leds_body.h"

#include <FastLED.h>

#include "custom_color_palettes.h"
#include "main.h"

bool isOn = false;

LEDsBody::LEDsBody() {
}

void LEDsBody::setup() {
    FastLED.addLeds<WS2812B, LEDS_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(brightness);
}

void LEDsBody::setState(int newState) {
    if (newState == SHITTY_FLUTE_TIME_STATE) {
        FastLED.setBrightness(255);
    } else {
        FastLED.setBrightness(brightness);
    }
    previousState = currentState;
    currentState = newState;
}

void LEDsBody::setQuizzLEDs(int r, int g, int b) {
    quizzRed = r;
    quizzGreen = g;
    quizzBlue = b;
}

void LEDsBody::setNormalBrightness(int newBrightness) {
    brightness = newBrightness;
    FastLED.setBrightness(brightness);
}

void LEDsBody::update() {
    switch (currentState) {
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
        case SUB_STATE:
            testAnimation();
            break;
        case DODO_STATE:
            pacifica_loop();
            break;
        default:
            idleRainbow();
            break;
    }
    FastLED.show();
}

void LEDsBody::idleRainbow() {
    uint8_t thisHue = beat8(10, 255);

    fill_rainbow(leds, NUM_LEDS, thisHue, 10);
}

void LEDsBody::fillRBG() {
    fill_solid(leds, NUM_LEDS, CRGB(quizzRed, quizzGreen, quizzBlue));
    FastLED.setBrightness(brightness);
}

void LEDsBody::twoGradient(uint8_t colorIndex, CRGBPalette16 palette) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(palette, colorIndex, 150, LINEARBLEND);
        colorIndex += 3;
    }
}

void LEDsBody::testAnimation() {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    delay(5);
    int pixel = random(NUM_LEDS);
    leds[pixel] = CRGB(255, random8(30, 200), 0);
}

// Fire2012 by Mark Kriegsman
void LEDsBody::fire2012() {
    static byte heat[NUM_LEDS];

    for (int i = 0; i < NUM_LEDS; i++) {
        heat[i] = qsub8(heat[i], random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }

    for (int k = NUM_LEDS - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    if (random8() < SPARKING) {
        int y = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
    }

    for (int j = 0; j < NUM_LEDS; j++) {
        CRGB color = HeatColor(heat[j]);
        int pixelnumber;
        if (gReverseDirection) {
            pixelnumber = (NUM_LEDS - 1) - j;
        } else {
            pixelnumber = j;
        }
        leds[pixelnumber] = color;
    }
}

void LEDsBody::horribleStrobeEffet() {
    long now = millis();
    if (now - lastStrobeEffect > 30) {
        fill_solid(leds, NUM_LEDS, isOn ? CRGB::Black : CRGB::White);
        isOn = !isOn;
        lastStrobeEffect = now;
    }
}

void LEDsBody::pacifica_loop() {
    // Increment the four "color index start" counters, one for each wave layer.
    // Each is incremented at a different speed, and the speeds vary over time.
    static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
    static uint32_t sLastms = 0;
    uint32_t ms = GET_MILLIS();
    uint32_t deltams = ms - sLastms;
    sLastms = ms;
    uint16_t speedfactor1 = beatsin16(3, 179, 269);
    uint16_t speedfactor2 = beatsin16(4, 179, 269);
    uint32_t deltams1 = (deltams * speedfactor1) / 256;
    uint32_t deltams2 = (deltams * speedfactor2) / 256;
    uint32_t deltams21 = (deltams1 + deltams2) / 2;
    sCIStart1 += (deltams1 * beatsin88(1011, 10, 13));
    sCIStart2 -= (deltams21 * beatsin88(777, 8, 11));
    sCIStart3 -= (deltams1 * beatsin88(501, 5, 7));
    sCIStart4 -= (deltams2 * beatsin88(257, 4, 6));

    // Clear out the LED array to a dim background blue-green
    fill_solid(leds, NUM_LEDS, CRGB(2, 6, 10));

    // Render each of four layers, with different scales and speeds, that vary over time
    pacifica_one_layer(pacifica_palette_1, sCIStart1, beatsin16(3, 11 * 256, 14 * 256), beatsin8(10, 70, 130), 0 - beat16(301));
    pacifica_one_layer(pacifica_palette_2, sCIStart2, beatsin16(4, 6 * 256, 9 * 256), beatsin8(17, 40, 80), beat16(401));
    pacifica_one_layer(pacifica_palette_3, sCIStart3, 6 * 256, beatsin8(9, 10, 38), 0 - beat16(503));
    pacifica_one_layer(pacifica_palette_3, sCIStart4, 5 * 256, beatsin8(8, 10, 28), beat16(601));

    // Add brighter 'whitecaps' where the waves lines up more
    pacifica_add_whitecaps();

    // Deepen the blues and greens a bit
    pacifica_deepen_colors();
}

// Add one layer of waves into the led array
void LEDsBody::pacifica_one_layer(CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff) {
    uint16_t ci = cistart;
    uint16_t waveangle = ioff;
    uint16_t wavescale_half = (wavescale / 2) + 20;
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        waveangle += 250;
        uint16_t s16 = sin16(waveangle) + 32768;
        uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
        ci += cs;
        uint16_t sindex16 = sin16(ci) + 32768;
        uint8_t sindex8 = scale16(sindex16, 240);
        CRGB c = ColorFromPalette(p, sindex8, bri, LINEARBLEND);
        leds[i] += c;
    }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void LEDsBody::pacifica_add_whitecaps() {
    uint8_t basethreshold = beatsin8(9, 55, 65);
    uint8_t wave = beat8(7);

    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        uint8_t threshold = scale8(sin8(wave), 20) + basethreshold;
        wave += 7;
        uint8_t l = leds[i].getAverageLight();
        if (l > threshold) {
            uint8_t overage = l - threshold;
            uint8_t overage2 = qadd8(overage, overage);
            leds[i] += CRGB(overage, overage2, qadd8(overage2, overage2));
        }
    }
}

// Deepen the blues and greens
void LEDsBody::pacifica_deepen_colors() {
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
        leds[i].blue = scale8(leds[i].blue, 145);
        leds[i].green = scale8(leds[i].green, 200);
        leds[i] |= CRGB(2, 5, 7);
    }
}