#include "driver/elev.h"

extern int lastFloor;
extern elev_motor_direction_t lastDirection;
extern elev_motor_direction_t direction;

void elevator_update_button_signals(int buttonSignals[][N_BUTTONS]);


/*! @brief Sender motoren i riktig retning, og oppdaterer direction- og lastDirection-variabler.
  * @param[in] dirn Ny motorretning
  */
void elevator_set_direction(elev_motor_direction_t dirn);
