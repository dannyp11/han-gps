CHECK_SRC_DIRS := $(shell find test/* -maxdepth 0 -type d)
CHECK_SRC_DIRS += $(shell find test/xbee-test/* -maxdepth 0 -type d)

CHECK_SRC_DIRS := $(filter-out test/xbee-test,$(CHECK_SRC_DIRS))
CHECK_SRC_DIRS := $(filter-out test/gps,$(CHECK_SRC_DIRS))
CHECK_SRC_DIRS := $(filter-out test/stm32f103c8-blink,$(CHECK_SRC_DIRS))

CHECK_SRC_OBJS := $(addsuffix .diro,$(CHECK_SRC_DIRS))

all:
	$(MAKE) -C main

check: $(CHECK_SRC_OBJS)

	
clean:
	./cleanup.sh
	
	
%.diro: %
	$(MAKE) -C $<  
