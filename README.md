# Heis Weiß

## Masterbranch

Testet og funker på sal.

Tilstandsmaskin med tilstandene idle, go, stay, stop.

## Entry-exit-states-branch

Testet og funker i simulator.

Samme tilstander som tidligere, i tillegg til eventuelle inn- og ut-tilstander:

* idle (ingen inn- eller ut-tilstander)
* go
  * go_entry
  * go_internal
* stay
  * stay_entry
  * stay_internal
  * stay_exit
* stop (ingen inn- eller ut-tilstander)

Inn- og ut-tilstandene kjører kun én gang, henholdsvis før og etter tilstandsmaskinen havner i tilhørende interntilstand. Interntilstanden kjører i løkke.


---

Heisdriveren skal i src/driver.

Kompilatoren pleier å klage på at build/ og build/driver/ ikke finnes.

På sal: bruk elev.c/h, io.c/h, channels.h fra oppgavezipen. io.c må legges til Makefila.

For simulator: bruk elev.c/h fra https://github.com/erlendb/simulatorheis_tilpdat

* (???) Trøbbel i tårnet hvis man slenger inn bestilling i samme etasje som heisen står i, akkurat idet heisen er på vei bort derfra. Hvis man er uheldig så kjøres lights_clear() med floor=-1.

spør ORDER = BUTTON?
spør define alle konstante variabler?
spør inn- og ut-states?
timer_check()
inn- og ut-states?
x timer_is_activated() -> timer_is_started()
x timer_deactivate() -> timer_clear()
x timeLimit -> TIME_LIMIT
x BETWEEN_FLOORS = -1
x Elevatormodul?
x ERROR_STATE
x queue_stop() -> queue_should_stop()
x lastDirectionBeforeStop
x states caps
x queue_count() -> queue_count_orders()
- konsekvent state_t order_t button vs int
- queue_add(floor, button)



misbruk av notasjon lastDirectionBeforeStop
