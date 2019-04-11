// Sett SIMULATOR til 1 for å inkludere simulatordriveren.
// Sett SIMULATOR til 0 for å inkludere salheisdriveren.
// Husk å også endre SIMULATOR-flagget i Makefile
#define SIMULATOR 0

#if SIMULATOR
  #include "driver-sim/elev.h"
#else
  #include "driver-sal/elev.h"
#endif
