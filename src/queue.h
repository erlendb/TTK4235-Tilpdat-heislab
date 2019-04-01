extern int lastFloor;
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

void queue_update(int floor, int type);
void queue_clear();
int queue_stop(int floor);
int queue_count();
void queue_print();

int queue_check_above(int floor);
int queue_check_below(int floor);

void set_direction(elev_motor_direction_t dirn);
