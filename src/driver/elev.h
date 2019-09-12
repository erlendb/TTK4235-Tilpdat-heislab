// Denne filen inneholder headerfil (elev.h) for driveren for både simulatoren og heisen på sal.
// For å velge om du skal bruke header for simulator eller for sal må du endre kodelinjen under
#define SIMULATOR   // #define SIMULATOR hvis du skal kjøre heisen i simulator
//#define SAL       // #define SAL hvis du skal kjøre heisprogrammet i heisen på sanntidssal.

// Mer info om simulatorheis på github.com/erlendb

#ifdef SIMULATOR //Koden herfra og ned til #endif kjøres hvis du har satt inn #define SIMULATOR over

#ifndef __INCLUDE_DRIVER_H__
#define __INCLUDE_DRIVER_H__

// Number of floors. Hardware-dependent, do not modify.
#define N_FLOORS 4

// Number of buttons (and corresponding lamps) on a per-floor basis
#define N_BUTTONS 3

typedef enum tag_elev_motor_direction {
    DIRN_DOWN = -1,
    DIRN_STOP = 0,
    DIRN_UP = 1
} elev_motor_direction_t;

typedef enum tag_elev_lamp_type {
    BUTTON_CALL_UP = 0,
    BUTTON_CALL_DOWN = 1,
    BUTTON_COMMAND = 2
} elev_button_type_t;


int elev_init(); //void->int

void elev_set_motor_direction(elev_motor_direction_t dirn);
void elev_set_button_lamp(elev_button_type_t button, int floor, int value);
void elev_set_floor_indicator(int floor);
void elev_set_door_open_lamp(int value);
void elev_set_stop_lamp(int value);

int elev_get_button_signal(elev_button_type_t button, int floor);
int elev_get_floor_sensor_signal(void);
int elev_get_stop_signal(void);
int elev_get_obstruction_signal(void);


#endif // #ifndef __INCLUDE_DRIVER_H__

#endif // #ifdef SIMULATOR

#ifdef SAL

// Wrapper for libComedi Elevator control.
// These functions provides an interface to the elevators in the real time lab
//
// 2007, Martin Korsgaard
#ifndef __INCLUDE_DRIVER_H__
#define __INCLUDE_DRIVER_H__


// Number of floors
#define N_FLOORS 4
#define N_BUTTONS 3



/**
  Initialize elevator.
  @return Non-zero on success, 0 on failure.
*/
int elev_init(void);



/**
  Motor direction for function elev_set_motor_direction().
*/
typedef enum tag_elev_motor_direction {
    DIRN_DOWN = -1,
    DIRN_STOP = 0,
    DIRN_UP = 1
} elev_motor_direction_t;



/**
  Sets the motor direction of the elevator.
  @param dirn New direction of the elevator.
*/
void elev_set_motor_direction(elev_motor_direction_t dirn);



/**
  Turn door-open lamp on or off.
  @param value Non-zero value turns lamp on, 0 turns lamp off.
*/
void elev_set_door_open_lamp(int value);



/**
  Get signal from obstruction switch.
  @return 1 if obstruction is enabled. 0 if not.
*/
int elev_get_obstruction_signal(void);



/**
  Get signal from stop button.
  @return 1 if stop button is pushed, 0 if not.
*/
int elev_get_stop_signal(void);



/**
  Turn stop lamp on or off.
  @param value Non-zero value turns lamp on, 0 turns lamp off.
*/
void elev_set_stop_lamp(int value);



/**
  Get floor sensor signal.
  @return -1 if elevator is not on a floor. 0-3 if elevator is on floor. 0 is
    ground floor, 3 is top floor.
*/
int elev_get_floor_sensor_signal(void);



/**
  Set floor indicator lamp for a given floor.
  @param floor Which floor lamp to turn on. Other floor lamps are turned off.
*/
void elev_set_floor_indicator(int floor);



/**
  Button types for function elev_set_button_lamp() and elev_get_button().
*/
typedef enum tag_elev_lamp_type {
    BUTTON_CALL_UP = 0,
    BUTTON_CALL_DOWN = 1,
    BUTTON_COMMAND = 2
} elev_button_type_t;



/**
  Gets a button signal.
  @param button Which button type to check. Can be BUTTON_CALL_UP,
    BUTTON_CALL_DOWN or BUTTON_COMMAND (button "inside the elevator).
  @param floor Which floor to check button. Must be 0-3.
  @return 0 if button is not pushed. 1 if button is pushed.
*/
int elev_get_button_signal(elev_button_type_t button, int floor);



/**
  Set a button lamp.
  @param lamp Which type of lamp to set. Can be BUTTON_CALL_UP,
    BUTTON_CALL_DOWN or BUTTON_COMMAND (button "inside" the elevator).
  @param floor Floor of lamp to set. Must be 0-3
  @param value Non-zero value turns lamp on, 0 turns lamp off.
*/
void elev_set_button_lamp(elev_button_type_t button, int floor, int value);



#endif // #ifndef __INCLUDE_DRIVER_H__

#endif // #ifdef SAL
