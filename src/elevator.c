#include "elevator.h"

int lastFloor = BETWEEN_FLOORS;
int get_last_floor() { return lastFloor; }
void set_last_floor(int floor) { lastFloor = floor; }

int currentFloor = BETWEEN_FLOORS;
int get_current_floor() { return currentFloor; }
void set_current_floor(int floor) { currentFloor = floor; }

elev_motor_direction_t lastDirection = DIRN_STOP;
elev_motor_direction_t get_last_direction() { return lastDirection; }
void set_last_direction(elev_motor_direction_t dirn) { lastDirection = dirn; }

elev_motor_direction_t currentDirection = DIRN_STOP;
elev_motor_direction_t get_current_direction() { return currentDirection; }
void set_current_direction(elev_motor_direction_t dirn) { currentDirection = dirn; }

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
