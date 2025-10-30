#include "MIDIUSB.h"
#include "SPI.h"

enum            MIDI_EVENT{NOTE_OFF,NOTE_ON,KEY_PRESSURE,CONTROLLER,PROGRAM_CHANGE,CHANNEL_PRESSURE,PITCH};
const byte      EVENT_STATUS[7]={0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E};
const uint8_t   NOTE_BIAS=36;

const uint8_t   SN74HC165_PL=3;
const uint8_t   SN74HC165_BIT_BIAS=1;

const uint8_t   SN74HC238_INPUT[3]={5,6,7};
const uint8_t   SN74HC238_INPUT_NUM=3;
const uint8_t   SN74HC238_BIT_WIDTH=8;

const uint8_t   PEDAL_SW=4;
bool            PEDAL_STATE;

const uint8_t   CONV_SW_NUM=4;
const uint8_t   CONV_SW[CONV_SW_NUM]={8,9,10,11};
unsigned char   CONV_STATE;

const uint8_t   SW_MATRIX_ROW=8;
const uint8_t   SW_MATRIX_COL=7;

enum            SW_EVENT{SW_OFF,SW_ON};
unsigned long   SW_UPPER_EVENT_TIMESTAMP[SW_MATRIX_ROW][SW_MATRIX_COL];
unsigned long   SW_LOWER_EVENT_TIMESTAMP[SW_MATRIX_ROW][SW_MATRIX_COL];

uint16_t        SW_STATUS[2]={0x7FFF,0x8000};
uint16_t        SW_UPPER_STATUS[SW_MATRIX_ROW][SW_MATRIX_COL];
uint16_t        SW_LOWER_STATUS[SW_MATRIX_ROW][SW_MATRIX_COL];

bool            SW_NOTE_STATE[SW_MATRIX_ROW][SW_MATRIX_COL];
bool            SW_UPPER_STATE[SW_MATRIX_ROW][SW_MATRIX_COL];
bool            SW_LOWER_STATE[SW_MATRIX_ROW][SW_MATRIX_COL];

inline uint8_t note(uint8_t row,uint8_t col){ return NOTE_BIAS+row+SN74HC238_BIT_WIDTH*col; }

inline byte velocity(unsigned long interval){ return min(255/(interval+1)+30,127); };

void MIDINote(MIDI_EVENT mode,byte channel,byte data1,byte data2){
    Serial.print("[DEBUG] :: mode : "); Serial.println(EVENT_STATUS[mode]);
    Serial.print("[DEBUG] :: channel : "); Serial.println(EVENT_STATUS[mode]|channel);
    Serial.print("[DEBUG] :: data1 : "); Serial.println(data1);
    Serial.print("[DEBUG] :: data2 : "); Serial.println(data2);
    midiEventPacket_t message={EVENT_STATUS[mode],(EVENT_STATUS[mode]<<4)|channel,data1,data2};
    MidiUSB.sendMIDI(message); MidiUSB.flush();
}

void selectRow(uint8_t row){
    for(uint8_t i=0;i<SN74HC238_INPUT_NUM;i++)
        digitalWrite(SN74HC238_INPUT[i],(row>>i)&1);
}

uint16_t retrieveStatus(){
    uint16_t status;
    digitalWrite(SN74HC165_PL,LOW);
    digitalWrite(SN74HC165_PL,HIGH);
    status=SPI.transfer(0);
    status=(status<<8)|SPI.transfer(0);
    return status;
}

void setup(){
    for(uint8_t i=0;i<SN74HC238_INPUT_NUM;i++)
        pinMode(SN74HC238_INPUT[i],OUTPUT);
    pinMode(SN74HC165_PL, OUTPUT);
    pinMode(PEDAL_SW, INPUT_PULLUP);
    for(uint8_t i=0;i<4;i++)
       pinMode(CONV_SW[i],INPUT_PULLUP);
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    Serial.begin(9600);
}

void loop(){
    for(uint8_t row=0;row<SW_MATRIX_ROW;row++){
        selectRow(row);
        uint16_t status=retrieveStatus();
        for(uint8_t col=0;col<SW_MATRIX_COL;col++){
            if(col==SW_MATRIX_COL-1&&row!=0)
                break;
            SW_UPPER_STATUS[row][col]=(SW_UPPER_STATUS[row][col]<<1)|((status>>(col))&1);
            SW_LOWER_STATUS[row][col]=(SW_LOWER_STATUS[row][col]<<1)|((status>>(col+SN74HC238_BIT_WIDTH))&1);
            if(SW_UPPER_STATUS[row][col]==SW_STATUS[SW_OFF]){
                SW_UPPER_STATE[row][col]=SW_OFF;
                SW_UPPER_EVENT_TIMESTAMP[row][col]=millis();
                if(SW_NOTE_STATE[row][col]==SW_ON&&SW_LOWER_STATE[row][col]==SW_OFF){
                    MIDINote(NOTE_OFF,0,note(row,col),velocity(SW_UPPER_EVENT_TIMESTAMP[row][col]-SW_LOWER_EVENT_TIMESTAMP[row][col]));
                    SW_NOTE_STATE[row][col]=SW_OFF;
                }
            }
            if(SW_UPPER_STATUS[row][col]==SW_STATUS[SW_ON]){
                SW_UPPER_STATE[row][col]=SW_ON;
                SW_UPPER_EVENT_TIMESTAMP[row][col]=millis();
            }
            if(SW_LOWER_STATUS[row][col]==SW_STATUS[SW_OFF]){
                SW_LOWER_STATE[row][col]=SW_OFF;
                SW_LOWER_EVENT_TIMESTAMP[row][col]=millis();
            }
            if(SW_LOWER_STATUS[row][col]==SW_STATUS[SW_ON]){
                SW_LOWER_STATE[row][col]=SW_ON;
                SW_LOWER_EVENT_TIMESTAMP[row][col]=millis();
                if(SW_NOTE_STATE[row][col]==SW_OFF&&SW_UPPER_STATE[row][col]==SW_ON){
                    MIDINote(NOTE_ON,0,note(row,col),velocity(SW_LOWER_EVENT_TIMESTAMP[row][col]-SW_UPPER_EVENT_TIMESTAMP[row][col]));
                    SW_NOTE_STATE[row][col]=SW_ON;
                }
            }
        }
    }
    if(digitalRead(PEDAL_SW)==LOW&&PEDAL_STATE==SW_OFF){
      MIDINote(CONTROLLER,0,64+CONV_STATE,127);
      PEDAL_STATE=SW_ON;
    }
    if(digitalRead(PEDAL_SW)==HIGH&&PEDAL_STATE==SW_ON){
      MIDINote(CONTROLLER,0,64+CONV_STATE,0);
      PEDAL_STATE=SW_OFF;
    }
    for(uint8_t i=0;i<CONV_SW_NUM;i++){
      if(digitalRead(CONV_SW[i])==LOW){
        MIDINote(CONTROLLER,0,64+CONV_STATE,0);
        CONV_STATE=i;
        break;
      }
    }
}