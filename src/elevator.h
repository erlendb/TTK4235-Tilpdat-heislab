/*! @file
  * @brief Heis
  */

#include "driver/elev.h"

#define BETWEEN_FLOORS -1

/*! Sist kjente etasje, utenom -1 (mellom to etasjer).
  * Inneholder alltid en positiv etasjeverdi.
  * Unntak: Blir initialisert til -1.
  */
extern int lastFloor;

/*! Sist kjente kjøreretning for heisen, utenom DIRN_STOP.
  * Inneholder alltid DIRN_UP eller DIRN_DOWN.
  * Unntak: Blir initialisert til DIRN_STOP.
  */
extern elev_motor_direction_t lastDirection;

/*! Nåværende kjøreretning for heisen, inkludert DIRN_STOP.
  * Kan inneholde DIRN_STOP.
  * Initialiseres til DIRN_STOP.
  */
extern elev_motor_direction_t currentDirection;

/*! @brief Henter knappesignaler og oppdaterer array.
  * @param[out] buttonSignals Knappetrykksignalene lagres her.
  */
void elevator_update_button_signals(int buttonSignals[][N_BUTTONS]);

/*! @brief Sender motoren i riktig retning, og oppdaterer direction- og lastDirection-variabler.
  * @param[in] dirn Ny motorretning
  */
void elevator_set_direction(elev_motor_direction_t dirn);
