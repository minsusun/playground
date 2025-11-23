#include "MIDIUSB.h"
#include "SPI.h"

enum NOTE_STATUS{NOTE_OFF,NOTE_ON};
const byte NOTE_CODE[2]={0x08,0x09};
const byte CHANNEL_PREFIX[2]={0x80,0x90};

// LSB -> MSB
const uint8_t SN74HC238_INPUT[3]={6,5,4};
const uint8_t SN74HC238_INPUT_NUM=3;

const uint8_t SN74HC165_PL=7;

const uint8_t SW_MATRIX_ROW=4;
const uint8_t SW_MATRIX_COL=4;

uint16_t SW_STATE[SW_MATRIX_ROW][SW_MATRIX_COL];
uint16_t SW_NOTE[SW_MATRIX_ROW][SW_MATRIX_COL]={
    {29,30,31,32},
    {33,34,35,36},
    {37,38,39,40},
    {41,42,43,44}
};

void MIDINote(NOTE_STATUS mode,byte channel,byte pitch,byte velocity){
    Serial.print("[DEBUG] :: mode ");
    Serial.println(mode);
    Serial.print("[DEBUG] :: channel ");
    Serial.println(CHANNEL_PREFIX[mode]|channel);
    Serial.print("[DEBUG] :: pitch ");
    Serial.println(pitch);
    Serial.print("[DEBUG] :: velocity ");
    Serial.println(velocity);
    midiEventPacket_t message={NOTE_CODE[mode],(CHANNEL_PREFIX[mode]<<4)|channel,pitch,velocity};
    MidiUSB.sendMIDI(message);
    MidiUSB.flush();
}

void selectRow(uint8_t row){
    for(uint8_t i=0;i<SN74HC238_INPUT_NUM;i++)
        digitalWrite(SN74HC238_INPUT[i],(row>>i)&1);
}

uint8_t retrieveStatus(){
    uint8_t status;
    digitalWrite(SN74HC165_PL,HIGH);
    status=SPI.transfer(0);
    digitalWrite(SN74HC165_PL,LOW);
    return status;
}

bool updateSW(uint8_t row,uint8_t col,uint8_t status){
    SW_STATE[row][col]=(SW_STATE[row][col]<<1)|status;
    return SW_STATE[row][col]==0x8000;
}

void setup(){
    for(uint8_t i=0;i<SN74HC238_INPUT_NUM;i++)
        pinMode(SN74HC238_INPUT[i],OUTPUT);
    pinMode(SN74HC165_PL, OUTPUT);
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    Serial.begin(9600);
}

void loop(){
    for(uint8_t i=0;i<SW_MATRIX_ROW;i++){
        selectRow(SW_MATRIX_ROW-i-1);
        uint8_t status=retrieveStatus();
        for(uint8_t j=0;j<SW_MATRIX_COL;j++){
            if(updateSW(i,j,(status>>j)&1)){
                Serial.print("[DEBUG] :: SW status changed. ROW-");
                Serial.print(i);
                Serial.print(" COL-");
                Serial.println(j);
                MIDINote(1,0,SW_NOTE[i][j],0x40);
                delay(100);
                MIDINote(0,0,SW_NOTE[i][j],0x00);
            }
        }
    }
}