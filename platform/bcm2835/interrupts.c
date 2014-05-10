#include <interrupts.h>
#include <stdint.h>
#include <mmio.h>

// Register 1, register 2, basic register -- 1 marks enabled source.
static uint32_t irq_sources[3];
static uint32_t fiq_source;

static irq_handler_t irq_handlers[72];
static fiq_handler_t fiq_handler;

void platform_enable_irq(uint8_t vector, irq_handler_t handler)
{
    if (vector > 71)
        return;

    irq_handlers[vector] = handler; 
    int reg = 0; uintptr_t reg_addr = INTERRUPT_REG_BASE;

    if (vector > 63) {
        vector -= 64; reg = 2, reg_addr += IRQ_BASIC_ENABLE;
    } else if (vector > 31) {
        vector -= 32; reg = 1, reg_addr += IRQ_ENABLE_2;
    } else {
        reg_addr += IRQ_ENABLE_1;
    }

    if (!(irq_sources[reg] & (1 << vector)))
    {
        mmio_reg_write(reg_addr, 1 << vector);
        irq_sources[reg] &= (1 << vector);
    }
}

void platform_enable_fiq(uint8_t vector, fiq_handler_t handler)
{
    if (vector > 71)
        return;

    fiq_handler = handler;
    vector &= (1 << 7);

    fiq_source = vector;
    mmio_reg_write(INTERRUPT_REG_BASE + FIQ_CONTROL, vector);
}

void platform_interrupts_init()
{
    // Disable FIQ.
    mmio_reg_write(INTERRUPT_REG_BASE + FIQ_CONTROL, 0x00000000);

    // Disable all IRQ sources.
    mmio_reg_write(INTERRUPT_REG_BASE + IRQ_DISABLE_1, 0xFFFFFFFF);
    mmio_reg_write(INTERRUPT_REG_BASE + IRQ_DISABLE_2, 0xFFFFFFFF);
    // Bits 31:8 are unused.
    mmio_reg_write(INTERRUPT_REG_BASE + IRQ_BASIC_DISABLE, 0xFF);
}
