/*! @file
  * @brief Køsystem
  */

/*! Enum med ordre. Alle verdier bortsett fra ORDER_NONE samsvarer med verdiene fra tag_elev_lamp_type (elev.h).
  */
typedef enum orders {
	ORDER_NONE = -1,               ///< Ingen bestilling.
	ORDER_UP = BUTTON_CALL_UP,     ///< Bestilling i oppoverretning.
	ORDER_DOWN = BUTTON_CALL_DOWN, ///< Bestilling i nedoverretning.
	ORDER_ALL = BUTTON_COMMAND     ///< Bestilling inni heisen eller bestilling i begge retninger.
} order_t;

/*! @brief      Oppdaterer køen ut fra ny bestilling og eksisterende bestillinger.
  * @details    order = -1 (ingen bestilling) overskriver etasjen.
  * @param[in]  floor Etasjen der bestillinger skal oppdateres.
  * @param[in]  order Den nye bestillingen i gitt etasje.
  */
void queue_add(int floor, order_t order);

/*! @brief Sletter alle bestillinger fra køen.
  */
void queue_clear_all();

/*! @brief Sletter alle bestillinger i gitt etasje
  * @param[in] floor Etasjen bestillinger skal slettes fra
*/
void queue_clear(int floor);

/*! @brief Sjekker om vi skal stoppe i gitt etasje med gitt retning på heisen
  * @param[in]  floor Etasjen vi skal sjekke om vi skal stoppe i
  * @param[in]  direction Retningen vi skal sjekke om vi skal stoppe i
  *
  * @return Hvorvidt vi skal stoppe, 1 (stopp) eller 0 (ikke stopp).
  */
int queue_should_stop(int floor, elev_motor_direction_t direction);

/*! @brief Teller antall etasjer som har bestillinger
  *
  * @return Antall etasjer med bestillinger.
  */
int queue_any_orders();

/*! @brief Sjekker hvorvidt det finnes bestillinger over gitt etasje
  * @param[in] floor Etasjen vi skal sjekke bestillinger over.
  *
  * @return Hvorvidt det finnes bestillinger over, 1 (ja) eller 0 (nei)
  */
int queue_check_above(int floor);

/*! @brief Sjekker hvorvidt det finnes bestillinger under gitt etasje
  * @param[in] floor Etasjen vi skal sjekke bestillinger under.
  *
  * @return Hvorvidt det finnes bestillinger under, 1 (ja) eller 0 (nei)
  */
int queue_check_below(int floor);

/*! @brief Henter og returnerer bestillingen i gitt etasje.
  * @param[in] floor Etasjen vi skal hente bestillingen fram
  *
  * @return Bestillingsverdien for etasjen: NO_ORDER (ingen bestilling), ORDER_UP (bestilling oppover), ORDER_DOWN (bestilling nedover) eller ORDER_ALL (bestilling inni heisen eller bestilling i begge retninger).
  */
order_t queue_get_order(int floor);

/*! @brief Oppdaterer køen med bestillinger fra knappetrykkarray
  * @param[in] buttonSignals Array med nye knappetrykk
  */
void queue_update();
