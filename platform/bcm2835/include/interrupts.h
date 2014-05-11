#ifndef _BCM2835_INTERRUPTS_H
#define _BCM2835_INTERRUPTS_H

#include <exceptions.h>

#define INTERRUPT_REG_BASE  0x2000B000

#define IRQ_BASIC_PENDING   0x200
#define IRQ_PENDING_1       0x204
#define IRQ_PENDING_2       0x208
#define FIQ_CONTROL         0x20C
#define IRQ_ENABLE_1        0x210
#define IRQ_ENABLE_2        0x214
#define IRQ_BASIC_ENABLE    0x218
#define IRQ_DISABLE_1       0x21C
#define IRQ_DISABLE_2       0x220
#define IRQ_BASIC_DISABLE   0x224

#define INTERRUPT_TIMER0    0
#define INTERRUPT_TIMER1    1
#define INTERRUPT_TIMER2    2
#define INTERRUPT_TIMER3    3

#define FIQ_TIMER0          (INTERRUPT_TIMER0)
#define FIQ_TIMER1          (INTERRUPT_TIMER1)
#define FIQ_TIMER2          (INTERRUPT_TIMER2)
#define FIQ_TIMER3          (INTERRUPT_TIMER3)

#define FIQ_ARM_TIMER       64

typedef void (*irq_handler_t)(irq_frame_t *);
typedef void (*fiq_handler_t)(exception_frame_t *);

/*
 * Enable a specific IRQ.
 *     uint8_t vector -> the vector for IRQ, where basic IRQs succeed reg 1 & 2.
 *     irq_handler_t handler.
 */ 
void platform_enable_irq(uint8_t vector, irq_handler_t handler);

/*
 * Enable the FIQ.
 *     uint8_t vector -> the vector for FIQ defined as per FIQ sources.
 *     fiq_handler_t handler.
 */ 
void platform_enable_fiq(uint8_t vector, fiq_handler_t handler);

/*
 * The platform IRQ handler.
 *     irq_frame_t *irq_frame -> stack frame as passed by the exception handling glue.
 */
void platform_irq_handler(irq_frame_t *irq_frame);

/*
 * Initializes the interrupt system.
 */
void platform_interrupts_init(void);

#endif /* _BCM2835_INTERRUPTS_H */
