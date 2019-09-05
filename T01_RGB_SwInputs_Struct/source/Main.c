/*
 * @file:			T01_RGB_SwInputs_Struct.c
 * @comany:			ITESO
 * @Engineer Team:	D.F.R. / R.G.P.
 */

#include <stdio.h>
#include <stdint.h>
#include "MK64F12.h"

// #define DEBUG_ON
#define slow_cte  3000
#define fast_cte  1000

void delay(uint16_t delay);
void RGB_White_Fast_or_Slow ( uint16_t speed_par, uint16_t sw3_par, uint16_t sw2_par);

int main(void) {

	uint32_t valueWhite_sw3_i = 0;
	uint32_t valueFast_sw2_i = 0;
	uint32_t speed = slow_cte;

	// CG de PTA, PTB, PTC, - , PTE.
	SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK
			| SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTE_MASK;

	PORTB->PCR[22] = 0x00000100; // RED   - pin como GPIO
	PORTE->PCR[26] = 0x00000100; // GREEN - pin como GPIO
	PORTB->PCR[21] = 0x00000100; // BLUE  - pin como GPIO
	PORTA->PCR[4 ] = 0x00000103; // SW3   - GPIO -	Pull Enable	- Pull Up
	PORTC->PCR[6 ] = 0X00000103; // SW2   - GPIO -	Pull Enable	- Pull Up

	GPIOA->PDDR = 0x00000000;	 // Input - 0 en pin4
	GPIOC->PDDR = 0x00000000;	 // Input - 0 en pin6

	GPIOB->PSOR = 0x00600000; 	 // OFF - 1 en pin22 y pin21 del PTB.
	GPIOE->PSOR = 0x04000000;	 // OFF - 1 en pin26 de PTE.
	GPIOB->PDDR = 0x00400000 | 0x00200000;	 // Output - 1 en pin22 y pin 21 del PTB.
	GPIOE->PDDR = 0x04000000;				 // Output - 1 en pin26 de PTE.

	while (1) {

		valueWhite_sw3_i = GPIOA->PDIR;		 		// Lee completo el GPIOA
		valueWhite_sw3_i = valueWhite_sw3_i & 0x10; // Mask del GPIOC en pin4

		valueFast_sw2_i = GPIOC->PDIR;			  // Lee completo el GPIOC
		valueFast_sw2_i = valueFast_sw2_i & 0x40; // Mask del GPIOC en pin 6

		// Funcion que prende LEDS con SWs
		RGB_White_Fast_or_Slow (speed, valueWhite_sw3_i, valueFast_sw2_i );

		}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
void delay(uint16_t delay) {
	volatile uint16_t j, i;

	for (j = delay; j > 0; j--) {
		for (i = 0; i < 1000; ++i) {
			__asm("nop");
		}

	}
}

void RGB_White_Fast_or_Slow(uint16_t speed_par, uint16_t sw3_par, uint16_t sw2_par) {
if ( sw3_par == 0) {
	speed_par = ( sw2_par == 0) ? fast_cte : slow_cte;
	GPIOB->PTOR = 0x00400000; // ON  -
	GPIOE->PTOR = 0x04000000; // ON - WHITE
	GPIOB->PTOR = 0x00200000; // ON  -
	delay( speed_par );
	GPIOB->PTOR = 0x00400000; // OFF  -
	GPIOE->PTOR = 0x04000000; // OFF - WHITE
	GPIOB->PTOR = 0x00200000; // OFF  -
	delay( speed_par );
} else {
	speed_par = ( sw2_par == 0) ? fast_cte : slow_cte;
	GPIOB->PCOR = 0x00400000; // ON - RED pin22.
	delay(speed_par);
	GPIOB->PSOR = 0x00400000; // OFF - RED pin22.
	delay(speed_par);
	GPIOE->PCOR = 0x04000000; // ON - GREEN pin26.
	delay(speed_par);
	GPIOE->PSOR = 0x04000000; // OFF - GREEN pin26.
	delay(speed_par);
	GPIOB->PCOR = 0x00200000; // ON - BLUE pin21.
	delay(speed_par);
	GPIOB->PSOR = 0x00200000; // OFF - BLUE pin21.
	delay(speed_par);
}
}
