#include <FastLED.h>

#define LEDS_PIN 2
#define NUM_LEDS 34

#define COOLING  55
#define SPARKING 120
#define gReverseDirection true

class LEDsBody {
    public:

        LEDsBody();
        void setup();
        void update();

        void setState(int);

        CRGB leds[NUM_LEDS];

    private:
        unsigned long previousMillis = 0;
        int frameCounter = 0;

        void idleRainbow();
        void fillRBG(int r, int g, int b);
        void twoGradient();
        void fastBlink();
        void fire2012();

        int currentState = 666;
        int previousState = 0;
};