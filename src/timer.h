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
