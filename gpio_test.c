#include "bcm2837.h"
#include "optee.h"
#include "gpio.c"

#include <stdio.h>
#include <unistd.h>


static void gpio_test_out(uint8_t pin, gpio_level level) {
  TEE_Param params[TEE_NUM_PARAMS];
  params[0].value.a = pin;
  params[0].value.b = gpio_out;
  invoke_command(GPIO_SET_PIN_FSEL, 0, params);

  params[0].value.a = pin;
  params[0].value.b = level;
  invoke_command(GPIO_SET_PIN_LEVEL, 0, params);
}

static void gpio_test_in(uint8_t pin) {
  TEE_Param params[TEE_NUM_PARAMS];
  params[0].value.a = pin;
  params[0].value.b = gpio_in;
  invoke_command(GPIO_SET_PIN_FSEL, 0, params);

  params[0].value.a = pin;
  invoke_command(GPIO_GET_PIN_LEVEL, 0, params);
	if (params[0].value.b == gpio_high) printf("Pin %d -> High\n", pin);
	else printf("Pin %d -> Low\n", pin);
}

static void pwm_test() {
  TEE_Param params[TEE_NUM_PARAMS];
  params[0].value.a = 10;
  invoke_command(PWM_SET_CLK, 0, params);

  params[0].value.a = 0;
  params[0].value.b = 1024;
  invoke_command(PWM_SET_M, 0, params);

  params[0].value.a = 18;
  params[0].value.b = gpio_alt5;
  invoke_command(GPIO_SET_PIN_FSEL, 0, params);

  params[0].value.a = 0;
  invoke_command(PWM_SET_ENABEL, 0, params);

  int bright;
	while(1) {
    for (bright = 0 ; bright < 1024 ; ++bright) {
      params[0].value.a = 0;
      params[0].value.b = bright;
      invoke_command(PWM_SET_N, 0, params);
	    usleep(1000);		
    }
    for (bright = 1023 ; bright >= 0 ; --bright) {
      params[0].value.a = 0;
      params[0].value.b = bright;
      invoke_command(PWM_SET_N, 0, params);
	   usleep(1000) ;
    }
  }
}

int main() {
  gpio = core_mmu_get_va(BCM2837_GPIO_BASE, 0);
  if (gpio == NULL) return 1;
	pwm = core_mmu_get_va(BCM2837_PWM_BASE, 0);
  if (gpio == NULL) return 1;
	clk = core_mmu_get_va(BCM2837_CLK_BASE, 0);
  if (gpio == NULL) return 1;

	pwm_test();
	// gpio_test_in(25);
	// gpio_test_out(18, gpio_high);
	// gpio_test_in(23);
}