#include "driver/elev.h"

//Skrur av alle lys utenom stopplampa
void lights_clear_all() {
  for (int f = 0; f < N_FLOORS; f++) {
    elev_set_button_lamp(BUTTON_COMMAND, f, 0);
	if (f != N_FLOORS -1) elev_set_button_lamp(BUTTON_CALL_UP, f, 0);
	if (f != 0) elev_set_button_lamp(BUTTON_CALL_DOWN, f, 0);
  }
}

void lights_clear(int floor) {
  elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
  if (floor != N_FLOORS -1) elev_set_button_lamp(BUTTON_CALL_UP, floor, 0);
  if (floor != 0) elev_set_button_lamp(BUTTON_CALL_DOWN, floor, 0);
}
