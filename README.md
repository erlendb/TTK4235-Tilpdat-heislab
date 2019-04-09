# Heis Weiß

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



Sekvensdiagram

title Heis Weiß

Main->States:state_idle()
States->Queue:queue_count()
Queue->States:count
States->Main:state=idle
Main->Elevator:elevator_update_button_signals()
Main->Queue:queue_update()
Main->Lights:lights_update()
Main->States:state_idle()
States->Queue:queue_count()
Queue->States:count
States->Main:state = go
Main->States:state_go()
States->Driver:elev_get_floor_sensor_signal()
Driver->States:floor
States->Elevator:elevator_set_direction()
States->Driver:elev_set_floor_indicator()
States->Queue:queue_stop()
Queue->States:shouldStop
States->Main:state = stay
Main->States:go_stay()
States->Driver:getFloor()
Driver->States:floor
States->Timer:is_timer_activated()
Timer->States:is_timer_activated
States->Lights:lights_clear()
States->Queue:queue_clear()
States->Door:door_open()
States->Timer:timer_start()
States->Queue:queue_get_order(
Queue->States:
States->Timer:timer_check()
Timer->States:
States->Door:door_close()
States->Timer:timer_deactivate()
States->Main:state = idle
