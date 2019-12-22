
#include <Wire.h>
#include <TimerOne.h>
#include <MultiFuncShield.h>


enum FormatType : int {
    FormatLambda = 0,
    FormatAFR,
    FormatLast
};


static float toValue(unsigned int conv, FormatType formatType);


static const int AdcPin = A5;
static const int AdcResolution = 1024;
static const unsigned int DisplayUpdatePeriod = 200;
static const float LambdaOffset = 0.5;
static const float AFREnd = 22.1;
static const float AFRStart = 7.4;



void
setup()
{
    Serial.begin(115200);
    Timer1.initialize();
    MFS.initialize(&Timer1);

    /* Init test of the MFS. */

    MFS.beep(5);
    MFS.write("8.8.8.8.");
    MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, 1);
    delay(300);
    MFS.write("");
    MFS.writeLeds(LED_1 | LED_2 | LED_3 | LED_4, 0);
}


void
loop()
{
    static FormatType formatType = FormatLambda;
    static bool sound = false;
    static unsigned long last_update = 0;

    /* Assuming that ADC voltage is 5V. */

    int conv = analogRead(AdcPin);
    auto timestamp = millis();
    auto value = toValue(conv, formatType);

    /* LED indicator output. */

    if (millis() - last_update >= DisplayUpdatePeriod) {
        last_update = millis();
        MFS.write(value, 2);
    }

    /* Serial output. */

    Serial.print(timestamp);
    Serial.print(' ');
    Serial.println(value);

    /* 
     * Sound indication of how far is AFR value away from 1.0.
     * The further the value is away from 1.0 the more frequent the beeps are.
     * AFR is in range [0.5; 1.5].
     */

    if (sound) {
        auto mid = AdcResolution / 2;
        unsigned int period = abs(mid - conv) * (20 - 1) / mid + 1;

        MFS.beep(1, 1, 1, 0, period);
    }

    /*
     * Handle button actions:
     * S1 press: switch output format type.
     * S2 press turn on sound indication.
     */

    auto button = MFS.getButton();

    switch (button) {


    case BUTTON_1_PRESSED:

        formatType = (FormatType) ((formatType + 1) % FormatLast);

        break;


    case BUTTON_2_PRESSED:

        sound = !sound;

        if (!sound) {
            MFS.beep(0);
        }

        break;


    }

    MFS.writeLeds(LED_1, formatType != 0);
    MFS.writeLeds(LED_2, sound);
    delay(1);
}


static float
toValue(unsigned int conv, FormatType formatType)
{
    switch (formatType) {


    case FormatLambda:

        return LambdaOffset + (float) conv / AdcResolution;

    case FormatAFR:
        
        return conv * (AFREnd - AFRStart) / AdcResolution + AFRStart;

    case FormatLast:

        abort();


    }

    return 0;
}
