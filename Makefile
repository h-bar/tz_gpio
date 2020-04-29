all: gpio_test.out
gpio_test.out: gpio.c gpio.h gpio_test.c bcm2837.h
	gcc -o gpio_test.out gpio_test.c

.PHONY: run clean
run:
	sudo ./gpio_test.out
clean: 
	rm gpio_test.out
	rm tz_gpio_test.out

.PHONY: patch-and-make flash optee-clean

ROOT							?= $(CURDIR)/../
BUILD_DIR					?= $(ROOT)/build
OPTEE_DIR 				?= $(ROOT)/optee_os
OPTEE_CORE_DIR 		?= $(OPTEE_DIR)/core
PTA_DIR						?= $(OPTEE_CORE_DIR)/pta
PTA_INCLUDE_DIR 	?= $(OPTEE_CORE_DIR)/include

patch-and-make:
	@install -v $(CURDIR)/bcm2837.h $(PTA_INCLUDE_DIR)/bcm2837.h
	@install -v $(CURDIR)/gpio.c $(PTA_DIR)/gpio.c
	$(MAKE) -C $(BUILD_DIR)

flash:
	$(MAKE) -C $(BUILD_DIR) flash

optee-clean:
	$(MAKE) -C $(BUILD_DIR) clean


.PHONY: gpio_test_tz

CFLAGS += -Wall -I../ta/include -I/usr/include -I./include
LDADD += -lteec -L/usr/lib

gpio_test_tz: tz_gpio_test.out
tz_gpio_test.out: gpio.c gpio.h tz_gpio_test.c bcm2837.h
	gcc -o tz_gpio_test.o 	-c tz_gpio_test.c $(CFLAGS)
	gcc -o tz_gpio_test.out tz_gpio_test.o 		$(LDADD)
	# gcc -o tz_gpio_test.out tz_gpio_test.c $(CFLAGS) $(LDADD)

run_tz:
	sudo ./tz_gpio_test.out
