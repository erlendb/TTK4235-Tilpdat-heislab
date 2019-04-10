/*! @file
  * @brief Heis
  */

#include "driver/elev.h"

#define BETWEEN_FLOORS -1

/*! Sist kjente etasje, utenom -1 (mellom to etasjer).
  * Inneholder alltid en positiv etasjeverdi.
  * Unntak: Blir initialisert til -1.
  */
int get_last_floor();
void set_last_floor(int floor);

int get_current_floor();
void set_current_floor(int floor);
void update_current_floor();

/*! Sist kjente kjøreretning for heisen, utenom DIRN_STOP.
  * Inneholder alltid DIRN_UP eller DIRN_DOWN.
  * Unntak: Blir initialisert til DIRN_STOP.
  */
elev_motor_direction_t get_last_direction();
void set_last_direction(elev_motor_direction_t dirn);

/*! Nåværende kjøreretning for heisen, inkludert DIRN_STOP.
  * Kan inneholde DIRN_STOP.
  * Initialiseres til DIRN_STOP.
  */
elev_motor_direction_t get_current_direction();
void set_current_direction(elev_motor_direction_t dirn);

elev_motor_direction_t get_last_direction_before_stop();
void set_last_direction_before_stop(elev_motor_direction_t dirn);

/*! @brief Henter knappesignaler og oppdaterer array.
  * @param[out] buttonSignals Knappetrykksignalene lagres her.
  */
void elevator_update_button_signals(int buttonSignals[][N_BUTTONS]);

/*! @brief Sender motoren i riktig retning, og oppdaterer direction- og lastDirection-variabler.
  * @param[in] dirn Ny motorretning
  */
void elevator_set_direction(elev_motor_direction_t dirn);
