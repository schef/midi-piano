#include <Arduino.h>

void setup();
void loop();
void getNote();
void serialPrint(int key, int i, boolean stanje);
void noteOn(int channel, int pitch, int velocity);
void midiChannelExpression(int channel, int value);
void command(int cmd, int value1, int value2);
#line 1 "src/sketch.ino"
// Pin Definitions
//These are pin definitions from Yamaha keyboard to mega2560

//There are two groups of DATA pins. First one is from 22-32 and second from 34 till 46
const int dataPin[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46};

//Group one is for veloity time calculation
const int selectPinOne[] = {23, 25, 27, 29, 31, 33};
//Group two is for tone
const int selectPinTwo[] = {35, 37, 39, 41, 43, 45};

boolean keyPressedOne[75];
boolean keyPressedTwo[75];
boolean keyPressedOneOld[75];
boolean keyPressedTwoOld[75];

unsigned long currentMillis[150];
unsigned long previousMillis[150];


void setup()
{
    pinMode(dataPin[0], INPUT);
    pinMode(dataPin[1], INPUT);
    pinMode(dataPin[2], INPUT);
    pinMode(dataPin[3], INPUT);
    pinMode(dataPin[4], INPUT);
    pinMode(dataPin[5], INPUT);
    pinMode(dataPin[6], INPUT);
    pinMode(dataPin[7], INPUT);
    pinMode(dataPin[8], INPUT);
    pinMode(dataPin[9], INPUT);
    pinMode(dataPin[10], INPUT);
    pinMode(dataPin[11], INPUT);

    pinMode(selectPinOne[0], OUTPUT);
    pinMode(selectPinOne[1], OUTPUT);
    pinMode(selectPinOne[2], OUTPUT);
    pinMode(selectPinOne[3], OUTPUT);
    pinMode(selectPinOne[4], OUTPUT);
    pinMode(selectPinOne[5], OUTPUT);

    pinMode(selectPinTwo[0], OUTPUT);
    pinMode(selectPinTwo[1], OUTPUT);
    pinMode(selectPinTwo[2], OUTPUT);
    pinMode(selectPinTwo[3], OUTPUT);
    pinMode(selectPinTwo[4], OUTPUT);
    pinMode(selectPinTwo[5], OUTPUT);

    //Serial.begin(31250);
    Serial.begin(9600);
    Serial.print("ready:");
}

void loop()
{
getNote();
}

void getNote() {

    for (int s = 0; s < 6; s++) {

        digitalWrite(selectPinOne[s], HIGH);

        for (int d = 0; d < 12; d++) {

            int number = d*6+s;
            keyPressedOne[number] = digitalRead(dataPin[d]);

            // key pressed
            if (keyPressedOne[number] && (keyPressedOne[number] != keyPressedOneOld[number])) {

                keyPressedOne[number] = true;
                keyPressedOneOld[number] = keyPressedOne[number];

                // put the note into buffer

                //previousMillisChannel[d] = currentMillis;

                //noteChannelPressed[d][notesChannel[d]] = 8 * d + s;
                //notesChannel[t]++;
                serialPrint(0, number, true);
                
            }

            // key released

            if (keyPressedOne[number] == 0 && keyPressedOne[number] != keyPressedOneOld[number]) {

                keyPressedOne[number] = false;
                keyPressedOneOld[number] = keyPressedOne[number];
                //noteOn(t, keyToMidiMap[8 * d + s], 0);
                serialPrint(0, number, false);
            }
        }

        digitalWrite(selectPinOne[s], LOW);
    }
}

void serialPrint(int key, int i, boolean stanje){ 
	Serial.print("k");
	Serial.print(key);
	Serial.print("-");
	Serial.print(i);
	Serial.print(":");
	Serial.print(stanje);
	Serial.println(""); 
}

void noteOn(int channel, int pitch, int velocity) {
    command(0x90 + channel, pitch, velocity);
}


void midiChannelExpression(int channel, int value) {
    // Modulation wheel effect
    command(0xB0 + channel, 0x01, value);
}

void command(int cmd, int value1, int value2) {
    Serial.write(cmd);
    Serial.write(value1);
    Serial.write(value2);
}
