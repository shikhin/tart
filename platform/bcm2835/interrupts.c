#include <interrupts.h>
#include <stdint.h>
#include <mmio.h>

// Register 1, register 2, basic register -- 1 marks enabled source.
static uint32_t irq_sources[3];
static uint32_t fiq_source;

static irq_handler_t irq_handlers[72];

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

    if (!(irq_sources[reg] & (1 << vector))) {
        mmio_reg_write(reg_addr, 1 << vector);
        irq_sources[reg] &= (1 << vector);
    }
}

void platform_enable_fiq(uint8_t vector)
{
    if (vector > 71)
        return;

    vector &= (1 << 7);

    fiq_source = vector;
    mmio_reg_write(INTERRUPT_REG_BASE + FIQ_CONTROL, vector);
}

void platform_fiq_handler(exception_frame_t *exception_frame __UNUSED)
{

}

void platform_irq_handler(irq_frame_t *irq_frame)
{
    uint32_t pending_basic, pending[2];
    pending_basic = mmio_reg_read(INTERRUPT_REG_BASE + IRQ_BASIC_PENDING);
    pending[0] = mmio_reg_read(INTERRUPT_REG_BASE + IRQ_PENDING_1);
    pending[1] = mmio_reg_read(INTERRUPT_REG_BASE + IRQ_PENDING_2);

    // The 'GPU IRQs' in the basic pending register. 
    static uint8_t gpu_irqs[] = {7, 9, 10, 18, 19, 53, 54, 55, 56, 57, 62};
    for (int i = 0; i < 8; i++)
        if (pending_basic & (1 << i)) {
            /* TODO: assert(irq_handlers[64 + i]); */
            irq_handlers[64 + i](irq_frame);
        }

    for (int i = 10; i < 21; i++)
        if (pending_basic & (1 << i)) {
            /* TODO: assert(irq_handlers[gpu_irqs[i - 10]]); */
            irq_handlers[gpu_irqs[i - 10]](irq_frame);
        }

    for (int i = 0; i < 2; i++) {
        if (pending_basic & (1 << (8 + i))) {
            for (int j = 0; j < 32; j++)
                if (pending[i] & (1 << j)) {
                    /* TODO: assert(irq_handlers[(32 * i) + j]); */
                    irq_handlers[(32 * i) + j](irq_frame);
                }
        }
    }
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
