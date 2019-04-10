#include "driver/elev.h"
#include "elevator.h"

int lights[N_FLOORS][N_BUTTONS] = {0};

void lights_clear(int floor) {
  for (int button = 0; button < N_BUTTONS; button++) {
    if (lights[floor][button] == 1) {
      lights[floor][button] = 0;
      if (!(floor == 0 && button == BUTTON_CALL_DOWN) && !(floor == N_FLOORS -1 && button == BUTTON_CALL_UP)) elev_set_button_lamp(button, floor, 0);
    }
  }
}

void lights_clear_all() {
  for (int floor = 0; floor < N_FLOORS; floor++) {
    lights_clear(floor);
  }
}

void lights_update() {
	for (int floor=0; floor < N_FLOORS; floor++) {
		for (int button=0; button < N_BUTTONS; button++) {
			if (elevator_get_button_signal(floor, button) && lights[floor][button] == 0) {
        lights[floor][button] = 1;
        elev_set_button_lamp(button, floor, 1);
      }
		}
	}
}
