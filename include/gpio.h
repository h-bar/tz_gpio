#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>

#include "bcm2837.h"

static volatile bcm2837_gpio *gpio;
static volatile bcm2837_pwm	*pwm;
static volatile bcm2837_clk *clk;


static int init_gpio() {
  int fd = open("/dev/mem",O_RDWR|O_SYNC);
	if(fd < 0) {
		printf("Can't open /dev/mem\n");
		return 1;
	}

	gpio = mmap(0, 4*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, BCM2837_GPIO_BASE);
	pwm = mmap(0, 4*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, BCM2837_PWM_BASE);
	clk = mmap(0, 4*1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, BCM2837_CLK_BASE) ;
	
	if(gpio == NULL || pwm == NULL || clk == NULL) {
		printf("Can't mmap devices\n");
		return 1;
	}
	
	return 0;
}

static int destroy_gpio() {
	return 0;
}

//############################
//   GPIO
//############################ 
#define fsel_grp(pin) 									(pin/10)
#define pin_grp(pin)										(pin/32)
#define make_fsel_config(pin, option) 	(option << (pin%10 * 3))
#define make_pin_set_config(pin) 				(1 << pin%32)
#define make_pin_clr_config(pin) 				(1 << pin%32)
#define extract_reading(pin, reading)		(reading & (1 << pin%32))

static void gpio_set_func(uint8_t pin, gpio_func func) {
	gpio->GPFSEL[fsel_grp(pin)] = (gpio->GPFSEL[fsel_grp(pin)]
																& ~(make_fsel_config(pin, 0x7)))
																| make_fsel_config(pin, func);
}

static void gpio_set_level(uint8_t pin, gpio_level level) {
	if (level == gpio_high) gpio->GPSET[pin_grp(pin)] = make_pin_set_config(pin);
	else gpio->GPCLR[pin_grp(pin)] = make_pin_set_config(pin);
}

static gpio_level gpio_get_level(uint8_t pin) {
	if (extract_reading(pin, gpio->GPLEV[pin_grp(pin)])) return gpio_high;
	return gpio_low;
}



//############################
//   PWM
//############################ 
static void pwm_set_clk(uint32_t divider) {
	uint32_t pwmctl_holder = pwm->CTL;
	
	// from https://www.raspberrypi.org/forums/viewtopic.php?t=37770
	pwm->CTL = 0x0;																			// Turn oiff PWM
	clk->PWMCTL = BCM2837_CLK_PASSWD | 0x20;						// Stop CLK generator
	while(clk->PWMCTL & 0x80); 													// Wait for CLK generator to stop

	clk->PWMCTL = BCM2837_CLK_PASSWD | 0x006; 					// Source=PLLD (500 MHz)
	clk->PWMDIV = BCM2837_CLK_PASSWD | (divider << 12); // Configure divider
	clk->PWMCTL |= BCM2837_CLK_PASSWD | 0x10; 					// Enable clock
	while(!(clk->PWMCTL & 0x80)); 											// Wait for busy flag to turn on.

	pwm->CTL = pwmctl_holder;
}

static void pwm_set_M(uint32_t pwm_num, uint32_t M) {
	if (pwm_num == 0) pwm->RNG1 = M;
	else pwm->RNG2 = M;
}

static void pwm_set_N(int32_t pwm_num, uint32_t N) {
	if (pwm_num == 0) pwm->DAT1 = N;
	else pwm->DAT2 = N;
}

static void pwm_enable(int32_t pwm_num) {
	if (pwm_num == 0) pwm->CTL |= 0x81;
	else pwm->CTL |= 0x8100;

	pwm->STA |= 0x10;
}