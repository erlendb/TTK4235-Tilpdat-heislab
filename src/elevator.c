#include "elevator.h"

static int buttonSignals[N_FLOORS][N_BUTTONS];

void elevator_fetch_button_signals() {
  for (int floor = 0; floor < N_FLOORS; floor++) {
    for (int button = 0; button < N_BUTTONS; button++) {
      if (!(floor == N_FLOORS - 1 && button == BUTTON_CALL_UP) && !(floor == 0 && button == BUTTON_CALL_DOWN)) {
        buttonSignals[floor][button] = elev_get_button_signal(button,floor);
      }
    }
  }
}

int elevator_get_button_signal(elev_button_type_t button, int floor) {
  return buttonSignals[button][floor];
}
