#include "driver/elev.h"
#include "elevator.h"

void lights_clear_all() {
  for (int floor = 0; floor < N_FLOORS; floor++) {
    elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
  	if (floor != N_FLOORS -1 ) elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
  	if (floor != 0) elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
  }
}

void lights_clear(int floor) {
  elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
  if (floor != N_FLOORS -1) elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
  if (floor != 0) elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
}

void lights_update(int buttonSignals[][N_BUTTONS]) {
	for (int floor=0; floor < N_FLOORS; floor++) {
		for (int button=0; button < N_BUTTONS; button++) {
			if (buttonSignals[floor][button]) elev_set_button_lamp(button, floor, 1);
		}
	}
}
