/*
 * (C) Copyright 2015
 * Emcraft Systems, <www.emcraft.com>
 * Alexander Potashev <aspotashev@emcraft.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>
#include <linux/gpio.h>

#include <mach/stm32.h>
#include <mach/fb.h>
#include <mach/platform.h>

#if defined(CONFIG_MFD_STMPE)

#include <mach/ts.h>

#include <mach/exti.h>
#include <linux/mfd/stmpe.h>

#define STM32F4_IRQ_EXTI4		10
#define STM32F4_IRQ_EXTI15_10		40
#define STMPE811_INTERNAL_IRQS		8
#define STMPE811_IRQ_BASE		(NR_IRQS - STMPE811_INTERNAL_IRQS)

#endif

#define STM32F4_LTDC_IRQ	88
#define STM32F4_LTDC_ERR_IRQ	89

/*
 * Framebuffer platform device resources
 */
static struct resource stm32f4_fb_resources[] = {
	{
		.start	= STM32F4_LTDC_BASE,
		.end	= STM32F4_LTDC_BASE + STM32F4_LTDC_LENGTH - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= STM32F4_LTDC_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};

struct stm32f4_fb_platform_data stm32f4x9_fb_data = {
	.mode_str	= "480x272",
	.modes		= NULL,
};

static struct platform_device stm32f4_fb_device = {
	.name = "stm32f4-ltdc",
	.id = 0,
	.num_resources = ARRAY_SIZE(stm32f4_fb_resources),
	.resource = stm32f4_fb_resources,
	.dev = {
		.coherent_dma_mask = 0xFFFFFFFF,
		.platform_data = &stm32f4x9_fb_data,
	},
};

#if defined(CONFIG_MFD_STMPE)
#if defined(CONFIG_STM32_TS)
/**
 * struct stmpe_ts_platform_data - stmpe811 touch screen controller platform
 * data
 * @sample_time: ADC converstion time in number of clock.
 * (0 -> 36 clocks, 1 -> 44 clocks, 2 -> 56 clocks, 3 -> 64 clocks,
 * 4 -> 80 clocks, 5 -> 96 clocks, 6 -> 144 clocks),
 * recommended is 4.
 * @mod_12b: ADC Bit mode (0 -> 10bit ADC, 1 -> 12bit ADC)
 * @ref_sel: ADC reference source
 * (0 -> internal reference, 1 -> external reference)
 * @adc_freq: ADC Clock speed
 * (0 -> 1.625 MHz, 1 -> 3.25 MHz, 2 || 3 -> 6.5 MHz)
 * @ave_ctrl: Sample average control
 * (0 -> 1 sample, 1 -> 2 samples, 2 -> 4 samples, 3 -> 8 samples)
 * @touch_det_delay: Touch detect interrupt delay
 * (0 -> 10 us, 1 -> 50 us, 2 -> 100 us, 3 -> 500 us,
 * 4-> 1 ms, 5 -> 5 ms, 6 -> 10 ms, 7 -> 50 ms)
 * recommended is 3
 * @settling: Panel driver settling time
 * (0 -> 10 us, 1 -> 100 us, 2 -> 500 us, 3 -> 1 ms,
 * 4 -> 5 ms, 5 -> 10 ms, 6 for 50 ms, 7 -> 100 ms)
 * recommended is 2
 * @fraction_z: Length of the fractional part in z
 * (fraction_z ([0..7]) = Count of the fractional part)
 * recommended is 7
 * @i_drive: current limit value of the touchscreen drivers
 * (0 -> 20 mA typical 35 mA max, 1 -> 50 mA typical 80 mA max)
 *
 * */
static struct stmpe_ts_platform_data stmpe811_ts_info = {
			.coordinate_calibration = stm32f4_ts_calibration,
			.sample_time 		= 0x04,
			.mod_12b 		= 0x01,
			.ref_sel 		= 0x00,
			.adc_freq 		= 0x01,
			.ave_ctrl 		= 0x02,
			.touch_det_delay 	= 0x03,
			.settling 		= 0x02,
			.fraction_z 		= 0x07,
			.i_drive 		= 0x01,
		};
#endif

static struct stmpe_platform_data stmpe811_ioe_info = {
			.id             	= 0,
		        .blocks         	= STMPE_BLOCK_TOUCHSCREEN,
			.irq_trigger    	= IRQF_TRIGGER_FALLING,
			.irq_base		= STMPE811_IRQ_BASE,
			.exti_line		= STM32F2_EXTI_LINE_GPIO_4,
			.exti_enable_int 	= stm32_exti_enable_int_ts,
			.exti_clear_pending	= stm32_exti_clear_pending_ts,
#if defined(CONFIG_STM32_TS)			
			.ts			= &stmpe811_ts_info,
#endif
		};

static struct i2c_board_info __initdata
			stm32f4_bdinfo_i2c1[] = {
	        {
			I2C_BOARD_INFO("stmpe811", 0x41),
			.platform_data = &stmpe811_ioe_info,
			.irq = STM32F4_IRQ_EXTI4,
		}
	};
#endif

#if defined(CONFIG_TOUCHSCREEN_CRTOUCH_MT)
/*
 * I2C-connected Freescale CRTouch touchscreen device.
 * We register it with the driver `crtouch_mt`.
 */
static struct i2c_board_info __initdata emcraft_iot_lcd_crtouch = {
	I2C_BOARD_INFO("crtouchId", 0x49),
};
#endif /* CONFIG_TOUCHSCREEN_CRTOUCH_MT */


static int stm32f4x9_fb_lcd_init(int init)
{
	int p = stm32_platform_get();

	if (p == PLATFORM_STM32_STM32F7_DISCO) {
		gpio_direction_output(STM32_GPIO_PORTPIN2NUM(7, 3), 0);
	}

#if defined(CONFIG_TOUCHSCREEN_CRTOUCH_MT)
	/*
	 * Set wake-up active
	 */
	else if (p == PLATFORM_STM32_STM_STM32F7_SOM)
		gpio_direction_output(STM32_GPIO_PORTPIN2NUM(7, 3), 0);
#endif
	if (p == PLATFORM_STM32_STM_DISCO) {
		gpio_direction_output(STM32_GPIO_PORTPIN2NUM(0, 1), 1);
		gpio_direction_output(STM32_GPIO_PORTPIN2NUM(0, 2), 0);
	}
	return 0;
}

void __init stm32f4x9_fb_init(void)
{
	int device;
	int ret;

	ret = 0;

	device = stm32_device_get();
	if (device == DEVICE_STM32F439II || device == DEVICE_STM32F746NG) {
#if defined(CONFIG_TOUCHSCREEN_CRTOUCH_MT)
		/*
		 * Register the I2C-connected CRTouch touchscreen
		 */
		i2c_register_board_info(0, &emcraft_iot_lcd_crtouch, 1);
#endif
		
#if defined(CONFIG_MFD_STMPE)
		i2c_register_board_info(0, stm32f4_bdinfo_i2c1,
			sizeof(stm32f4_bdinfo_i2c1) /
			sizeof (struct i2c_board_info));
#endif
		stm32f4x9_fb_data.init = stm32f4x9_fb_lcd_init;
		ret = platform_device_register(&stm32f4_fb_device);
	}

	if (ret < 0) {
		printk(KERN_ERR "stm32f4-fb: Could not initialize "
			"the LCD screen.\n");
	}
}
