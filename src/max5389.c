/**
 * @file max5389.c
 * @brief low-voltage linear taper digital potentiometer.
 *
 * @version 0.0.1
 * @author Duy Huynh
 **/
 
/*------------------------------------------------------------------*
 *                           Includes                               *
 *------------------------------------------------------------------*/ 
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
//Power on will sets wiper position to midscale
static max5389_wp_res_t wp_res[2]; 
static const max5389_ctl_pin_t wp_pins[2] = {{PinINCA,PinCSA,PinUDA},
                                             {PinINCB,PinCSB,PinUDB}};

/*------------------------------------------------------------------*
 *                 Private Function Prototypes                      *
 *------------------------------------------------------------------*/
/**
 * @brief Convert wiper position to wiper resistance
 * @param[in] channel Select channel
 * @param[in] w_pos Wiper position
 * @param[out] None
 * @return wiper resistance
 **/
static f32 pos_to_res(max5389_channel_t channel, u16 w_pos)
{	
	u16 pos = w_pos;
	if(pos > wp_res[channel].tap_max)
		pos = wp_res[channel].tap_max;
	return ((f32)pos * wp_res[channel].step_res) + wp_res[channel].wiper_res;
}

/**
 * @brief Convert wiper resistance to wiper position
 * @param[in] channel Select channel
 * @param[in] w_res Wiper resistance
 * @param[out] None
 * @return wiper position
 **/
static u16 res_to_pos(max5389_channel_t channel, f32 w_res)
{
	f32 res = w_res;
	if(res > wp_res[channel].hl_res)
		return wp_res[channel].tap_max;
	else if(res < wp_res[channel].wiper_res)
		return 0;
	else
		return (u16)((res - wp_res[channel].wiper_res) / wp_res[channel].step_res);
}

/*------------------------------------------------------------------*
 *                   Public Function Prototypes                     *
 *------------------------------------------------------------------*/
/**
 * @brief Initialize MAX5389
 * @param[in] hl_res_a End-to-end resistance channel A
 * @param[in] resolution_a Number of unique wiper positions between high terminal and low terminal channel A
 * @param[in] wiper_res_a Wiper resistance channel A
 * @param[in] hl_res_b End-to-end resistance channel B
 * @param[in] resolution_b Number of unique wiper positions between high terminal and low terminal channel B
 * @param[in] wiper_res_b Wiper resistance channel B
 * @param[out] None
 * @return None
 **/
void max5389_init(u16 hl_res_a, u16 resolution_a, f32 wiper_res_a, u16 hl_res_b, u16 resolution_b, f32 wiper_res_b)
{
	/*
		wiper_res: wiper resistance or Rw The resistance of the analog switch that connects
		the Wiper Terminal to the Resistor Ladder.		
		HIGH
		| n = 256
		|_________________
		\                 |
		/ Rs              |
		\ n = 255  Rw     |
		|__________/______|
		|                 |  Rhw
		\                 |
		/ Rs              |
		\ n = 254  Rw     |
		|__________/______|
		.                 |
		.                 |____W
		.           Rw    |
		|__________/______|
		|                 |
		\                 |
		/ Rs              |
		\ n = 1    Rw     | Rlw
		|__________/______|
		|                 |
		\                 |
		/ Rs              |
		\ n = 0    Rw     |
		|__________/______|
		|
		|
		LOW
	*/
	// Initialize pins of max5389
	MAX5389_PIN_INIT();
	MAX5389_PIN_WRITE(wp_pins[CHANNEL_A].PinCS, HIGH);   // set pin CSA to High to unselect channel A
	MAX5389_PIN_WRITE(wp_pins[CHANNEL_A].PinINC, HIGH);  // set pin INCA to High
	// Unselect channel B
	MAX5389_PIN_WRITE(wp_pins[CHANNEL_B].PinCS, HIGH);  // set pin CSB to High to unselect channel B
	MAX5389_PIN_WRITE(wp_pins[CHANNEL_B].PinINC, HIGH); // set pin INCB to High
	// Init channel A
	wp_res[CHANNEL_A].tap_max = resolution_a;  
	wp_res[CHANNEL_A].hl_res = hl_res_a;       //Rhl
	wp_res[CHANNEL_A].wiper_res = wiper_res_a; //Rw
	// Calculate step resistance
	wp_res[CHANNEL_A].step_res = wp_res[CHANNEL_A].hl_res / (f32)wp_res[CHANNEL_A].tap_max; //Rs
	// Set wiper postion channel A to minimum
	max5389_wiper_pos_min(CHANNEL_A);
	
	// Init channel B
	wp_res[CHANNEL_B].tap_max = resolution;    
	wp_res[CHANNEL_B].hl_res = hl_res;        Rhl
	wp_res[CHANNEL_B].wiper_res = wiper_res;  //Rw
	// Calculate step resistance
	wp_res[CHANNEL_B].step_res = wp_res[CHANNEL_B].hl_res / (f32)wp_res[CHANNEL_B].tap_max; //Rs
	// Set wiper postion channel B to minimum
	max5389_wiper_pos_min(CHANNEL_B);			
} 

/**
 * @brief Increment wiper position
 * @param[in] channel Select channel
 * @param[in] val Step position
 * @param[out] None
 * @return wiper position
 **/
u16 max5389_inc_wiper_pos(max5389_channel_t channel, u16 val)
{	
	u16 size = val;
	
	if((wp_res[channel].current_pos + size) > wp_res[channel].tap_max)
		size = wp_res[channel].tap_max - wp_res[channel].current_pos;	
	
	MAX5389_PIN_WRITE(wp_pins[channel].PinUD, UP);  // set up control input
	MAX5389_PIN_WRITE(wp_pins[channel].PinCS, LOW);  // select chip
	for(i = 0; i < size; i++)
	{
		MAX5389_PIN_WRITE(wp_pins[channel].pinINC, LOW);  // set pin INCA to Low
		DELAY_NS(25);           // delay 25 nanosecond (tIL)
		MAX5389_PIN_WRITE(wp_pins[channel].pinINC, HIGH); // set pin INCA to High
		DELAY_NS(25);            // delay 25 nanosecond (tIH)			
	}
	DELAY_NS(50);  // delay 50 nanosecond (tIW)
	MAX5389_PIN_WRITE(wp_pins[channel].PinCS, HIGH);  // unselect chip	
	wp_res[channel].current_pos = wp_res[channel].current_pos + size;
	
	return wp_res[channel].current_pos;
}

/**
 * @brief Decrement wiper position
 * @param[in] channel Select channel
 * @param[in] val Step position
 * @param[out] None
 * @return wiper position
 **/
u16 max5389_dec_wiper_pos(max5389_channel_t channel, u16 val)
{
	u16 size = val;
	
	if(wp_res[channel].current_pos < size)
		size = wp_res[channel].current_pos;
	
	MAX5389_PIN_WRITE(wp_pins[channel].PinUD, DOWN);  // set down control input
	MAX5389_PIN_WRITE(wp_pins[channel].PinCS, LOW);  // select chip
	for(i = 0; i < size; i++)
	{
		MAX5389_PIN_WRITE(wp_pins[channel].pinINC, LOW);  // set pin INCA to Low
		DELAY_NS(25)            // delay 25 nanosecond (tIL)
		MAX5389_PIN_WRITE(wp_pins[channel].pinINC, HIGH); // set pin INCA to High
		DELAY_NS(25)            // delay 25 nanosecond (tIH)		
	}
	DELAY_NS(50);  // delay 50 nanosecond (tIW)
	MAX5389_PIN_WRITE(wp_pins[channel].PinCS, HIGH);  // unselect chip	
	wp_res[channel].current_pos = wp_res[channel].current_pos - size;
	
	return wp_res[channel].current_pos;
}

/**
 * @brief Set a specific wiper position
 * @param[in] channel Select channel
 * @param[in] val Wiper position
 * @param[out] None
 * @return wiper position
 **/
u16 max5389_set_wiper_pos(max5389_channel_t channel, u16 val)
{
	if(wp_res[channel].current_pos == val)
		return wp_res[channel].current_pos;
	else if(wp_res[channel].current_pos < val)
		return max5389_inc_wiper_pos(channel, val - wp_res[channel].current_pos);
	else 
		return max5389_dec_wiper_pos(channel, wp_res[channel].current_pos - val);	
}

/**
 * @brief Get current wiper position
 * @param[in] channel Select channel
 * @param[out] None
 * @return wiper position
 **/ 
u16 max3589_get_wiper_pos((max5389_channel_t channel)
{
	return wp_res[channel].current_pos;
}

/**
 * @brief Set a specific wiper resistance
 * @param[in] channel Select channel
 * @param[in] val Wiper resistance
 * @param[out] None
 * @return wiper resistance
 **/
 f32 max5389_set_wiper_res(max5389_channel_t channel, f32 val)
 {
	u16 pos = res_to_pos(channel, val);
	
	pos = max5389_set_wiper_pos(channel, pos);
	return pos_to_res(channel, pos);
 }
 
/**
 * @brief Get current wiper resistance
 * @param[in] channel Select channel
 * @param[out] None
 * @return wiper position
 **/
 
f32 max3589_get_wiper_res((max5389_channel_t channel)
{
	return pos_to_res(channel, wp_res[channel].current_pos);
}

/**
 * @brief Set wiper position to minimum
 * @param[in] channel Select channel
 * @param[out] None
 * @return wiper position
 **/
u16 max5389_wiper_pos_min(max5389_channel_t channel)
{
	MAX5389_PIN_WRITE(wp_pins[channel].PinUD, DOWN);  // set down control input
	MAX5389_PIN_WRITE(wp_pins[channel].PinCS, LOW);  // select chip
	for(i = 0; i < wp_res[channel].tap_max; i++)
	{
		MAX5389_PIN_WRITE(wp_pins[channel].pinINC, LOW);  // set pin INCA to Low
		DELAY_NS(25)            // delay 25 nanosecond (tIL)
		MAX5389_PIN_WRITE(wp_pins[channel].pinINC, HIGH); // set pin INCA to High
		DELAY_NS(25)            // delay 25 nanosecond (tIH)		
	}
	DELAY_NS(50);  // delay 50 nanosecond (tIW)
	MAX5389_PIN_WRITE(wp_pins[channel].PinCS, HIGH);  // unselect chip	
	wp_res[channel].current_pos = 0;
	
	return wp_res[channel].current_pos;
}

/**
 * @brief Set wiper position to maximum
 * @param[in] channel Select channel
 * @param[out] None
 * @return wiper position
 **/
u16 max5389_wiper_pos_max(max5389_channel_t channel)
{
	MAX5389_PIN_WRITE(wp_pins[channel].PinUD, UP);  // set down control input
	MAX5389_PIN_WRITE(wp_pins[channel].PinCS, LOW);  // select chip
	for(i = 0; i < wp_res[channel].tap_max; i++)
	{
		MAX5389_PIN_WRITE(wp_pins[channel].pinINC, LOW);  // set pin INCA to Low
		DELAY_NS(25)            // delay 25 nanosecond (tIL)
		MAX5389_PIN_WRITE(wp_pins[channel].pinINC, HIGH); // set pin INCA to High
		DELAY_NS(25)            // delay 25 nanosecond (tIH)		
	}
	DELAY_NS(50);  // delay 50 nanosecond (tIW)
	MAX5389_PIN_WRITE(wp_pins[channel].PinCS, HIGH);  // unselect chip	
	wp_res[channel].current_pos = wp_res[channel].tap_max;
	
	return wp_res[channel].current_pos;
}

 
/******************* Copyright(c) 2023 *****END OF FILE****/
