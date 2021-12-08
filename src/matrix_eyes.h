#include <LedControl.h>
#include <stdint.h>

#define CS_PIN 15
#define CLK_PIN 18
#define DIN_PIN 23

class MatrixEyes {
    public:

        MatrixEyes();
        void setup();
        void update();
        void setState(int);

    private:
        LedControl display = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 2);
        unsigned long previousMillis = 0;

        int frameCounter = 0;
        int currentState = 0;
        int previousState = 0;

        void loveMode();
        void runAnimation(uint64_t left_eye_anim[], uint64_t right_eye_anim[], int size);
        void displayImage(uint64_t left_eye, uint64_t right_eye);
};