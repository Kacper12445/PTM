#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
 
#include "HD44780.h"
 
#ifndef _BV
#define _BV(bit)				(1<<(bit))
#endif
#ifndef sbi
#define sbi(reg,bit)		reg |= (_BV(bit))
#endif
 
#ifndef cbi
#define cbi(reg,bit)		reg &= ~(_BV(bit))
 
#endif
 
void TimerInit() {
	//Wybranie trybu pracy CTC z TOP OCR1A
	sbi(TCCR1B,WGM12);
	//Wybranie dzielnika czestotliwosci
	sbi(TCCR1B,CS12);
	//Zapisanie do OCR1A wartosci odpowiadajacej 0.1
	OCR1A = 3125;
	//Uruchomienie przerwania
	sbi(TIMSK,OCIE1A);
}
int get_Button()
{
	sbi(PORTC, PC0);
	if(bit_is_clear(PINC, PC0))
	{
		_delay_ms(100);
		return 20;
	}
	//cbi(PORTC, PC0);
	return -1;
}

uint8_t liczby[100] EEMEM;

int decy_sekundy = 0;

int main() 
{
	TimerInit();						//ustawianie dzialanie timera
	LCD_Initalize();
	LCD_Home();
	
	char text[20];
	char text2[20];
	int licznik = 0;
	int i = 0;
	int button = 0;
	
	sei();
	decy_sekundy = 0;	
	
	LCD_Clear();
	LCD_GoTo(0, 0);
	licznik = (decy_sekundy/10);
	sprintf(text, "Welcome !");
	LCD_WriteText(text);

	_delay_ms(1000);
	
	while (licznik >= 0 && licznik < 255)
	{
		button = get_Button();
		if(button == 20)
		{
			LCD_Clear();
			LCD_GoTo(0, 0);
			licznik = (decy_sekundy/10);
			
			eeprom_write_byte(&liczby[i], licznik);
			
			sprintf(text, "Licznik: %d", licznik);
			LCD_WriteText(text);
			
			LCD_GoTo(0,1);
			sprintf(text2, "Ineks: %d", i);
			LCD_WriteText(text2);
			i++;
			_delay_ms(250);
		}

	}
}

ISR(TIMER1_COMPA_vect)
{
	decy_sekundy++;			//liczenie w zmiennej decysekund
}
