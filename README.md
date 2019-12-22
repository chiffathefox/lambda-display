# Summary

This is a sketch for the AFR/lambda indicator. It assumes an analog signal in the range from 0V to 5V on pin A5.
The sketch is designed for the Arduino UNO board with [Arduino Multifunction Shield](https://www.aliexpress.com/item/33014299284.html). 

# Operations manual

- The sketch shows value on the 4-segment indicator located on the Shield.
- Button S1 switches between AFR and lambda output modes.
- Button S2 turns on the sound indication of how far the current value is away from ideal (1 for AFR and 14.75 for lambda mode).

# Uploading the firmware

To upload the firmware you have to use the Arduino IDE:

1. Open your Arduino IDE.
2. Go to Sketch -> Include Library -> Add .ZIP library and select the library `libraries/MultiFuncShield-Library.zip`.
3. Do the same for `libraries/TimerOne-1.1.0.zip`.
4. Open the sketch `lambda-display.ino` in Arduino IDE.
5. Connect your Arduino UNO board to the PC and hit Upload.

# License

The code is licensed under MIT unless stated otherwise in a file.
