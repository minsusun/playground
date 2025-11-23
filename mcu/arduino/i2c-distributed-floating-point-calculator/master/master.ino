#include <Wire.h>

#define NUM_SLAVE 4

bool flag = false;
bool isSlaveReady[NUM_SLAVE];

uint32_t jobStartedAt;
uint32_t jobFinishedAt;

union t_uint32{
  uint32_t t_32;
  uint8_t t_8[4];
};

union t_float{
  float t_f_32;
  uint8_t t_8[4];
};

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  if(Serial.available())
  {
    t_uint32 n;

    n.t_32 = Serial.parseInt(); // Serial.parseInt() returns long(4-Byte)

    while(Serial.available()) Serial.read(); // flush remaining

    jobStartedAt = millis();

    propagate(n);

    Serial.print("N: ");
    Serial.println(n.t_32);

    flag = true;
    for(int i = 0; i < NUM_SLAVE; i++) isSlaveReady[i] = false;
  }

  if(flag)
  {
    collect();
  }

  bool globalReady = true;
  for(int i = 0; i < NUM_SLAVE; i++) globalReady &= isSlaveReady[i];

  if(flag && globalReady)
  {
    jobFinishedAt = millis();
    flag = false;

    Serial.print(jobFinishedAt - jobStartedAt);
    Serial.println("ms consumed.");
  }
}

void propagate(t_uint32 n)
{
  for(int i = 1; i < NUM_SLAVE + 1; i++)
  {
    Wire.beginTransmission(i);

    for(int j = 0; j < 4; j++)
    {
      Wire.write(n.t_8[j]);
    }

    Wire.endTransmission();
  }
}

void collect()
{
  for(int i = 1; i < NUM_SLAVE + 1; i++)
  {  
    Wire.requestFrom(i, 5, true);
    if(Wire.available())
    {
      if((char)(Wire.read()) == 'P')
      {
        isSlaveReady[i - 1] = true;

        t_float num;
        num.t_8[0] = Wire.read();
        num.t_8[1] = Wire.read();
        num.t_8[2] = Wire.read();
        num.t_8[3] = Wire.read();

        if(isSlaveReady[i - 1])
        {
          Serial.print("Slave #");
          Serial.print(i);
          Serial.print(" finished: ");
          Serial.println(num.t_f_32);
        }

        Serial.println();
      }
    }
  }
}
