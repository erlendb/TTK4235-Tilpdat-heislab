#include "driver/elev.h"

//Skrur av alle lys utenom stopplampa
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
