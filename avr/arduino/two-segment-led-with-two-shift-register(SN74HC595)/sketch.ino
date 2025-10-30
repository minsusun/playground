// define 74hc595n
const int NUM_74HC595 = 2;
const int NUM_74HC595_OUTPUT = 8;

// define 74hc595n pin connection
const int DS = 4;       // DS, Serial Data Input
const int SHCP = 5;     // SHCP, Shift Register Clock Input
const int STCP = 6;     // STCP, Storage Register Clock Input

// define test pattern
// patterns for testing the led units
// each pattern consists of in order of a to g
const int NUM_TEST_PATTERNS = 8;
const int TEST_PATTERNS[] = { 0b1111111, 0b0111111, 0b0011111, 0b0001111, 0b0000111, 0b0000011, 0b0000001, 0b0000000 };

// define pattern
// patterns for digit 0 to 9 respectively
// each pattern consists of in order of a to g
const int NUM_PATTERNS = 10;
const int PATTERNS[] = { 0b0000001, 0b1001111, 0b0010010, 0b0000110, 0b1001100, 0b0100100, 0b0100000, 0b0001101, 0b0000000, 0b0000100 };

// define counter limit
const int COUNT_LIMIT = 100;

// initialize counter
int cnt = 0;

// pin setup
void setup(){
    pinMode(DS, OUTPUT);
    pinMode(SHCP, OUTPUT);
    pinMode(STCP, OUTPUT);

    for(int i = 0 ; i < NUM_TEST_PATTERNS + 1 ; i++){
        digitalWrite(STCP, LOW);

        for(int j = 0 ; j < NUM_74HC595_OUTPUT ; j++){
            digitalWrite(SHCP, LOW);
            digitalWrite(DS, (TEST_PATTERNS[(i >= NUM_TEST_PATTERNS) ? NUM_TEST_PATTERNS-1 : i]>>j) & 1);
            digitalWrite(SHCP, HIGH);
        }

        digitalWrite(STCP, HIGH);

        delay(200);
    }
}

void loop(){
    int quotient = cnt;
    int remainder = cnt;

    // make storage clock pulse
    digitalWrite(STCP, LOW);

    // write data
    for(int i = 0 ; i < NUM_74HC595 ; i++){
        // digits decompress
        remainder = quotient % NUM_PATTERNS;
        quotient = quotient / NUM_PATTERNS;

        // write digit
        for(int j = 0 ; j < NUM_74HC595_OUTPUT ; j++){
            // make shift clock pulse
            // write data to DS
            digitalWrite(SHCP, LOW);
            digitalWrite(DS, (PATTERNS[remainder]>>j) & 1);
            digitalWrite(SHCP, HIGH);
        }
    }

    // update counter
    cnt = (cnt + 1) % COUNT_LIMIT;

    digitalWrite(STCP, HIGH);

    // delay 500 ms
    delay(500);
}