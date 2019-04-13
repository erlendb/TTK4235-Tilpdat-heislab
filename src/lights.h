/** @file
  * @brief Bestillingslys
  *
  * Denne modulen avhenger av å hente lagrede knappetrykk i buttons-modulen.
  * Det er mulig å bruke denne modulen uten å være avhengig av buttons-modulen ved å bytte ut buttons_get_signal()-kallene med å kalle elev_get_button_signal() fra elev.h
  */

/** @brief Skrur av alle bestillingslys i gitt etasje
  *
  * @param[in] floor Etasje der lysene skal skrues av
  */
void lights_clear_floor(int floor);

/** @brief Skrur av alle bestillingslys
  */
void lights_clear_all();

/** @brief Oppdaterer bestillingslys ut fra nye knappetrykkarray i buttons-modulen.
  * Skrur på nye lys dersom det har kommet en ny bestilling.
  */
void lights_update_from_button_signals();
