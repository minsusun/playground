#ifndef F_CPU
#define F_CPU 8000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define TIMER_FULL_TICK 104
#define TIMER_HALF_TICK 52

#define DATA_PORT PORTA
#define DATA_DDR DDRA
#define CS_PORT PORTB
#define CS_DDR DDRB

#define _UART_TX PA5
#define _UART_RX PA6

#define HD44780_EN PB0
#define HD44780_RS PA4

#define ROTTERY_CLK PB2
#define ROTTERY_DATA PB1

#define LED PA7

volatile uint16_t txBits = 0;

uint8_t c, s;
uint8_t sBreak=0, sModifier=0, sShift=0;

unsigned A0;
uint8_t cursor = 0;
uint8_t counter = 0;
uint8_t value = 255;

void UART_TX(uint8_t data) {
	/* sleep until prior tx ends */
	while(txBits);
	
	/* stop bit(1) - data(8) - start bit(0) */
	/* total 10 bits */
	txBits = (0x0200 | (data << 1)) & ~0xF000;
	
	/* Timer 0 CTC TCCR0A[1:0] TCCR0B[3]*/
	TCCR0A &= ~0x03;
	TCCR0B &= ~0x04;
	TCCR0A |= 0x02;
	TCCR0B |= 0x00;
	
	/* Timer 0 interrupt TIMSK0[1] */
	/* output compare A match */
	TIMSK0 |= 0x02;
	
	/* Timer 0 start */
	TCNT0 = 0x00;
	TCCR0B &= ~0x07;
	TCCR0B |= 0x02;
}

inline void UART_TX_str(char *str) { while(*(str) != '\0') UART_TX(*(str++)); }

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

void HD44780_putchar(char c) {
	HD44780_write_byte(1, c);
	HD44780_wait();
	
	cursor++;
	
	if(cursor == 16) HD44780_write_byte(0, 0x80 + 0x40), HD44780_wait();
	if(cursor >= 32) HD44780_write_byte(0, 0x80), HD44780_wait(), cursor = 0;
}

ISR(TIM0_COMPA_vect) {
	/* cache volatile variable */
	uint16_t l_txBits = txBits;
	
	/* write LSB */
	if(l_txBits & 0x01) PORTA |= (1 << _UART_TX);
	else PORTA &= ~(1 << _UART_TX);
	
	/* remove sended bit */
	l_txBits = (l_txBits >> 1);
	txBits = l_txBits;
	/* when tx ends */
	if(l_txBits == 0) {
		TCCR0B &= ~0x07, TIMSK0 &= ~0x02;
		/* fast PWM */
		TCCR0A |= 0x03;
		TCCR0B &= ~0x04;
		TCCR0A &= 0xCF;
		TCCR0A |= 0x30;
		
		TCCR0B &= 0xF4;
		TCCR0B |= 0x03;
	}
	
	/* update remaining bits */
	
}

ISR(TIM1_COMPA_vect) {
	static uint8_t data = 0;
	static uint8_t bitCount = 0;
	
	bitCount++;
	/* start bit */
	if(bitCount == 1) OCR1A = TIMER_FULL_TICK;
	/* data */
	else if(bitCount >= 2 && bitCount <= 9) data |= (((PINA >> _UART_RX) & 0x01) << (bitCount - 2));
	/* stop bit */
	else {
		/* Timer 1 stop */
		TCCR1B &= ~0x07;
		
		if(data == '+') value = (value - 5 < 0) ? 0 : value - 5;
		if(data == '-') value = (value + 5 > 255) ? 255 : value + 5;
		
		OCR0B = value;
		
		/* send received data */
		HD44780_putchar(data);
		
		/* reset data and bitCount */
		data = 0, bitCount = 0;
		
		/* enable PCINT0 */
		GIMSK |= 0x10;
	}
}

ISR(PCINT0_vect) {
	if(((PINA >> _UART_RX) & 0x01) == 0x00) {
		/* disable PCINT0 */
		GIMSK &= ~0x10;
		
		/* Timer 1 start with HALF TICK */
		TCNT1 = 0;
		OCR1A = TIMER_HALF_TICK;
		TCCR1B |= 0x02; // Pre Scaler 8
	}
}

ISR(PCINT1_vect) {
	uint8_t t = GIMSK;
	GIMSK = 0;
	unsigned A = ((PINB >> ROTTERY_CLK) & 0x01);
	if(A0 != A) {
		A0 = A;
		unsigned B = ((PINB >> ROTTERY_DATA) & 0x01);
		if(A == B) UART_TX('+');
		else UART_TX('-');
	}
	GIMSK = t;
}

int main(void)
{
	DATA_DDR = ((1 << _UART_TX) | (1 << LED) | 0x0F | (1 << HD44780_RS));
	CS_DDR = ((1 << HD44780_EN));
	
	HD44780_init();
	
	HD44780_write_byte(0, 0x01);
	_delay_ms(2);
	
	TCCR0A = 0, TCCR0B = 0;
	
	OCR0B = value;
	
	/* fast PWM */
	TCCR0A |= 0x03;
	TCCR0B &= ~0x04;
	TCCR0A &= 0xCF;
	TCCR0A |= 0x30;
	
	TCCR0B &= 0xF4;
	TCCR0B |= 0x03;
	
	/* Timer 0 match value OCR0A */
	OCR0A = TIMER_FULL_TICK;
	
	/* Timer 1 CTC TCCR1A[1:0] TCCR1B[4:3] */
	TCCR1A &= ~0x03;
	TCCR1B &= ~0x18;
	TCCR1A |= 0x00;
	TCCR1B |= 0x08;
	
	/* Timer 1 interrupt TIMSK1[1] */
	/* output compare A match */
	TIMSK1 |= 0x02;
	
	/* PCIE0 enable GIMSK[4] */
	GIMSK |= 0x30;
	/* PCINT6 mask enable PCMSK0[6] */
	PCMSK0 |= 0x40;
	PCMSK1 |= (1 << ROTTERY_CLK);
	
	A0 = ((PINB >> ROTTERY_CLK) & 0x01);
	counter = 0;
	
	sei();
	
	while (1);
}