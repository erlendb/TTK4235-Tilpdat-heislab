/*! @file
  * @brief Heis
  */

#include "driver/elev.h"

#define BETWEEN_FLOORS -1

extern int currentFloor;
extern int lastFloor;
extern elev_motor_direction_t currentDirection;
extern elev_motor_direction_t lastDirection;
extern elev_motor_direction_t lastDirectionBeforeStop;

void update_current_floor();

/*! @brief Henter knappesignaler og oppdaterer array.
  * @param[out] buttonSignals Knappetrykksignalene lagres her.
  */
void elevator_update_button_signals();

/*! @brief Sender motoren i riktig retning, og oppdaterer direction- og lastDirection-variabler.
  * @param[in] dirn Ny motorretning
  */
void elevator_set_direction(elev_motor_direction_t dirn);

int elevator_get_button_signal(elev_button_type_t button, int floor);
