/*! @file
  * @brief Timer
  */

/*! @brief Starter klokka
*/
void timer_start();

/*! @brief Sjekker og returnerer hvorvidt timeren har passert riktig tid
*
* @return Hvorvidt vi har passert grensetiden, 1 (passert) eller 0 (ikke passert)
*/
int timer_check();

/*! @brief Deaktiverer klokka
*/
void timer_deactivate();

/*! @brief Sjekker og returnerer hvorvidt klokka er aktivert
*
* @return Hvorvidt timeren er aktivert, 1 (ja) eller 0 (nei)
*/
int timer_is_activated();
