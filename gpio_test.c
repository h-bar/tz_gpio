#include <err.h>
#include <stdio.h>
#include <string.h>
#include <time.h> 

#ifdef TZ
#include <tee_client_api.h>
#include "tz_gpio.h"
#else
#include "gpio.h"
#endif

static void gpio_test_out(uint8_t pin, gpio_level level) {
	gpio_set_func(pin, gpio_out);
	gpio_set_level(pin, level);
}

static void gpio_test_in(uint8_t pin) {
	gpio_set_func(pin, gpio_in);
	int level = gpio_get_level(pin);
	if (level == gpio_high) printf("Pin %d -> High\n", pin);
	else printf("Pin %d -> Low\n", pin);
}

static void pwm_test() {
	pwm_set_clk(10);
	pwm_set_M(0, 1024);
	pwm_set_N(0, 0);
	gpio_set_func(18, gpio_alt5);
	pwm_enable(0);

  int bright;
	while(1) {
    for (bright = 0 ; bright < 1024 ; ++bright) {
			pwm_set_N(0, bright);	
    }
    for (bright = 1023 ; bright >= 0 ; --bright) {
			pwm_set_N(0, bright);
    }
  }
}

static void test_100_io(){
	clock_t t; 
	gpio_set_func(18, gpio_out);
  t = clock();
	for (int i = 0; i < 1000; i++) {
		gpio_set_level(18, gpio_high);
	}
	t = clock() - t;

	printf("1000 gpio set takes %d cycles\n", t);
}

int main(void)
{
	init_gpio();
	test_100_io();
	// pwm_test();
	// gpio_test_out(18, gpio_high);
	// gpio_test_in(23);
	
	destroy_gpio();
	return 0;
}
