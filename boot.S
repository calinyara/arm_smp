.global _start
.section .text.boot, "ax"

_start:
    mrs x0, mpidr_el1
    and x0, x0, #3

    mov x6, x0
    mov x1, #0x0
    add x2, x1, #0x400000
    lsl x6, x6, #18
    sub x3, x2, x6
    mov sp, x3

    bl uart_init

    mrs x0, mpidr_el1
    and x0, x0, #3
    cbz x0, core0
    cmp x0, #1
    b.eq core1
    cmp x0, #2
    b.eq core2
    cmp x0, #3
    b.eq core3

core0:
    bl print0
    b hang

core1:
    bl print1
    b hang

core2:
    bl print2
    b hang

core3:
    bl print3
    b hang

hang:
    wfi
    b hang

