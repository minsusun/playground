// test bench SLAVE

#include <Wire.h>

#define LED_PIN 2

#define SLAVE_ID 4

union t_uint32{
  uint32_t t_32;
  uint8_t t_8[4];
} num;

union t_float{
  float t_f_32;
  uint8_t t_8[4];
} result;

bool flag;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN + 1, OUTPUT);
  digitalWrite(LED_PIN + 1, LOW);
  Wire.begin(SLAVE_ID);
  Wire.onReceive(receiveHandler);
  Wire.onRequest(requestHandler);
}

void loop()
{
  if(flag)
  {
    digitalWrite(LED_PIN, HIGH);
    for(uint32_t i = 0; i < num.t_32; i++)
    {
      result.t_f_32 += 1.0f;
    }
    flag = false;
    digitalWrite(LED_PIN, LOW);
  }
}

void receiveHandler(int nByte)
{
  num.t_8[0] = Wire.read();
  num.t_8[1] = Wire.read();
  num.t_8[2] = Wire.read();
  num.t_8[3] = Wire.read();
  while(Wire.available())
  {
    Wire.read();
  }
  flag = true;
  result.t_f_32 = 0.0f;
}

void requestHandler()
{
  if(!flag)
  {
    Wire.write('P');
    for(int j = 0; j < 4; j++) Wire.write(result.t_8[j]);
  }
}
