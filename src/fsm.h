/** @file
  * @brief Tilstandsmaskin med tilhørende greier
  */

/** Enum med heistilstander
  *
  * Dette er tilstandende tilstandsmaskinen kan gå inn i.
  * Initialiseringstilstand finnes også, men er ikke tatt med her. Heisen er kun i initialiseringstilstanden én gang i begynnelsen av programmet,
  */
typedef enum states {
  IDLE,       ///< I IDLE-tilstand venter heisen på ny bestilling.
  GO,         ///< I GO-tilstand bestemmer heisen retning og går mot en bestilling.
  STAY,       ///< I STAY-tilstand står heisen i en etasje og håndterer en bestilling.
  EMERGENCY,  ///< I EMERGENCY-tilstand går heisen i nødmodus.
} state_t;

/** Enum med actions for tilstandene.
  *
  * INTERNAL-action kan sies å være "selve tilstanden". ENTRY og EXIT utføres én gang hver (for hver tilstand), henholdsvis på vei inn i og ut av tilstanden.
  * Tilstander kan ha ingen, én eller flere.
  */
typedef enum state_actions {
  ENTRY,    ///< ENTRY-action utføres på vei inn i en ny tilstand.
  INTERNAL, ///< INTERNAL-action utføres gjentatte ganger så lenge vi holder oss i en tilstand.
  EXIT,     ///< EXIT-action utføres på vei ut av en tilstand.
} state_action_t;

/** @brief Henter etasjesignal fra heisdriveren og oppdaterer global currentFloor.
  */
void fsm_update_current_floor();

/** @brief Nøytraliserer lastDirectionBeforeStop-variablen (setter den til 0-ish).
  *
  * Variablen blir satt til DIRN_STOP, en verdi og en retning som ikke gir mening at en aktiv lastDirectionBeforeStop-variabel har.
  */
void fsm_unset_last_direction_before_stop();

/** @brief Sjekker og returnerer hvorvidt lastDirectionBeforeStop har fått en verdi annet enn initialiserings- og unset-verdien.
  *
  * @return Hvorvidt variablen har en aktiv verdi.
  */
elev_motor_direction_t fsm_last_direction_before_stop_is_set();

/** @brief Sender motoren i gitt retning, og oppdaterer retningsvariabler.
  *
  * @param[in] dirn Ny motorretning
  */
void fsm_set_elevator_direction(elev_motor_direction_t dirn);

/** @brief Henter den neste tilstanden tilstandsmaskinen skal inn i
  *
  * @return Neste tilstand
  */
state_t fsm_get_next_state();

/** @brief Oppdaterer tilstandsmaskinsvariablene som angår neste tilstand og neste action.
  *
  * @param[in] state Neste tilstand
  * @param[in] action Neste action
  */
void fsm_transition(state_t state, state_action_t action);

/** @brief Kjører initialiseringstilstanden.
  *
  * Kjører heisen ned til nærmeste etasje hvis heisen startes mellom to etasjer. Heisen blir stående hvis den startes i en etasje.
  * Går deretter til idle-tilstand.
  */
void fsm_state_initialize();

/** @brief Kjører idle-tilstand.
  *
  * Venter på ny bestilling hvis køen er tom.
  * Går til stay-tilstand hvis vi har en bestilling i samme etasje som heisen står i.
  * Går til go-state hvis vi har bestilling et annet sted.
  */
void fsm_state_idle();

/** @brief Kjører go-tilstand.
  *
  * Sender heisen i retning av neste bestilling. Stopper hvis vi havner i en etasje med en bestilling som skal håndteres.
  */
void fsm_state_go();

/** @brief Kjører stay-state
  *
  * Håndterer en bestilling i en etasje.
  */
void fsm_state_stay();

/** @brief Kjører stop-state
  *
  * Fikser alt som skal fikses ved nødstopp.
  */
void fsm_state_emergency();
