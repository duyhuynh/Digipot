/**
 * @file max5389.h
 * @brief low-voltage linear taper digital potentiometer.
 *
 * @version 0.0.1
 * @author Duy Huynh
 **/
 
#ifndef _MAX5389_H
#define _MAX5389_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------*
 *                           Includes                               *
 *------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Add gpio header here

/*------------------------------------------------------------------*
 *                          Define macro                            *
 *------------------------------------------------------------------*/
// define delay function
// replace this function by your delay nano second function
#define DELAY_NS(value) delay_ns(value)
// define GPIO functions
// Change to gpio functions of MCU that you used in your project
// Replace this function by your pin write function
#define MAX5389_PIN_WRITE(pin, value) PinWrite(pin, value)
// Replace this function by your pin module init function
#define MAX5389_PIN_INIT()  PinModuleInit()
 
#define	u8	unsigned	char
#define	u16	unsigned	short
#define	u32	unsigned	int
#define	u64	unsigned	long	long

#define	f32	float
#define	f64	double 

//Define MAX5389 pins
#define	PinCSA	1
#define	PinCSB	14
#define	PinUDA	2	
#define	PinUDB	3
#define	PinINCA	13
#define	PinINCB	12

#define DOWN 0
#define UP 1
#define LOW 0
#define HIGH 1

#define MAX5389_TAP_MAX 256        //256-tap
#define MAX5389_TAP_POWER_ON MAX5389_TAP_MAX/2   // Power-On Sets Wiper to Midscale

// Resistance between high terminal and low terminal
#define MAX5389_RESISTANCE_10K 10000  
#define MAX5389_RESISTANCE_50K 50000 
#define MAX5389_RESISTANCE_100K 100000

// Scale error
#define MAX5389_FULL_SCALE_ERR_10K -3
#define MAX5389_FULL_SCALE_ERR_50K -1
#define MAX5389_FULL_SCALE_ERR_100K -0.5
  
#define MAX5389_ZERO_SCALE_ERR_10K 3
#define MAX5389_ZERO_SCALE_ERR_50K 1
#define MAX5389_ZERO_SCALE_ERR_100K 0.5

/*
  PARAMETER      | SYMBOL | CONDITIONS   |MIN | TYP | MAX | UNITS
Wiper Resistance | RWL    | VDD > 2.6V   |    | 250 | 600 |   Ω
                 |        | VDD > 4.75V  |    | 150 | 200 |
*/
#define MAX5389_WIPER_RESISTANCE_5V    200    // Wiper Resistance with VDD > 4.75V
#define MAX5389_WIPER_RESISTANCE_3V    600    // Wiper Resistance with VDD > 2.6V


 
/*------------------------------------------------------------------*
 *                      Define enum, structure                      *
 *------------------------------------------------------------------*/
typedef enum _max5389_channel_t
{
	CHANNEL_A = 0,
	CHANNEL_B
} max5389_channel_t;

typedef struct _max5389_ctl_pin_t
{
	int pinINC;  
	int pinCS;
	int pinUD;
} max5389_ctl_pin_t;

typedef struct _max5389_wp_res_t
{
	u16 tap_max;     // Wiper tap maximum. Number of unique wiper positions between high terminal and low terminal
	f32 hl_res;      // Total resistance between high terminal and low terminal (end to end resistance).
	f32 step_res;    // Step resistance = res_hl/tap_max
	f32 wiper_res;   // Wiper resistance. The resistance of the analog switch that connects the Wiper Terminal to the Resistor Ladder. 
	u16 current_pos; // Current wiper position
} max5389_wp_res_t;

/*------------------------------------------------------------------*
 *         Define callback functions, interrupt handler             *
 *------------------------------------------------------------------*/

/*------------------------------------------------------------------*
 *                        Global Data Types                         *
 *------------------------------------------------------------------*/


/*------------------------------------------------------------------*
 *                       Function Prototypes                        *
 *------------------------------------------------------------------*/  
// Initialize MAX5389
void max5389_init(u16 hl_res_a, u16 resolution_a, f32 wiper_res_a, u16 hl_res_b, u16 resolution_b, f32 wiper_res_b);
// Wiper position
// Increment wiper position
u16 max5389_inc_wiper_pos(max5389_channel_t channel);
// Decrement wiper position
u16 max5389_dec_wiper_pos(max5389_channel_t channel);
// Set a specific wiper position 
u16 max5389_set_wiper_pos(max5389_channel_t channel, u16 val);
// Get current wiper position 
u16 max3589_get_wiper_pos((max5389_channel_t channel);
// Wiper resistence
// Set a specific wiper resistance 
f32 max5389_set_wiper_res(max5389_channel_t channel, f32 val);
// Get current wiper resistance 
f32 max3589_get_wiper_res((max5389_channel_t channel);
// set wiper position to minimum
u16 max5389_wiper_pos_min(max5389_channel_t channel);
// set wiper position to maximum
u16 max5389_wiper_pos_max(max5389_channel_t channel);
 
#ifdef __cplusplus
} /* extern "C" */
#endif

#endif //!_MAX5389_H

/******************* Copyright(c) 2023 *****END OF FILE****/
