#ifndef _BCM2837_H
#define _BCM2837_H

#include <stdint.h>

/* BCM2837 GPIO Register Address */
#define BCM2837_PERI_BASE			0x3F000000

#define BCM2837_GPIO_OFFSET   0x00200000
#define BCM2837_PWM_OFFSET    0x0020C000
#define BCM2837_CLK_OFFSET		0x00101000

#define BCM2837_GPIO_BASE     (BCM2837_PERI_BASE + BCM2837_GPIO_OFFSET)
#define BCM2837_GPIO_SIZE			0xB1

#define BCM2837_PWM_BASE			(BCM2837_PERI_BASE + BCM2837_PWM_OFFSET)
#define BCM2837_PWM_SIZE			0x28

#define BCM2837_CLK_BASE			(BCM2837_PERI_BASE + BCM2837_CLK_OFFSET)

#define BCM2837_GPIO_GPFSEL0	0x00
#define BCM2837_GPIO_GPFSEL1	0x04
#define BCM2837_GPIO_GPFSEL2	0x08
#define BCM2837_GPIO_GPFSEL3	0x0C
#define BCM2837_GPIO_GPFSEL4	0x10
#define BCM2837_GPIO_GPFSEL5	0x14

#define BCM2837_GPIO_GPSET0		0x1C
#define BCM2837_GPIO_GPSET1		0x20

#define BCM2837_GPIO_GPCLR0		0x28
#define BCM2837_GPIO_GPCLR1		0x2C

#define BCM2837_GPIO_GPLEV0		0x34
#define BCM2837_GPIO_GPLEV1		0x38

#define BCM2837_GPIO_GPEDS0		0x40
#define BCM2837_GPIO_GPEDS1		0x44

#define BCM2837_GPIO_GPREN0		0x4C
#define BCM2837_GPIO_GPREN1		0x50

#define BCM2837_GPIO_GPFEN0		0x58
#define BCM2837_GPIO_GPFEN1		0x5C

/* BCM2837 GPIO Function Selection Options */
#define	BCM2837_GPIO_FSEL_IN		0x0			// 0b000
#define	BCM2837_GPIO_FSEL_OUT		0x1			// 0b001
#define BCM2837_GPIO_FSEL_ALT0  0x4			// 0b100
#define	BCM2837_GPIO_FSEL_ALT1  0x5			// 0b101
#define	BCM2837_GPIO_FSEL_ALT2  0x6			// 0b110
#define	BCM2837_GPIO_FSEL_ALT3  0x7			// 0b111
#define	BCM2837_GPIO_FSEL_ALT4  0x3			// 0b011
#define	BCM2837_GPIO_FSEL_ALT5  0x2			// 0b010

#define BCM2837_GPIO_LEVEL_HIGH 1
#define BCM2837_GPIO_LEVEL_LOW	0

#define	BCM2837_PWM_CTL 			0x0		// PWM Control 								32
#define	BCM2837_PWM_STA 			0x4		// STA PWM Status 						32
#define	BCM2837_PWM_DMAC 			0x8		// PWM DMA Configuration 			32
#define	BCM2837_PWM_RNG1 			0x10	// PWM Channel 1 Range 				32
#define	BCM2837_PWM_DAT1 			0x14	// PWM Channel 1 Data 				32
#define	BCM2837_PWM_FIF1 			0x18	// PWM FIFO Input 						32
#define	BCM2837_PWM_RNG2 			0x20	// RNG2 PWM Channel 2 Range 	32
#define	BCM2837_PWM_DAT2 			0x24	// PWM Channel 2 Data 				32



#define BCM2837_CLK_PWMCTL 	0xA0
#define BCM2837_CLK_PWMDIV	0xA4
#define BCM2837_CLK_PASSWD	0x5A000000
typedef struct {
	uint32_t GPFSEL[6];
	uint32_t : 32;
	uint32_t GPSET[2];
	uint32_t : 32;
	uint32_t GPCLR[2];
	uint32_t : 32;
	uint32_t GPLEV[2];
	uint32_t : 32;
	uint32_t GPEDS[2];
	uint32_t : 32;
	uint32_t GPREN[2];
	uint32_t : 32;
	uint32_t GPFEN[2];
	uint32_t : 32;
	uint32_t GPHEN[2];
	uint32_t : 32;
	uint32_t GPLEN[2];
	uint32_t : 32;
	uint32_t GPAREN[2];
	uint32_t : 32;
	uint32_t GPAFEN[2];
	uint32_t : 32;
	uint32_t GPPUD;
	uint32_t GPPUDCLK[2];
} bcm2837_gpio;

typedef struct {
	uint32_t CTL;								// 0x00
	uint32_t STA;								// 0x04
	uint32_t DMAC;							// 0x08
	uint32_t :	32;							// 0x0C
	uint32_t RNG1;							// 0x10
	uint32_t DAT1;							// 0x14
	uint32_t FIF1;							// 0x18
	uint32_t :	32;							// 0x1C
	uint32_t RNG2;							// 0x20
	uint32_t DAT2;							// 0x24
} bcm2837_pwm;

typedef struct {
	uint32_t unknow[40];
	uint32_t PWMCTL;
	uint32_t PWMDIV;
} bcm2837_clk;


typedef enum  {
	gpio_in		= BCM2837_GPIO_FSEL_IN,
	gpio_out	= BCM2837_GPIO_FSEL_OUT,
	gpio_alt0 = BCM2837_GPIO_FSEL_ALT0,
	gpio_alt1	= BCM2837_GPIO_FSEL_ALT1,
	gpio_alt2	= BCM2837_GPIO_FSEL_ALT2,
	gpio_alt3	= BCM2837_GPIO_FSEL_ALT3,
	gpio_alt4	= BCM2837_GPIO_FSEL_ALT4,
	gpio_alt5	= BCM2837_GPIO_FSEL_ALT5,
} gpio_func;

typedef enum  {
	gpio_high		= BCM2837_GPIO_LEVEL_HIGH,
	gpio_low		= BCM2837_GPIO_LEVEL_LOW
} gpio_level;


#endif /*_BCM2837_H*/
