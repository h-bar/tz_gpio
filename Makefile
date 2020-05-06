.PHONY: all nw sw run run-sw run-nw clean clean-sw clean-nw
all: sw nw

sw: gpio_test-sw.out
nw: gpio_test-nw.out

run-sw:
	sudo ./gpio_test-sw.out
clean-sw:
	rm -f ./gpio_test-sw.out

run-nw:
	sudo ./gpio_test-nw.out
clean-nw:
	rm -f ./gpio_test-nw.out

clean: clean-sw clean-nw

CFLAGS += -I../ta/include -I/usr/include -I./include
LDADD += -lteec -L/usr/lib
gpio_test-sw.out: gpio_test.c include/tz_gpio.h include/bcm2837.h
	gcc -o gpio_test-sw.out gpio_test.c -D TZ $(CFLAGS) $(LDADD)

gpio_test-nw.out: gpio_test.c include/tz_gpio.h include/bcm2837.h
	gcc -o gpio_test-nw.out gpio_test.c $(CFLAGS) $(LDADD)




# all: gpio_test.out
# gpio_test.out: gpio.c gpio.h gpio_test.c bcm2837.h
# 	gcc -o gpio_test.out gpio_test.c

# .PHONY: run clean
# run:
# 	sudo ./gpio_test.out
# clean: 
# 	rm gpio_test.out
# 	rm tz_gpio_test.out

# .PHONY: patch-and-make flash optee-clean

# ROOT							?= $(CURDIR)/../
# BUILD_DIR					?= $(ROOT)/build
# OPTEE_DIR 				?= $(ROOT)/optee_os
# OPTEE_CORE_DIR 		?= $(OPTEE_DIR)/core
# PTA_DIR						?= $(OPTEE_CORE_DIR)/pta
# PTA_INCLUDE_DIR 	?= $(OPTEE_CORE_DIR)/include

# patch-and-make:
# 	@install -v $(CURDIR)/bcm2837.h $(PTA_INCLUDE_DIR)/bcm2837.h
# 	@install -v $(CURDIR)/gpio.c $(PTA_DIR)/gpio.c
# 	$(MAKE) -C $(BUILD_DIR)

# flash:
# 	$(MAKE) -C $(BUILD_DIR) flash

# optee-clean:
# 	$(MAKE) -C $(BUILD_DIR) clean
