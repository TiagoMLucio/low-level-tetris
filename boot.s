
.global start, delay
.text
start:
  /*
   * configura os stack pointers
   */
  mov r0, #0xd3     // Modo SVC
  msr cpsr_c,r0
  ldr sp, =stack_addr

  // Continua executando no modo supervisor (SVC), interrupções desabilitadas

   /*
    * Zera segmento BSS
    */
   ldr r0, =bss_begin
   ldr r1, =bss_end
   mov r2, #0
loop_bss:
   cmp r0, r1
   bge done_bss
   strb r2, [r0], #1
   b loop_bss

done_bss:
  /*
   * Executa a função main
   */
  b main

delay:
   subs r0, r0, #1
   bne delay
   mov pc, lr