#include <reg51.h>  
#define ELEVATOR_REQUEST P3  
#define FLOOR_SELECTION P1  
#define LEDS P0  
#define DISPLAY P2 
 
// array of digits of seven segment display (from '1' to '8') according to assignment 7 
unsigned char digitsDISPLAY[] = {0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01};

// delay function according to example 7-7 
// AT89S52 / DS89C420 with XTAL = 11.0592 MHz 
//void delay(unsigned int time){
//	unsigned int x, y;
//	for(x = 0; x < time; x++)
//		for(y = 0; y < 1275; y++); 
//}

// function get just a floor number from 1 to 8, 
// then shifts 1 to left by (floor - 1) positions,
// so sets one led corresponding to selected floor to 1 
void showLEDS(unsigned char number){
	LEDS = (1 << (number - 1));
}

// function get just a floor number from 1 to 8, 
// then uses number as index of array and substract 1, because array index starts from 0, 
// so defines DISPLAY (P2) value from pre-defined digit patterns of digitsDISPLAY[] array 
void showDISPLAY(unsigned char number){
	DISPLAY = digitsDISPLAY[number - 1];
}

void main(void){
	unsigned char current_floor = 1;
	unsigned char requested_floor = 0;
	unsigned char selected_floor = 0;
	unsigned char i = 0; // iteration steps
	
	LEDS = 0x00; // output port 0 - 8 red leds
	DISPLAY = 0x00; // output port 2 - seven segment display
	ELEVATOR_REQUEST = 0xFF; // input port 3 - 8 elevator request switches
	FLOOR_SELECTION = 0xFF; // input port 1 - 8 floor selection switches
	
	while(1){
		// check if all of switches are not activated,
		// just display initial data of current floor to leds and seven segment display
		if(ELEVATOR_REQUEST == 0xFF && FLOOR_SELECTION == 0xFF){
			showDISPLAY(current_floor); // update display
			showLEDS(current_floor); // update leds
		}
		
		// check for elevator request switches activation
		if(ELEVATOR_REQUEST != 0xFF){
			// iterate through each bit in ELEVATOR_REQUEST
			for(i = 0; i < 8; i++){
				// check if bit isn't set, so one of the ELEVATOR_REQUEST switch is activated
				if(!(ELEVATOR_REQUEST & (1 << i))){
					requested_floor = i + 1; // set requested floor according to activated switch
					break; // exit loop when request is found
				}
			}
			current_floor = requested_floor; // update current floor
			showDISPLAY(current_floor); // update display
			showLEDS(current_floor); // update leds
		}
		
		// check for floor selection switches activation
		if(FLOOR_SELECTION != 0xFF){
			// iterate through each bit in FLOOR_SELECTION
			for(i = 0; i < 8; i++){
				// check if bit isn't set, so one of the FLOOR_SELECTION switch is activated
				if(!(FLOOR_SELECTION & (1 << i))){
					selected_floor = i + 1; // set selected floor according to activated switch
					break; // exit loop when selection is found
				}
			}
			
			// move step by step from current floor to selected floor
			while(current_floor != selected_floor){
				// ternary operator with incrementing and decrementing current floor in relation to selected floor
				(current_floor < selected_floor) ? current_floor++ : current_floor--;
				showDISPLAY(current_floor); // update display
				showLEDS(current_floor); // update leds
			}
		}
	}
}