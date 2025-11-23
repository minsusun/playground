const unsigned PIN_LED1 = A0;
const unsigned PIN_LED2 = A1;

const unsigned PIN_SW1 = 13;
const unsigned PIN_SW2 = 12;

unsigned STATE_LED1 = HIGH;
unsigned STATE_LED2 = HIGH;

int16_t STATE_SW1;
int16_t STATE_SW2;

unsigned long TIMESTAMP_STD;

void setup() {
    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);

    pinMode(PIN_SW1, INPUT_PULLUP);
    pinMode(PIN_SW2, INPUT_PULLUP);

    digitalWrite(PIN_LED1, HIGH);
    digitalWrite(PIN_LED2, HIGH);

    Serial.begin(9600);
    TIMESTAMP_STD = millis();
}

void loop() {
    STATE_SW1 = (STATE_SW1<<1) | digitalRead(PIN_SW1);
    STATE_SW2 = (STATE_SW2<<1) | digitalRead(PIN_SW2);

    unsigned long TIMESTAMP_CUR = millis();

    if(STATE_SW1 == 0x8000){
        char buffer[200];

        digitalWrite(PIN_LED1, !STATE_LED1);

        sprintf(buffer, "[ %8lu ] :: INFO :: SW1 Activated. LED1 turned ", TIMESTAMP_CUR - TIMESTAMP_STD);

        if(STATE_LED1)
            strcat(buffer, "ON");
        else
            strcat(buffer, "OFF");

        STATE_LED1 =! STATE_LED1;

        Serial.println(buffer);
    }

    if(STATE_SW2 == 0x8000){
        char buffer[200];

        digitalWrite(PIN_LED2, !STATE_LED2);

        sprintf(buffer, "[ %8lu ] :: INFO :: SW2 Activated. LED2 turned ", TIMESTAMP_CUR - TIMESTAMP_STD);

        if(STATE_LED2)
            strcat(buffer, "ON");
        else
            strcat(buffer, "OFF");

        STATE_LED2 =! STATE_LED2;

        Serial.println(buffer);
    }
}