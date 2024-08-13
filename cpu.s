
.text
.global delay, set_arm_mode, get_arm_mode
.global get_core, enable_irq, enable_fiq
.global init_core
.global enable_cache
.global disable_mmu, enable_mmu
.global flush_tlb
.global put_lock, get_lock

#
# Retorna o modo atual do processador
#
get_arm_mode:
   mrs r0, cpsr
   and r0, r0, #0b11111
   mov pc, lr

#
# Altera o modo do processador
#
set_arm_mode:
   mrs r1, cpsr
   bic r1, r1, #0b11111
   bic r0, r0, #0b11111
   orr r1, r1, r0
   msr cpsr, r1
   mov pc, lr

#
# Retorna o ID do core em execução (affinity)
#
get_core:
  mrc p15,0,r0,c0,c0,5    // registrador MPIDR
  and r0, r0, #0x03
  mov pc, lr

#
# Modifica o flag I
#
enable_irq:
  cmp r0, #0
  beq disable_irq
  mrs r0, cpsr
  bic r0, r0, #(1 << 7)
  msr cpsr, r0
  mov pc, lr
disable_irq:
  mrs r0, cpsr
  orr r0, r0, #(1 << 7)
  msr cpsr, r0
  mov pc, lr

#
# Modifica o flag F
#
enable_fiq:
  cmp r0, #0
  beq disable_fiq
  mrs r0, cpsr
  bic r0, r0, #(1 << 6)
  msr cpsr, r0
  mov pc, lr
disable_fiq:
  mrs r0, cpsr
  orr r0, r0, #(1 << 6)
  msr cpsr, r0
  mov pc, lr

#
# Inicia execução de um core (r0) a partir do endereço (r1)
#
init_core:
  and r0, r0, #0x03
  ldr r2, =0x400000cc
  mov r3, #-1
  str r3, [r2, r0, lsl #4]
  ldr r2, =0x4000008c
  str r1, [r2, r0, lsl #4]
  sev
  mov pc, lr

#
# Habilita memória cache
#
enable_cache:
   mrc p15, 0, r0, c1, c0, 0
   orr r0, r0, #0x1800           // habilita caches de dados e de instruções e a previsão de desvios
   orr r0, r0, #0x0004
   mcr p15, 0, r0, c1, c0, 0
   mov pc, lr

#
# Desabilita MMU
#
disable_mmu:
  mrc p15, 0, r2, c1, c0, 0
  bic r2, #0x1000
  bic r2, #0x0005
  mcr p15, 0, r2, c1, c0, 0
  dsb
  mov pc, lr

#
# Liga MMU e ajusta TTB com o valor de R0
#
enable_mmu:
  mvn r2, #0
  bic r2, #0xC
  mcr p15, 0, r2, c3, c0, 0
  mcr p15, 0, r0, c2, c0, 0
  mcr p15, 0, r0, c2, c0, 1
  mrc p15, 0, r2, c1, c0, 0
  #orr r2, r2, #0x1000
  orr r2, r2, #0x0005
  mcr p15, 0, r2, c1, c0, 0
  mov pc, lr

#
# Invalida o TLB atual
#
flush_tlb:
  mov r2, #0
  mcr p15, 0, r2, c8, c7, 0
  mcr p15, 0, r2, c7, c10, 4
  dsb
  mov pc, lr

#
# Obtém um spinlock
# r0 = endereço, r1 = valor a escrever
# 
get_lock:
  ldrex r2, [r0]
  cmp r2, #0
  strexeq r2, r1, [r0]
  cmpeq r2, #0
  bne get_lock
  dmb
  mov pc, lr

#
# Devolve o spinlock (r0)
#
put_lock:
  mov r2, #0
  dmb
  str r2, [r0]
  mov pc, lr
