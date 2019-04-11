/** @file
  * @brief Heis
  * @details Mellomsteg mellom heisdriveren og tilstandsmaskin/moduler. Globale variabler knyttet til heisen.
  */

#include "driver.h"

#define BETWEEN_FLOORS -1

/** Etasjen som heisen er i nå, -1 dersom heisen er mellom to etasjer.
  *
  * Oppdateres med update_current_floor().
  */
extern int currentFloor;

/** Sist kjente etasje heisen var i.
  *
  * Denne er aldri -1.
  * Dersom heisen er mellom to etasjer svarer verdien til forrige etasje heisen var i.
  * Dersom heisen er i en etasje svarer verdien til nåværende etasje.
  */
extern int lastFloor;

/** Kjøreretningen heisen har nå, DIRN_STOP hvis heisen står stille.
  */
extern elev_motor_direction_t currentDirection;

/** Heisens sist kjente kjøreretning.
  *
  * Verdien svarer til forrige kjøreretning dersom heisen står i ro.
  * Verdien svarer til nåværende kjøreretning dersom heisen er i bevegelse.
  */
extern elev_motor_direction_t lastDirection;

/** Heisens siste kjøreretning før nødstopp.
  *
  * Ved første nødstopp mellom to etasjer: verdien settes til heisens forrige kjøreretning.
  * Ved flere påfølgende nødstopp uten at heisen har vært innom en etasje vil ikke verdien endres.
  * Ved første tur innom en etasje etter nødstopp vil verdien resettes til DIRN_STOP.
  */
extern elev_motor_direction_t lastDirectionBeforeStop;

/** @brief Henter etasjesignal fra heisdriveren og oppdaterer global currentFloor.
  */
void update_current_floor();

/** @brief Henter knappesignaler fra heisdriveren og oppdaterer lokalt array.
  */
void elevator_fetch_button_signals();

/** @brief Sender motoren i gitt retning, og oppdaterer globale currentDirection og lastDirection.
  *
  * @param[in] dirn Ny motorretning
  */
void elevator_set_direction(elev_motor_direction_t dirn);

/** @brief Returnerer hvorvidt en gitt knapp i en gitt etasje er trykket inn (ja/nei).
  *
  * @param[in] button Knappen vi spør om er trykket inn
  * @param[in] floor  Etasjen som knappen vi vil spørre etter ligger i.
  * @return Hvorvidt gitt knapp er trykket inn (ja/nei, 1/0).
  */
int elevator_get_button_signal(elev_button_type_t button, int floor);
