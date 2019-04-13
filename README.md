# Heis Weiß v4

Prosjekt i faget TTK4235 Tilpassede datasystemer.
Mekket av Erlend Blomseth og Axel Tveiten Bech.

Heisen er optimalisert for å skrive minst mulig til og lese minst mulig fra heishardvaren :).


### Hvordan funker heisen?

Heisen er basert på en tilstandsmaskin. Noen av tilstandene har spesifiserte inn- og ut-aktiviteter, altså oppgaver som skal utføres på vei inn til eller på vei ut av en tilstand.
Inn- og ut-delene kjører kun én gang, henholdsvis før og etter tilstandsmaskinen havner i tilhørende interntilstand. Interntilstanden kjører i løkke inntil vi skal videre til neste tilstand.

Tilstandsmaskinen er bygget opp som følger med tilstander og inn-ut-aktiviteter:

* initialize (Initialisering av heisen, utføres én gang. Ingen inn- eller ut-aktiviteter)
* idle (Vent på ny bestilling, deretter gå til riktig tilstand. Ingen inn- eller ut-aktiviteter)
* go (Bestem retning og kjør mot en bestilling)
  * entry (På vei inn til go-tilstanden)
  * internal (Selve go-tilstanden)
* stay (Heisen har stoppet i en etasje for å håndtere en bestilling)
  * entry (På vei inn til stay-tilstanden)
  * internal (Selve stay-tilstanden)
  * exit (På vei ut av stay-tilstanden)
* emergency (Nødstopp. Ingen inn- eller ut-tilstander)
