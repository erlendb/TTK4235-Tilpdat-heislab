#include "elevator.h"

int buttonSignals[N_FLOORS][N_BUTTONS];

void elevator_update_button_signals() {
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
