/**
  * @file
  * @brief Køsystem
  */

extern int lastFloor;
extern int currentFloor;
extern elev_motor_direction_t lastDirection;
extern elev_motor_direction_t direction;
extern int queue[];

//Bestillinger
typedef enum orders {
  ORDER_NO = -1,
  ORDER_DOWN = 0,
  ORDER_UP = 1,
  ORDER_ALL = 2
} order_t;


//Etasjer
typedef enum floors {FIRST, SECOND, THIRD, FOURTH} floor_t;

/*!
  * @brief Oppdaterer køen ut fra ny bestilling og eksisterende bestillinger.
  * @details
      order = -1 (ingen bestilling) overskriver etasjen.
  * @param[in]  floor Etasjen der bestillinger skal oppdateres.
  * @param[in]  order Den nye bestillingen i gitt etasje.
  */
void queue_update(int floor, int order);
void queue_clear();
int queue_stop(int floor);
int queue_count();

int queue_check_above(int floor);
int queue_check_below(int floor);

int queue_get(int floor);

void set_direction(elev_motor_direction_t dirn);

void queue_check_buttons();
