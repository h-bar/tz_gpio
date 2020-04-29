#include <err.h>
#include <stdio.h>
#include <string.h>

/* OP-TEE TEE client API (built by optee_client) */
#include <tee_client_api.h>

#include "gpio.h"
#include "bcm2837.h"

#define GPIO_UUID 	{ 0xd96a5b40, 0xe2c7, 0xb1af, { 0x87, 0x98, 0x16, 0x02, 0xc5, 0xff, 0xa8, 0x10 } }


TEEC_Result res;
TEEC_Context ctx;
TEEC_Session sess;
TEEC_Operation op;
TEEC_UUID uuid = GPIO_UUID;
uint32_t err_origin;

static int init_tee() {
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);
	return 0;
}

static int destroy_tee() {
	TEEC_CloseSession(&sess);
	TEEC_FinalizeContext(&ctx);
	return 0;
}

static TEEC_Result invoke(uint32_t cmd, uint32_t a, uint32_t b) {
	op.params[0].value.a = a;
	op.params[0].value.b = b;
	res =  TEEC_InvokeCommand(&sess, cmd, &op, &err_origin);
	if (res != TEEC_SUCCESS)
	errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
}

static void gpio_test_out(uint8_t pin, gpio_level level) {
  invoke(GPIO_SET_PIN_FSEL, pin, gpio_out);
	invoke(GPIO_SET_PIN_LEVEL, pin, level);
}

static void gpio_test_in(uint8_t pin) {
	invoke(GPIO_SET_PIN_FSEL, pin, gpio_in);
	invoke(GPIO_GET_PIN_LEVEL, pin, NULL);
	if (op.params[0].value.b == gpio_high) printf("Pin %d -> High\n", pin);
	else printf("Pin %d -> Low\n", pin);
}

static void pwm_test() {
	invoke(PWM_SET_CLK, 10, NULL);
	invoke(PWM_SET_M, 0, 1024);
	invoke(PWM_SET_N, 0, 0);
	invoke(GPIO_SET_PIN_FSEL, 18, gpio_alt5);
	invoke(PWM_SET_ENABEL, 0, NULL);

  int bright;
	while(1) {
    for (bright = 0 ; bright < 1024 ; ++bright) {
			invoke(PWM_SET_N, 0, bright);
	    usleep(1000);		
    }
    for (bright = 1023 ; bright >= 0 ; --bright) {
			invoke(PWM_SET_N, 0, bright);
			usleep(1000);
    }
  }
}

static void motor_test() {
	invoke(PWM_SET_CLK, 10, NULL);
	invoke(PWM_SET_M, 0, 1024);
	invoke(PWM_SET_N, 0, 50);
	invoke(GPIO_SET_PIN_FSEL, 18, gpio_alt5);
	invoke(PWM_SET_ENABEL, 0, NULL);
}


int main(void)
{
	init_tee();


	memset(&op, 0, sizeof(op));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	
	// pwm_test();
	motor_test();
	// gpio_test_out(18, gpio_high);
	// gpio_test_in(23);
	
	destroy_tee();
	return 0;
}
