#ifndef F_CPU
#define F_CPU 8000000
#endif

/* enable variable delay */
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "Indie Flower Regular_64.h"

#define CS_DDR DDRB
#define CS_PORT PORTB

#define DATA_DDR DDRA
#define DATA_PORT PORTA

#define USI_SCK PA4
#define USI_DO PA5

#define HD44780_EN PB0
#define HD44780_RS PA6

#define MAX7219_CS PB2

#define SH1106_CS PB1
#define SH1106_DC PA6
#define SH1106_RESET PA0

void SpiUSITx(uint8_t data) {
	USIDR = data;
	for(uint8_t i = 0; i < 8; i++) {
		USICR = 0b00010001;
		USICR = 0b00010011;
	}
}

void HD44780_write_byte_nibble(uint8_t rs, uint8_t data) {
	DATA_PORT &= ~(1 << HD44780_RS);
	DATA_PORT |= ((rs & 0x01) << HD44780_RS);
	CS_PORT |= (1 << HD44780_EN);
	DATA_PORT &= 0xF0;
	DATA_PORT |= (data & 0x0F);
	CS_PORT &= ~(1 << HD44780_EN);
}

void HD44780_write_byte(uint8_t rs, uint8_t data) {
	HD44780_write_byte_nibble(rs, ((data >> 4) & 0x0F));
	HD44780_write_byte_nibble(rs, (data & 0x0F));
}

void HD44780_wait(void) { _delay_us(37); }

void HD44780_init(void) {
	/* 15ms wait */
	_delay_ms(15);
	HD44780_write_byte(0, 0x03);
	
	/* 4.1ms wait */
	_delay_us(4100);
	HD44780_write_byte(0, 0x03);
	
	/* 100us wait */
	_delay_us(100);
	
	/* 8-bit mode */
	HD44780_write_byte(0, 0x02);
	HD44780_wait();
	
	/* 8-bit mode, 2 lines, 5x8 font */
	HD44780_write_byte(0, 0x28);
	HD44780_wait();
	
	/* Display Off */
	HD44780_write_byte(0, 0x08);
	HD44780_wait();
	
	/* Clear Display */
	HD44780_write_byte(0, 0x01);
	_delay_ms(2);
	
	/* Entry Mode Set */
	HD44780_write_byte(0, 0x06);
	HD44780_wait();
	
	/* Display On */
	HD44780_write_byte(0, 0x0c);
	HD44780_wait();
}

void HD44780_puts(char *str) {
	unsigned i;
	
	HD44780_write_byte(0, 0x80);
	HD44780_wait();
	
	for(i = 0; i < 16; i++) {
		if(str[i] == '\0') return;
		
		HD44780_write_byte(1, str[i]);
		HD44780_wait();
	}
	
	HD44780_write_byte(0, 0x80 + 0x40);
	HD44780_wait();
	
	for(i = 16; i < 32; i++) {
		if(str[i] == '\0') return;
		
		HD44780_write_byte(1, str[i]);
		HD44780_wait();
	}
}

void MAX7219_init(void) {
	const uint8_t initData[] = {0x09, 0xff, 0x0a, 0x01, 0x0b, 0x07, 0x0c, 0x01, 0x0f, 0x00};
		
	CS_PORT |= (1 << MAX7219_CS);
	
	for(unsigned i = 0; i < sizeof(initData); i += 2) {
		CS_PORT &= ~(1 << MAX7219_CS);
		SpiUSITx(initData[i]);
		SpiUSITx(initData[i + 1]);
		CS_PORT |= (1 << MAX7219_CS);
	}
}

void MAX7219_write_single(unsigned int pos, unsigned int chr) {
	CS_PORT &= ~(1 << MAX7219_CS);
	SpiUSITx(pos + 1);
	_delay_ms(5);
	SpiUSITx(chr);
	CS_PORT |= (1 << MAX7219_CS);
}

void MAX7219_clear(void) { for(unsigned i = 0; i < 8; i++) MAX7219_write_single(i, 0x0F); }

void sh1106_init(void) {
	const uint8_t init_commands[] = {0xae, 0x02, 0x10, 0x40, 0x81, 0xa0, 0xc0, 0xa6, 0xa8, 0x3f, 0xd3, 0x00, 0xd5, 0x50, 0xd9, 0x22, 0xda, 0x12, 0xdb, 0x35, 0xa4, 0xa6, 0xaf};
	CS_PORT &= ~(1 << SH1106_CS);
	DATA_PORT &= ~(1 << SH1106_DC);
	for(unsigned i = 0; i < sizeof(init_commands); i++) SpiUSITx(init_commands[i]);
	CS_PORT |= (1 << SH1106_CS);
	_delay_ms(500);
}

void sh1106_set_location(uint8_t page, uint8_t column) {
	column += 2;
	DATA_PORT &= ~(1 << SH1106_DC);
	SpiUSITx(0xB0 | (page & 0x0F));
	SpiUSITx(0x00 | (column & 0x0F));
	SpiUSITx(0x10 | ((column >> 4) & 0x0F));
}

void sh1106_clear(void) {
	CS_PORT &= ~(1 << SH1106_CS);
	for(unsigned page = 0; page < 8; page++) {
		sh1106_set_location(page, 0);
		DATA_PORT |= (1 << SH1106_DC);
		for(unsigned count = 0; count < 128; count++) SpiUSITx(0);
	}
	CS_PORT |= (1 << SH1106_CS);
}

void sh1106_text(const char *font_addr, uint8_t font_width, uint8_t page, uint8_t column) {
	unsigned l_fill = font_width < 64 ? (64 - font_width) >> 1 : 0;
	unsigned r_fill = (font_width + l_fill) < 64 ? 64 - (font_width + l_fill) : 0;
	CS_PORT &= ~(1 << SH1106_CS);
	for(unsigned y = 0; y < 64 / 8; y++) {
		sh1106_set_location(page + y, column);
		DATA_PORT |= (1 << SH1106_DC);
		for(unsigned x = 0; x < l_fill; x++) SpiUSITx(0);
		for(unsigned x = 0; x < font_width; x++) SpiUSITx(pgm_read_byte(font_addr++));
		for(unsigned x = 0; x < r_fill; x++) SpiUSITx(0);
	}
	CS_PORT |= (1 << SH1106_CS);
}

unsigned int ADC_read(void) { ADCSRA |= 0b01000000; while((ADCSRA & 0b01000000) != 0); return ADCH; }

void sys_init(void) {
	CS_DDR |= ((1 << HD44780_EN) | (1 << SH1106_CS) | (1 << MAX7219_CS));
	DATA_DDR |= (0x0F | (1 << USI_SCK) | (1 << USI_DO) | (1 << HD44780_RS) | (1 << SH1106_DC));
	
	CS_PORT &= ~(1 << HD44780_EN);
	CS_PORT |= ((1 << SH1106_CS) | (MAX7219_CS));

	/* HD44780 initialization */
	HD44780_init();

	/* MAX7219 initialization */
	MAX7219_init();
	MAX7219_clear();

	/* sh1106 initialization */
	sh1106_clear();
	sh1106_init();

	/* ADC initialization PA6 */
	ADMUX = 0b00000111;				// set MUX PA7
	ADCSRA |= 0b10000000;
	ADCSRA |= 0b00000110;
	ADCSRB |= 0b00010000;

	DATA_PORT &= ~(1 << USI_SCK);
}

int main(void)
{
	char msg[] = "Tick Period               ____ms";
	unsigned int s = 55;
	unsigned int m = 59;
	unsigned int h = 99;
	sys_init();
	
	
	sh1106_text(char_addr[0], char_width[0], 0, 0);
	sh1106_text(char_addr[1], char_width[1], 0, 64);
	
	while(1) {
		unsigned long value = ADC_read();
		unsigned long tick = 1000 * value / 255;
		
		msg[26] = (tick == 1000) ? '1' : ' ';
		msg[27] = (tick >= 100) ? ('0' + tick / 100 % 10) : ' ';
		msg[28] = (tick >= 10) ? ('0' + tick / 10 % 10) : ' ';
		msg[29] = '0' + tick % 10;
		
		sh1106_text(char_addr[s/10], char_width[s/10], 0, 0);
		sh1106_text(char_addr[s%10], char_width[s%10], 0, 64);
		
		MAX7219_write_single(0, s % 10);
		MAX7219_write_single(1, s / 10);
		MAX7219_write_single(2, 10);
		MAX7219_write_single(3, m % 10);
		MAX7219_write_single(4, m / 10);
		MAX7219_write_single(5, 10);
		MAX7219_write_single(6, h % 10);
		MAX7219_write_single(7, h / 10);
		
		HD44780_puts(msg);
		
		_delay_ms(tick);
		
		if(++s >= 60) {
			s = 0;
			if(++m >= 60) {
				m = 0;
				if(++h >= 100) h = 0;
			}
		}
	}
}