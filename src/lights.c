#include "driver.h"
#include "buttons.h"

static int lights[N_FLOORS][N_BUTTONS] = {};

void lights_clear_floor(int floor) {
  for (int button = 0; button < N_BUTTONS; button++) {
    if (lights[floor][button] == 1) {
      lights[floor][button] = 0;
      if (!(floor == 0 && button == BUTTON_CALL_DOWN) && !(floor == N_FLOORS -1 && button == BUTTON_CALL_UP)) {
        elev_set_button_lamp(button, floor, 0);
      }
    }
  }
}

void lights_clear_all() {
  for (int floor = 0; floor < N_FLOORS; floor++) {
    lights_clear_floor(floor);
  }
}

void lights_update_from_button_signals() {
	for (int floor=0; floor < N_FLOORS; floor++) {
		for (int button=0; button < N_BUTTONS; button++) {
			if (buttons_get_signal(floor, button) && lights[floor][button] == 0) {
        lights[floor][button] = 1;
        elev_set_button_lamp(button, floor, 1);
      }
		}
	}
}
