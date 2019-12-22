
#include <Wire.h>
#include <TimerOne.h>
#include <MultiFuncShield.h>


enum FormatType : int {
    FormatLambda = 0,
    FormatAFR,
    FormatLast
};


const int AdcPin = A5;
const int AdcResolution = 1024;


void
setup()
{
    Serial.begin(115200);
    Timer1.initialize();
    MFS.initialize(&Timer1);
    MFS.beep(5);
}


void
loop()
{
    static FormatType formatType = FormatLambda;
    static bool sound = false;

    /* Assuming that ADC voltage is 5V. */

    int conv = analogRead(AdcPin);
    auto timestamp = millis();
    auto value = toValue(conv, formatType);

    /* LED indicator output. */

    MFS.write(value, 2);

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

        return 0.5 + (float) conv / AdcResolution;

    case FormatAFR:
        
        return conv * (22.1 - 7.4) / AdcResolution + 7.4;

    case FormatLast:

        abort();


    }

    return 0;
}