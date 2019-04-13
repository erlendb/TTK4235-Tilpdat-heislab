/** @file
  * @brief Lesing av nngangssignaler/knappetrykk
  */

#include "driver.h"

/** @brief Henter knappesignaler fra heisdriveren og oppdaterer lokalt array.
  */
void buttons_fetch_signals();

/** @brief Returnerer hvorvidt en gitt knapp i en gitt etasje er trykket inn (ja/nei).
  *
  * Returnerer egentlig den lagrede verdien for knappen. Knappetrykksignaler bør hentes med buttons_fetch_signals() først.
  *
  * @param[in] button Knappen vi spør om er trykket inn
  * @param[in] floor  Etasjen som knappen vi vil spørre etter ligger i.
  * @return Hvorvidt gitt knapp er trykket inn (ja/nei, 1/0).
  */
int buttons_get_signal(elev_button_type_t button, int floor);
