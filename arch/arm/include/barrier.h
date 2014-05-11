#ifndef _ARM_BARRIER_H
#define _ARM_BARRIER_H

/*
 * Data sync barrier.
 * No instruction after the DSB can run until all instructions before it have
 * completed.
 */
static inline void data_sync_barrier() 
{ __asm__ volatile ("mcr p15, #0, %[zero], c7, c10, #4" :: [zero] "r" (0)); }

/*
 * Data memory barrier.
 * No instruction after the DSB can run until all instructions before it have
 * completed.
 */
static inline void data_memory_barrier() 
{ __asm__ volatile ("mcr p15, #0, %[zero], c7, c10, #5" :: [zero] "r" (0)); }

#endif /* _ARM_BARRIER_H */
