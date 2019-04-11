/** @file
  * @brief Bestillingslys
  */

/** @brief Skrur av alle bestillingslys
  */
void lights_clear_all();

/** @brief Skrur av alle bestillingslys i gitt etasje
  *
  * @param[in] floor Etasje der lysene skal skrues av
  */
void lights_clear_floor(int floor);

/** @brief Oppdaterer bestillingslys ut fra nye knappetrykkarray i elevator-modulen.
  * Nytter elevator_get_button_signal(), og skrur på nye lys dersom det har kommet en ny bestilling.
  */
void lights_update_from_button_signals();
