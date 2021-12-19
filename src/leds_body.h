#include <FastLED.h>

#define LEDS_PIN 2
#define NUM_LEDS 12 //30

#define COOLING  70
#define SPARKING 120
#define gReverseDirection false

class LEDsBody {
    public:

        LEDsBody();
        void setup();
        void update();

        void setState(int);
        void setQuizzLEDs(int r, int g, int b);
        void setNormalBrightness(int brightness);

        CRGB leds[NUM_LEDS];

    private:
        unsigned long previousMillis = 0;
        unsigned long lastStrobeEffect = 0;

        int frameCounter = 0;
        int brightness = 150;

        void idleRainbow();
        void fillRBG();
        void twoGradient(uint8_t colorIndex, CRGBPalette16 palette);
        void fastBlink();
        void fire2012();
        void horribleStrobeEffet();
        void setupLovePalette();
        void setupZenPalette();

        int currentState = 0;
        int previousState = 0;

        int quizzRed = 0;
        int quizzGreen = 0;
        int quizzBlue = 255;

        CRGBPalette16 lovePalette;
        CRGBPalette16 zenPalette;
        TBlendType    currentBlending;
};