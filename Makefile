# SIMULATOR = 1: kompilerer riktige filer for å kjøre heisen i simulator
# SIMULATOR = 0: kompilerer riktige filer for å kjøre heisen på sal
# Husk å også endre SIMULATOR-flagget i driver.h
SIMULATOR = 1
ifeq ($(SIMULATOR), 1)
	DRIVER_SOURCE = driver-sim/elev.c
else
	DRIVER_SOURCE = driver-sal/elev.c driver-sal/io.c
endif

SOURCES := $(DRIVER_SOURCE) main.c buttons.c fsm.c queue.c lights.c door.c timer.c

BUILD_DIR := build
OBJ := $(SOURCES:%.c=$(BUILD_DIR)/%.o)

SRC_DIR := src
SRC := $(SOURCES:%.c=$(SRC_DIR)/%.c)

CC := clang
CFLAGS := -O0 -g3 -Wall -Werror -std=gnu99

LDFLAGS := -lcomedi -lm

.DEFAULT_GOAL := heis

heis : $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

$(BUILD_DIR) :
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean:
	rm -rf $(.DEFAULT_GOAL) $(BUILD_DIR)
