//Heistilstander
typedef enum states {idle, go, stay, stop} state_t;

//Stoppflagg
enum stops {NOT_STOPPED, STOPPED};

state_t state_start();

state_t state_idle();

state_t state_go();

state_t state_stay();

state_t state_stop();
