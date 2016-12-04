/*
 * (C) Copyright (C) 2009
 * ARM Ltd.
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

#ifndef __ASM_ARCH_IRQS_H
#define __ASM_ARCH_IRQS_H

#ifdef CONFIG_ARCH_STM32F1
#define NVIC_IRQS	68
#define STM32_GPIO_NUM	140
#else
#define NVIC_IRQS	91
#define STM32_GPIO_NUM	168
#endif

#if defined (CONFIG_STM32_GPIO_INT)
#define NR_IRQS		(NVIC_IRQS + STM32_GPIO_NUM)
#else
#define NR_IRQS		NVIC_IRQS
#endif
/*
 * Re-define NR_IRQS if the stmpe811 driver is compiled for
 * stm32f42/stm32f43 MCUs.
 */
#if defined(CONFIG_MFD_STMPE)
#undef NR_IRQS
#define NR_IRQS		(NVIC_IRQS + 8) /*8 internal interrupts (stmpe811)*/
#endif
#endif

