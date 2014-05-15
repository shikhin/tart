#ifndef _BCM2835_MAILBOX_H
#define _BCM2835_MAILBOX_H

#include <stdint.h>

#define MAILBOX_BASE    0x2000B880
#define MAILBOX_POLL    0x10
#define MAILBOX_SENDER  0x14
#define MAILBOX_STATUS  0x18
#define MAILBOX_CONFIG  0x1C
#define MAILBOX_WRITE   0x20

#define EMMC_CLOCK_ID   0x01
#define UART_CLOCK_ID   0x02
#define ARM_CLOCK_ID    0x03
#define CORE_CLOCK_ID   0x04
#define V3D_CLOCK_ID    0x05
#define H264_CLOCK_ID   0x06
#define ISP_CLOCK_ID    0x07
#define SDRAM_CLOCK_ID  0x08
#define PIXEL_CLOCK_ID  0x09
#define PWM_CLOCK_ID    0x0A

#define MAILBOX_ARM_TO_VC 8

#define MAILBOX_SUCCESS 0x80000000

#define MAILBOX_TIMEOUT 100

/*
 * Synchronously write to the mailbox.
 *     uint32_t data.
 *     uint8_t channel.
 */
void mailbox_write(uint32_t data, uint8_t channel);

/*
 * Synchronously read from the mailbox.
 *     uint8_t channel.
 *
 * Returns:
 *     uint32_t.
 */
uint32_t mailbox_read(uint8_t channel);

#endif /* _BCM2835_MAILBOX_H */
