# Heis Weiß

Heisprosjekt i faget TTK4235 Tilpassede datasystemer.

Mekket av Erlend Blomseth og Axel Tveiten Bech.

## Heis v4

**v4.4**: Oppdaterte heisdriveren til den nye fra https://github.com/erlendb/TTK4235-tilpdat-simulatorheis

**v4.3**: Testet og funker i simulator og på sal. FAT 11. april uten feil.

Optimalisert for minst mulig skriving til og lesing fra hardvaren/simulatoren.


### Hvordan bytte mellom simulator og heisen på sal

For å kompilere til bruk i simulator: bruk `#define SIMULATOR` øverst i *driver/elev.c* og *driver/elev.h*.

For å kompilere til bruk på sal: bruk `#define SAL` øverst i *driver/elev.c* og *driver/elev.h*.


### Hvordan funker heisen?
Tilstandsmaskin med spesifiserte inn- og ut-aktiviteter, altså oppgaver som skal utføres på vei inn til eller på vei ut av en tilstand.
Inn- og ut-delene kjører kun én gang, henholdsvis før og etter tilstandsmaskinen havner i tilhørende interntilstand. Interntilstanden kjører i løkke inntil vi skal videre til neste tilstand.

Tilstandsmaskinen er bygget opp som følger:

* start (Initialisering av heisen, utføres én gang)
* idle (Vent på ny bestilling, deretter gå til riktig tilstand. Ingen inn- eller ut-aktiviteter)
* go (Bestem retning og kjør mot en bestilling)
  * entry (På vei inn til go-tilstanden)
  * internal (Selve go-tilstanden)
* stay (Heisen har stoppet i en etasje for å utføre en bestilling)
  * entry (På vei inn til stay-tilstanden)
  * internal (Selve stay-tilstanden)
  * exit (På vei ut av stay-tilstanden)
* emergency (Nødstopp. Ingen inn- eller ut-tilstander)
