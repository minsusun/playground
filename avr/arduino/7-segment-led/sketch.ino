// 7-segment LED display pins a~g
const int NUM_LED = 7;
const int SEGMENT_PINS[] = {3, 4, 5, 6, 7, 8, 9};

// patterns g~a
const int NUM_PATTERNS = 8;
const int SEGMENT_PATTERNS[] = { 0b0001000, 0b0000011, 0b0100111, 0b0100001, 0b0000110, 0b0001110, 0b0010000, 0b0001001 };

int cnt = 0;

void setup(){
    // setup
    for(int i = 0 ; i < NUM_LED ; i++){
        pinMode(SEGMENT_PINS[i], OUTPUT);
        // check LED works
        digitalWrite(SEGMENT_PINS[i], LOW);
        delay(300);
    }

    // LED reset
    for(int i = 0 ; i < NUM_LED ; i++){
        digitalWrite(SEGMENT_PINS[i], HIGH);
    }
    delay(500);
}

void loop(){
    for(int i = 0 ; i < NUM_LED ; i++)
        digitalWrite(SEGMENT_PINS[i], (SEGMENT_PATTERNS[cnt] >> i) & 1);
    cnt = (cnt + 1) % NUM_PATTERNS;
    delay(500);
}