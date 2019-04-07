/*! @file
  * @brief Køsystem
  */

extern int lastFloor;
extern elev_motor_direction_t lastDirection;
extern elev_motor_direction_t direction;
extern int queue[];

//Bestillinger
typedef enum orders {
  ORDER_NO = -1,
  ORDER_UP = 0,
  ORDER_DOWN = 1,
  ORDER_ALL = 2
} order_t;

/*! @brief Oppdaterer køen ut fra ny bestilling og eksisterende bestillinger.
  * @details
      order = -1 (ingen bestilling) overskriver etasjen.
  * @param[in]  floor Etasjen der bestillinger skal oppdateres.
  * @param[in]  order Den nye bestillingen i gitt etasje.
  */
void queue_add(int floor, int order);

/*! @brief Sletter alle bestillinger fra køen.
  */
void queue_clear_all();


void queue_clear(int floor);

/*! @brief Sjekker om vi skal stoppe i gitt etasje, med hensyn til retningen heisen er på vei i
  * @param[in]  floor Etasjen vi skal sjekke om vi skal stoppe i
  *
  * @return Hvorvidt vi skal stoppe, 1 (stopp) eller 0 (ikke stopp).
  */
int queue_stop(int floor);

/*! @brief Teller antall etasjer som har bestillinger
  *
  * @return Antall etasjer med bestillinger.
  */
int queue_count();

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
int queue_get(int floor);

/*! @brief Sjekker alle knapper og oppdaterer kø og bestillingslys deretter.
  */
void queue_check_buttons();

void queue_update();

/*! @brief Sender motoren i riktig retning, og oppdaterer direction- og lastDirection-variabler.
  * @param[in] dirn Ny motorretning
  */
void set_direction(elev_motor_direction_t dirn);
