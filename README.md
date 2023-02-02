# Digipot library
Digipot is a library that provides functions of a digital potentiometer IC.
This library uses MAX5389 to set resistance.
The following are some of the applications where the digital potentiometers can be used:
 - To adjust the volume in stereo and other appliances
 - To adjust the brightness and contrast in LEDs
 - Programmable Voltage regulator
 - Sensor auto referencing circuits
 - To vary the resistance in an analog circuit
 - Automatic Gain Control
 - Used for sensor trimming and calibration instrumentation.
 - Level adjustments in automotive electronics
 - Programmable power supplies, filters, time constants or delay value.
## Functionality
The functions available in the library include
```c
max5389_init(hl_res_a, resolution_a, wiper_res_a, hl_res_b, resolution_b, wiper_res_b)	// init max5389 module.
max5389_set_wiper_res(CHANNEL_A, 4000)		// set a resistane to channel A.
max3589_get_wiper_res(CHANNEL_A)		// read the current resistance of channel A.
max5389_set_wiper_pos(CHANNEL_B, 40)		// set a position to channel B
max3589_get_wiper_pos(CHANNEL_B)		// read the current position of channel B.
```
## Examples
- `main.c`- example of using Digipot library. 
## Block diagram
![Alt Text](doc/Block_Diagram.png?raw=true)
## Sequence diagram
![Alt Text](doc/Sequence_Diagram.png?raw=true)
## Flow chart
![Alt Text](doc/Digiport_flowchart.png?raw=true)
