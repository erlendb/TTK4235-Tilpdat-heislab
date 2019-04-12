# Heis Weiß

Prosjekt i faget TTK4235 Tilpassede datasystemer.


## Heis utgave 2

Testet og funker i simulator og på sal. FAT gjennomført uten feil 11/4-19. FAT-testet versjon har tag v4.3.

Optimalisert for minst mulig skriving til og lesing fra hardvaren/simulatoren.


### Make

For å kompilere til bruk på sal: sett SIMULATOR-flagget øverst i Makefile og øverst i driver.h til 0.

For å kompilere til bruk i simulator (https://github.com/TTK4145/Simulator-v2): sett de to nevnte SIMULATOR-flaggene til 1.

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


### Bør kanskje fikses

* Misbruk av notasjon i lastDirectionBeforeStop. Kanskje bruke set_lastDirectionBeforeStop(dirn) og unset_lastDirectionBeforeStop()?
* Flytte lastDirectionBeforeStop-logikk inn i set_direction.
* go-internal etter optimalisering: leter kun etter bestilling idet heisen ankommer etasje. Om noen bestiller heisen mens den er på vei gjennom etasjen vil den ikke stoppe. Bra? Problem? Vet ikke helt. Fort gjort å fikse. 11/4: Er dette endret igjen?
* Endre køsystemet fra array med én dimensjon til to dimensjoner med alle knapper representert? Kan potensielt gjøre at programmet itererer gjennom bestillinger/knappetrykk færre ganger, og at vi slipper å lagre knappetrykk, lamper og kø i tre forskjellige arrayer.
* Dokumentere #define?
* Gjøre om globale elevator-variabler til lokale fsm-variabler.
* Fjerne mulighet for simulator før levering. Det vil gjøre ting litt ryddigere :).



## Heis utgave 1

Siste versjon har tag v3. Testet og fungerer på sal.

FAT stort sett ok, tror det bare er én bug:
Heisen kjører feil retning etter følgende sekvens: håndter en bestilling i en etasje -> send heisen en vei -> nødstopp før heisen kommer til en ny etasje -> send heisen motsatt retning, forbi forrige etasje, uten at heisen stopper i forrige etasje -> bestill heisen til den forrige etasjen heisen stoppet på.
Feilen kommer av at lastDirectionBeforeStop resettes i stay-state og ikke i go-state.

Tilstandsmaskin med tilstander idle, go, stay, stop.

Heisdrivere skal ligge i src/driver.

Kompilatoren pleier å klage på at build/driver/ ikke finnes. Opprett mappen og make på nytt :).
