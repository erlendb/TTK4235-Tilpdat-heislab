/*! @file
  * @brief Bestillingslys
  */

/*! @brief Skrur av alle bestillingslys i gitt etasje
  * @param[in] Etasje der lysene skal skrues av
  */
void lights_clear(int floor);

/*! @brief Skrur av alle bestillingslys i gitt etasje
  */
void lights_clear_all();

/*! @brief Oppdaterer bestillingslys i henhold til innhentede knappetrykksignaler
  * @param[in] buttonSignals Array med knappesignaler
  */
void lights_update();
