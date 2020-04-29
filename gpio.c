#include "bcm2837.h"

#include <unistd.h>

#include <stdint.h>
#include <stdio.h>

#ifndef OPTEE
#include "optee.h"
#endif

#ifdef OPTEE
#include <kernel/pseudo_ta.h>
#include <mm/core_memprot.h>

// #include <compiler.h>
// #include <stdio.h>
// #include <trace.h>
// #include <kernel/pseudo_ta.h>
// #include <mm/tee_pager.h>
// #include <mm/tee_mm.h>
// #include <string.h>
// #include <string_ext.h>
// #include <malloc.h>
// #include <io.h>
// #include <mm/core_mmu.h>
// #include <mm/core_memprot.h>
#endif

#define TA_NAME			"gpio.ta"
#define GPIO_UUID 	{ 0xd96a5b40, 0xe2c7, 0xb1af, { 0x87, 0x98, 0x16, 0x02, 0xc5, 0xff, 0xa8, 0x10 } }

#define GPIO_SET_PIN_FSEL   0x10
#define GPIO_SET_PIN_LEVEL  0x11
#define GPIO_GET_PIN_LEVEL  0X12

#define PWM_SET_CLK         0x21
#define PWM_SET_M           0x22
#define PWM_SET_N           0x23
#define PWM_SET_ENABEL      0x24

static TEE_Result invoke_command(uint32_t cmd_id, uint32_t param_types, TEE_Param params[TEE_NUM_PARAMS]) {
  if (cmd_id == GPIO_SET_PIN_FSEL) {
    uint8_t pin_num = params[0].a;
    gpio_func func = params[0].b;
    gpio_set_func(pin_num, func);
    return TEE_SUCCESS;


  } else if (cmd_id == GPIO_SET_PIN_LEVEL) {
    uint8_t pin_num = params[0].a;
    gpio_level level = params[0].b;
    gpio_set_level(pin_num, level);
    return TEE_SUCCESS;


  } else if (cmd_id == GPIO_GET_PIN_LEVEL) {
    uint8_t pin_num = params[0].a;
    params[0].b = gpio_get_level(pin_num);
    return TEE_SUCCESS;


  } else if (cmd_id == PWM_SET_CLK) {
    uint32_t divider = params[0].a;
    pwm_set_clk(divider);
    return TEE_SUCCESS;


  } else if (cmd_id == PWM_SET_M) {
    uint8_t pwm_num = params[0].a;
    uint32_t M = params[0].b;
    pwm_set_M(pwm_num, M);
    return TEE_SUCCESS;


  } else if (cmd_id == PWM_SET_N) {
    uint8_t pwm_num = params[0].a;
    uint32_t N = params[0].b;
    pwm_set_N(pwm_num, N);
    return TEE_SUCCESS;


  } else if (cmd_id == PWM_SET_ENABEL) {
    uint8_t pwm_num = params[0].a;
    pwm_enable(pwm_num);
    return TEE_SUCCESS;
  }

  return TEE_ERROR_NOT_IMPLEMENTED;
}