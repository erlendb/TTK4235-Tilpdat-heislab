#include "elevator.h"

int lastFloor = -1;
elev_motor_direction_t lastDirection = DIRN_STOP;
elev_motor_direction_t currentDirection = DIRN_STOP;

void elevator_update_button_signals(int buttonSignals[][N_BUTTONS]) {
  for (int floor = 0; floor < N_FLOORS; floor++) {
  	int button_command;
  	int button_call_up = 0;
  	int button_call_down = 0;

    button_command = elev_get_button_signal(BUTTON_COMMAND,floor);
    if (floor != N_FLOORS - 1) button_call_up = elev_get_button_signal(BUTTON_CALL_UP, floor);
  	if (floor != 0) button_call_down = elev_get_button_signal(BUTTON_CALL_DOWN, floor);

    buttonSignals[floor][BUTTON_COMMAND] = button_command;
    buttonSignals[floor][BUTTON_CALL_UP] = button_call_up;
    buttonSignals[floor][BUTTON_CALL_DOWN] = button_call_down;
  }
}

void elevator_set_direction(elev_motor_direction_t dirn) {
	elev_set_motor_direction(dirn);
	//direction får kun verdiene DIRN_UP eller DIRN_DOWN. Ved stopp vil direction inneholde sist kjente heisretning.
	if (dirn != DIRN_STOP) lastDirection = dirn;
	currentDirection = dirn;
}
