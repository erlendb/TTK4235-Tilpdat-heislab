#include "driver/elev.h"

//Skrur av alle lys utenom stopplampa
void lights_clear() {
  for (int f = 0; f < N_FLOORS; f++) {
    for (int b = 0; b < N_BUTTONS; b++) {
      elev_set_button_lamp(b, f, 0);
    }
  }
}
