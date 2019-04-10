#include "elevator.h"

int currentFloor = BETWEEN_FLOORS;
int lastFloor = BETWEEN_FLOORS;
elev_motor_direction_t currentDirection = DIRN_STOP;
elev_motor_direction_t lastDirection = DIRN_STOP;
elev_motor_direction_t lastDirectionBeforeStop = DIRN_STOP;

static int buttonSignals[N_FLOORS][N_BUTTONS];

void update_current_floor() {
  currentFloor = elev_get_floor_sensor_signal();
}

void elevator_update_button_signals() {
  for (int floor = 0; floor < N_FLOORS; floor++) {
    for (int button = 0; button < N_BUTTONS; button++) {
      if (!(floor == N_FLOORS - 1 && button == BUTTON_CALL_UP) && !(floor == 0 && button == BUTTON_CALL_DOWN)) buttonSignals[floor][button] = elev_get_button_signal(button,floor);
    }
  }
}

void elevator_set_direction(elev_motor_direction_t dirn) {
	elev_set_motor_direction(dirn);
	//direction får kun verdiene DIRN_UP eller DIRN_DOWN. Ved stopp vil direction inneholde sist kjente heisretning.
	if (dirn != DIRN_STOP) lastDirection = dirn;
	currentDirection = dirn;
}

int elevator_get_button_signal(elev_button_type_t button, int floor) {
  return buttonSignals[button][floor];
}
