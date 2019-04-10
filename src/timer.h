/** @file
  * @brief Timer
  */

/** @brief Starter klokka
*/
void timer_start();

/** @brief Sjekker og returnerer hvorvidt timeren har passert riktig tid
*
* @return Hvorvidt timeren er utløpt (ja/nei, 1/0)
*/
int timer_expired();
