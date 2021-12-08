global dump_registers_asm
global entry_point
global number_of_registers

_number_of_registers equ 6

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
        ret