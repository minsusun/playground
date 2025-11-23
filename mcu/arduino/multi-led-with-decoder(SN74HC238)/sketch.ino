const int BASE_PIN = 4;

const int MSB = 3;
const int LSB = 0;

int cnt = 0;

void setup() {
  // put your setup code here, to run once:
  for(int i = LSB ; i <= MSB ; i++){
    int bitPin = BASE_PIN + i;
    pinMode(bitPin, OUTPUT);
    digitalWrite(bitPin, 0);
  }
}

void loop() {
  for(int i = LSB ; i <= MSB ; i++){
    int bitPin = BASE_PIN + i;
    digitalWrite(bitPin, (cnt>>i) & 1);
  }
  
  cnt = (cnt + 1) % 16;

  delay(500);
}
