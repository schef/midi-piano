// Pin Definitions
//These are pin definitions from Yamaha keyboard to mega2560

//There are two groups of DATA pins. First one is from 22-32 and second from 34 till 46
const int dataPin[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46};

//Group one is for veloity time calculation
const int selectPinOne[] = {23, 25, 27, 29, 31, 33};
//Group two is for tone
const int selectPinTwo[] = {35, 37, 39, 41, 43, 45};

const int sustainPin = 12;
int sustainStatus;
int sustainStatusOld;

int keyNoteOne[80];
int keyNoteOneOld[80];
int keyNoteTwo[80];
int keyNoteTwoOld[80];

boolean keyStatus[80];

long millisOne[80];
long millisTwo[80];
int millisDiference;
int velocity;

long lastDebounceTime[80];  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int velMin;
int velMax;

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
    pinMode(dataPin[12], INPUT);

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

    pinMode(sustainPin, INPUT);

    Serial.begin(31250);
    //Serial.begin(9600);
    //Serial.print("ready:");
    //Serial.println("");
}

void loop()
{
    //getNote();
    for (int s = 0; s < 6; s++) {

        //selectOne
        digitalWrite(selectPinOne[s], HIGH);

        for (int d = 0; d < 13; d++) {
            int math = d*6+s;
            keyNoteOne[math] = digitalRead(dataPin[d]);
            if (keyNoteOne[math] != keyNoteOneOld[math]){
                if (keyNoteOne[math]){
//                    //Serial.print(d);
//                    //Serial.print(":");
//                    //Serial.print(s);
//                    //Serial.println("");
//                    //serialPrint(0, math, 1, millisOne[math]);
//                    //noteOn(1, math+26, 100);
                millisOne[math] = millis();
                //Serial.println(millisOne[math]);
                }
                else {
                    if(millis() - millisTwo[math] > 10 && keyStatus[math] == true)
                    noteOn(0, math+19, 0);
                    keyStatus[math] = false;
                    //serialPrint(0, math, 0, 0);
                }
                keyNoteOneOld[math] = keyNoteOne[math];
            }
        }

        digitalWrite(selectPinOne[s], LOW);
        //selectTwo
        digitalWrite(selectPinTwo[s], HIGH);

        for (int d = 0; d < 13; d++) {
            int math = d*6+s;
            keyNoteTwo[math] = digitalRead(dataPin[d]);
            if (keyNoteTwo[math] != keyNoteTwoOld[math]){
                //                Serial.print(millisTwo[math]);
                //                Serial.print("-");
                //                Serial.print(millisOne[math]);
                //                Serial.print("=");
                //                Serial.print(millisDiference);
                //                Serial.println("");
                if (keyNoteTwo[math]){
                    //Serial.print(d);
                    //Serial.print(":");
                    //Serial.print(s);
                    //Serial.println("");
                    //noteOn(0, math+19, 100);
                    //serialPrint(1, math, 1, velocity);
                millisTwo[math] = millis();
                //Serial.println(millisTwo[math]);
                millisDiference = millisTwo[math] - millisOne[math];
                if (millisDiference > 19)
                    millisDiference = 19;
                if(keyStatus[math] == false)
                    noteOn(0, math+19, velocityMath(millisDiference));
                    keyStatus[math] = true;
                //if (!millisDiference)
                //    millisDiference = -millisDiference;
                //Serial.println(velocityMath(millisDiference));
                //Serial.println(millisDiference);
                }
                else {
                    //noteOn(0, math+19, 0);
                    //serialPrint(1, math, 0, 0);
                    //Serial.println(millisOne[math]-millisTwo[math]);
                }
                keyNoteTwoOld[math] = keyNoteTwo[math];
            }
        }

        digitalWrite(selectPinTwo[s], LOW);
    }
    // output out the state of the pedal:
    sustainStatus = digitalRead(sustainPin);
    if(sustainStatus != sustainStatusOld){
        if(sustainStatus){
            //MIDI.sendControlChange(64,127,1);
            command(176, 64, 127);
        }
        else{
            //MIDI.sendControlChange(64,0,1);
            command(176, 64, 0);
        }
        sustainStatusOld = sustainStatus;
    }
}

int velocityMath(int millisDiference){
    int velocity = 35;
    velocity = autoScale( 4, 19, 127, 35, millisDiference);
//    if(millisDiference >= 41){
//        //velocity = 0;
//        velocity = 1;
//    }
//    else if (millisDiference <= 40 && millisDiference >= 31){
//        //velocity = 1;
//        velocity = autoScale( 31, 40, 40, 21, millisDiference);
//    }
//    else if (millisDiference <= 30 && millisDiference >= 25){
//        //velocity = 1;
//        velocity = autoScale( 30, 25, 40, 21, millisDiference);
//    }
//    else if (millisDiference <= 24 && millisDiference >= 19){
//        //velocity = 1;
//        velocity = autoScale( 19, 24, 60, 41, millisDiference);
//    }
//    else if (millisDiference <= 18 && millisDiference >= 13){
//        //velocity = 1;
//        velocity = autoScale( 13, 18, 80, 61, millisDiference);
//    }
//    else if (millisDiference <= 12 && millisDiference >= 9){
//        //velocity = 1;
//        velocity = autoScale( 9, 12, 100, 81, millisDiference);
//    }
//    else if (millisDiference < 8){
//        //velocity = 2;
//        velocity = autoScale( 4, 8, 127, 101, millisDiference);
//    }
//    //else{
//    //velocity = autoScale( 0, 28, 120, 41, millisDiference);
//    //}
    return velocity;
}

void serialPrint(int key, int i, int stanje, long unsigned time){ 
    Serial.print("k");
    Serial.print(key);
    Serial.print("-");
    Serial.print(i);
    Serial.print(":");
    Serial.print(stanje);
    Serial.print(":");
    Serial.print(time);
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

int autoScale( int originalMin, int originalMax, int newBegin, int
        newEnd, int inputValue){

    long zeroRefOriginalMax = 0;
    long zeroRefnewEnd = 0;
    long zeroRefCurVal = 0;
    long rangedValue = 0;
    boolean invFlag = 0;

    // Check for out of range inputValues
    if (inputValue < originalMin) {
        inputValue = originalMin;
    }
    if (inputValue > originalMax) {
        inputValue = originalMax;
    }

    // Zero Refference the values
    zeroRefOriginalMax = originalMax - originalMin;

    if (newEnd > newBegin){
        zeroRefnewEnd = newEnd - newBegin;
    }
    else
    {
        zeroRefnewEnd = newBegin - newEnd;
        invFlag = 1;
    }

    zeroRefCurVal = inputValue - originalMin;


    // Check for originalMin > originalMax  - the math for all other cases i.e. negative numbers seems to work out fine
    if (originalMin > originalMax ) {
        return 0;
    }

    if (invFlag == 0){
        rangedValue =  ((zeroRefCurVal * zeroRefnewEnd) /
                zeroRefOriginalMax) + newBegin ;
    }
    else     // invert the ranges
    {  
        rangedValue =  newBegin - ((zeroRefCurVal * zeroRefnewEnd) /
                zeroRefOriginalMax)  ;
    }

    return rangedValue;
}
