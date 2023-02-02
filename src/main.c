/**
 * @file main.c
 * @brief run digital potentiometer
 *
 * @version 0.0.1
 * @author Duy Huynh
 **/
 
/*------------------------------------------------------------------*
 *                           Includes                               *
 *------------------------------------------------------------------*/ 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "max5389.h" 

/*------------------------------------------------------------------*
 *                           Define macro                           *
 *------------------------------------------------------------------*/

/*------------------------------------------------------------------*
 *                        Global Data Types                         *
 *------------------------------------------------------------------*/

/*------------------------------------------------------------------*
 *                        Private Data Types                        *
 *------------------------------------------------------------------*/
 
/*------------------------------------------------------------------*
 *                 Private Function Prototypes                      *
 *------------------------------------------------------------------*/
 
/*------------------------------------------------------------------*
 *                   Public Function Prototypes                     *
 *------------------------------------------------------------------*/

int main(void) 
{
	u16 wp_pos = 0, ret_pos = 0;
	f32 wp_res = 500, ret_res = 0;
	
	// Init max5389 module
	max5389_init(MAX5389_RESISTANCE_10K,MAX5389_TAP_MAX,MAX5389_WIPER_RESISTANCE_5V,
	             MAX5389_RESISTANCE_10K,MAX5389_TAP_MAX,MAX5389_WIPER_RESISTANCE_5V);
	
	//  Set wiper position of channel A to minimum
	ret_pos = max5389_wiper_pos_min(CHANNEL_A);
	printf( "Channel A - Wiper position min: %u\n ", ret_pos);
	//  Set wiper position of channel B to minimum
	ret_pos = max5389_wiper_pos_min(CHANNEL_B);
	printf( "Channel B - Wiper position min: %u\n ", ret_pos);
	//  Set a specific wiper position to channel A
	wp_pos = 5;
	ret_pos = max5389_set_wiper_pos(CHANNEL_A, wp_pos);
	printf( "Channel A - Set wiper position. Set value: %u  Return value : %u\n ", wp_pos, ret_pos);
	//  Set a specific wiper position to channel B
	wp_pos = 300;
	ret_pos = max5389_set_wiper_pos(CHANNEL_B, wp_pos);
	printf( "Channel A - Set wiper position. Set value: %u  Return value : %u\n ", wp_pos, ret_pos);
	//  Set a specific wiper resistance to channel A
	wp_res = 5000;
	ret_res = max5389_set_wiper_res(CHANNEL_A, wp_res);
	printf( "Channel A - Set wiper resistance. Set value: %u  Return value : %u\n ", wp_res, ret_res);
	//  Set a specific wiper resistance to channel B
	wp_res = 1000;
	ret_res = max5389_set_wiper_res(CHANNEL_B, wp_res);
	printf( "Channel A - Set wiper resistance. Set value: %u  Return value : %u\n ", wp_res, ret_res);
	//  Set a specific wiper resistance to channel A
	wp_res = 0;
	ret_res = max5389_set_wiper_res(CHANNEL_A, wp_res);
	printf( "Channel A - Set wiper resistance. Set value: %u  Return value : %u\n ", wp_res, ret_res);
	//  Set a specific wiper resistance to channel B
	wp_res = 12000;
	ret_res = max5389_set_wiper_res(CHANNEL_B, wp_res);
	printf( "Channel A - Set wiper resistance. Set value: %u  Return value : %u\n ", wp_res, ret_res);
}

/******************* Copyright(c) 2023 *****END OF FILE****/
