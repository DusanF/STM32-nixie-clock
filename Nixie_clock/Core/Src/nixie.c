/*
 * nixie.c
 *
 *  Created on: Nov 10, 2020
 *      Author: DusanF
 */

#include <stdint.h>
#include "main.h"	//GPIO definitions

#define GPIOA_MASK 0x01FF
#define GPIOB_MASK 0xF007

void nixie_display(uint8_t *disp){
	uint32_t tmp_a=0, tmp_b=0;

	disp[0] &= 0xF;
	disp[1] &= 0xF;
	disp[2] &= 0xF;
	disp[3] &= 0xF;

	tmp_a = (disp[3]) | (disp[2]<<4) | ((disp[0] & 0x8) << 5);		//pins to enable
	tmp_b = (disp[1] & 0x7) | ((disp[1] & 0x8) << 9) | ((disp[0] & 0x7) << 13);

	tmp_a |= (~tmp_a) << 16;	//pins to disable
	tmp_b |= (~tmp_b) << 16;
	tmp_a &= GPIOA_MASK | (GPIOA_MASK<<16);	//do not touch unused pins
	tmp_b &= GPIOB_MASK | (GPIOB_MASK<<16);

	GPIOA->BSRR = tmp_a;	//apply changes
	GPIOB->BSRR = tmp_b;
}

void nixie_cpp(uint8_t *start, uint8_t *end){	//cathode poisoning prevention - light up every digit for short time
	uint8_t disp[4];
	memcpy(disp, start, 4);
	if(disp[0] == 0)
		disp[0] = 0xF;

	for(uint_fast8_t i=0; i<9; i++){
		for(uint_fast8_t j=(start[0]?0:1); j<4; j++){	//skip first digit if it was not on
			disp[j]++;
			disp[j] %= 10;
			if(disp[j] == end[j] || disp[j] == start[j]){	//skip digit that was or will be on
				disp[j]++;
				disp[j] %= 10;
			}
			if(disp[j] == end[j] || disp[j] == start[j]){
				disp[j]++;
				disp[j] %= 10;
			}
		}
		nixie_display(disp);
		HAL_Delay(75);
	}
}
