#include "elevator.h"

int lastFloor = BETWEEN_FLOORS;
elev_motor_direction_t lastDirection = DIRN_STOP;
elev_motor_direction_t currentDirection = DIRN_STOP;

void elevator_update_button_signals(int buttonSignals[][N_BUTTONS]) {
  for (int floor = 0; floor < N_FLOORS; floor++) {
    buttonSignals[floor][BUTTON_COMMAND] = elev_get_button_signal(BUTTON_COMMAND,floor);
    if (floor == N_FLOORS - 1) buttonSignals[floor][BUTTON_CALL_UP] = 0;
    else buttonSignals[floor][BUTTON_CALL_UP] = elev_get_button_signal(BUTTON_CALL_UP, floor);
  	if (floor == 0) buttonSignals[floor][BUTTON_CALL_DOWN] = 0;
    else buttonSignals[floor][BUTTON_CALL_DOWN] = elev_get_button_signal(BUTTON_CALL_DOWN, floor);
  }
}

void elevator_set_direction(elev_motor_direction_t dirn) {
	elev_set_motor_direction(dirn);
	//direction får kun verdiene DIRN_UP eller DIRN_DOWN. Ved stopp vil direction inneholde sist kjente heisretning.
	if (dirn != DIRN_STOP) lastDirection = dirn;
	currentDirection = dirn;
}
