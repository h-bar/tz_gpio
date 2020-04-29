all: gpio_test.out
gpio_test.out:
	gcc -o gpio_test.out gpio_test.c

.PHONY: run
run:
	sudo ./gpio_test.out


.PHONY: install-to-optee

ROOT							?= $(CURDIR)
BUILD_DIR					?= $(ROOT)/build
OPTEE_DIR 				?= $(ROOT)/optee_os
OPTEE_CORE_DIR 		?= $(OPTEE_DIR)/core
PTA_DIR						?= $(OPTEE_CORE_DIR)/pta
PTA_INCLUDE_DIR 	?= $(OPTEE_CORE_DIR)/include

patch-and-make:
	@install -v $(CURDIR)/bcm2837.h $(PTA_INCLUDE_DIR)/bcm2837.h
	@install -v $(CURDIR)/gpio.c $(PTA_DIR)/gpio.c
	$(MAKE) -C $(BUILD_DIR)