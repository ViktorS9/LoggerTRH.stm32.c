   .text
   .syntax unified
   .thumb
   .extern _tn_curr_run_task
   .extern _tn_next_task_to_run
   .extern _tn_sys_on_context_switch
   .global ffs_asm
   .global PendSV_Handler
   .global SVC_Handler
   .global _tn_arch_sys_start
   .global _tn_arch_context_switch_now_nosave
   .global tn_arch_int_dis
   .global tn_arch_int_en
   .global tn_arch_sr_save_int_dis
   .global tn_arch_sr_restore
   .global _tn_arch_is_int_disabled
   .global _tn_arch_inside_isr
   .global _tn_arch_context_switch_pend
   .global tn_arch_sched_dis_save
   .global tn_arch_sched_restore
.equ ICSR_ADDR, 0xE000ED04
.equ PENDSVSET, 0x10000000
.equ PR_08_11_ADDR, 0xE000ED1C
.equ PR_12_15_ADDR, 0xE000ED20
.equ PENDS_VPRIORITY, 0x00FF0000
.equ SVC_VPRIORITY, 0xFF000000
.equ FPU_FPCCR_ADDR, 0xE000EF34
.equ FPU_FPCCR_LSPEN, 0xBFFFFFFF
.thumb_func
ffs_asm:
      mov r1, r0
      rsbs r0, r1, #0
      ands r0, r0, r1
      clz r0, r0
      rsb r0, r0, #0x20
      bx lr
.thumb_func
PendSV_Handler:
      cpsid i
      mrs r2, PSP
      stmdb r2!, {r4-r11, lr}
      ldr r5, =_tn_curr_run_task
      ldr r6, =_tn_next_task_to_run
      ldr r0, [r5]
      ldr r1, [r6]
      str r2, [r0]
      bl _tn_sys_on_context_switch
      ldr r4, [r6]
      str r4, [r5]
L__context_restore:
      ldr r0, [r4]
      ldmia r0!, {r4-r11, lr}
      msr PSP, r0
      cpsie i
      bx lr
.thumb_func
_tn_arch_sys_start:
      mrs r2, MSP
      msr PSP, r2
      mrs r2, CONTROL
      movs r3, #0x02
      orrs r2, r2, r3
      msr CONTROL, r2
      isb
      movs r2, #4
      muls r1, r2, r1
      adds r0, r0, r1
      msr MSP, r0
      ldr r1, =PR_12_15_ADDR
      ldr r0, [r1]
      ldr r2, =PENDS_VPRIORITY
      orrs r0, r0, r2
      str r0, [r1]
      ldr r1, =PR_08_11_ADDR
      ldr r0, [r1]
      ldr r2, =SVC_VPRIORITY
      orrs r0, r0, r2
      str r0, [r1]
.thumb_func
_tn_arch_context_switch_now_nosave:
      cpsie i
      svc #0x00
.thumb_func
SVC_Handler:
      cpsid i
      ldr r5, =_tn_curr_run_task
      ldr r6, =_tn_next_task_to_run
      ldr r0, [r5]
      ldr r1, [r6]
      bl _tn_sys_on_context_switch
      ldr r4, [r6]
      str r4, [r5]
      b L__context_restore
.thumb_func
tn_arch_int_dis:
      cpsid i
      bx lr
.thumb_func
tn_arch_int_en:
      cpsie i
      bx lr
.thumb_func
tn_arch_sr_save_int_dis:
      mrs r0, PRIMASK
      cpsid i
      bx lr
.thumb_func
tn_arch_sr_restore:
      msr PRIMASK, r0
      bx lr
.thumb_func
_tn_arch_is_int_disabled:
      mrs r0, PRIMASK
      bx lr
.thumb_func
_tn_arch_inside_isr:
      mrs r0, CONTROL
      tst r0, #0x02
      ite eq
      moveq r0, #1
      movne r0, #0
      bx lr
.thumb_func
_tn_arch_context_switch_pend:
      ldr r1, =ICSR_ADDR
      ldr r0, =PENDSVSET
      str r0, [r1]
      bx lr
.thumb_func
tn_arch_sched_dis_save:
      mrs r0, BASEPRI
      ldr r1, =PR_08_11_ADDR
      ldr r1, [r1]
      lsr r1, r1, #24
      msr BASEPRI_MAX, r1
      bx lr
.thumb_func
tn_arch_sched_restore:
      msr BASEPRI, r0
      bx lr
  
