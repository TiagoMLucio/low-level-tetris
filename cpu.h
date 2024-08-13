
#pragma once

unsigned get_arm_mode(void);
void set_arm_mode(unsigned);
unsigned get_core(void);
void enable_irq(unsigned);
void enable_fiq(unsigned);
void init_core(unsigned, void *);
void enable_cache(void);
void disable_mmu(void);
void enable_mmu(void *);
void flush_tlb(void);
void get_lock(void *, unsigned);
void put_lock(void *);
