#include "bcm2837.h"

#define TA_NAME			"gpio.ta"
#define GPIO_UUID 	{ 0xd96a5b40, 0xe2c7, 0xb1af, { 0x87, 0x98, 0x16, 0x02, 0xc5, 0xff, 0xa8, 0x10 } }

#define GPIO_SET_PIN_FSEL   0x10
#define GPIO_SET_PIN_LEVEL  0x11
#define GPIO_GET_PIN_LEVEL  0X12

#define PWM_SET_CLK         0x21
#define PWM_SET_M           0x22
#define PWM_SET_N           0x23
#define PWM_SET_ENABLE      0x24

//############################
//   OPTEE
//############################ 
TEEC_Result res;
TEEC_Context ctx;
TEEC_Session sess;
TEEC_Operation op;
TEEC_UUID uuid = GPIO_UUID;
uint32_t err_origin;

static int init_tee() {
	res = TEEC_InitializeContext(0, &ctx);
	if (res != TEEC_SUCCESS) errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	res = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, 0, 0, &err_origin);
	if (res != TEEC_SUCCESS) errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x", res, err_origin);

	memset(&op, 0, sizeof(op));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
	
	return 0;
}

static int destroy_tee() {
	TEEC_CloseSession(&sess);
	TEEC_FinalizeContext(&ctx);
	return 0;
}

static inline int invoke(uint32_t cmd, uint32_t a, uint32_t b) {
	op.params[0].value.a = a;
	op.params[0].value.b = b;
	res =  TEEC_InvokeCommand(&sess, cmd, &op, &err_origin);
	if (res != TEEC_SUCCESS) {
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x", res, err_origin);
		return -1;
	}
	
	return op.params[0].value.b;
}

//############################
//   GPIO
//############################ 
static inline void gpio_set_func(uint8_t pin, gpio_func func) {
	invoke(GPIO_SET_PIN_FSEL, pin, func);
}

static inline void gpio_set_level(uint8_t pin, gpio_level level) {
	invoke(GPIO_SET_PIN_LEVEL, pin, level);
}

static inline uint32_t gpio_get_level(uint8_t pin) {
	return invoke(GPIO_GET_PIN_LEVEL, pin, -1);
}

//############################
//   PWM
//############################ 
static inline void pwm_set_clk(uint32_t divider) {
	invoke(PWM_SET_CLK, divider, -1);
}

static inline void pwm_set_M(uint32_t pwm_num, uint32_t M) {
	invoke(PWM_SET_M, pwm_num, M);
}

static inline void pwm_set_N(int32_t pwm_num, uint32_t N) {
	invoke(PWM_SET_N, pwm_num, N);
}

static inline void pwm_enable(int32_t pwm_num) {
	invoke(PWM_SET_ENABLE, pwm_num, -1);
}