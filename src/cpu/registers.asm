global dump_registers_asm
global entry_point
global number_of_registers

_number_of_registers equ 15

section .data:
    number_of_registers: db _number_of_registers
    counter: db 0
    entry_point:
        resb 8 * _number_of_registers
section .text:
    count:
        push rax
        mov rax, [counter]
        add rax, 8
        mov [counter], rax
        pop rax
        ret
    dump_registers_asm:
        
        mov [entry_point], rax
        mov [entry_point + 8], rbx
        mov [entry_point + 16], rcx
        mov [entry_point + 24], rdx
        mov [entry_point + 32], rdi
        mov [entry_point + 40], rsi
        mov [entry_point + 48], rbp
        mov [entry_point + 56], r8
        mov [entry_point + 64], r9
        mov [entry_point + 72], r10
        mov [entry_point + 80], r11
        mov [entry_point + 88], r12
        mov [entry_point + 94], r13
        mov [entry_point + 98], r14
        mov [entry_point + 102], r15
        ret