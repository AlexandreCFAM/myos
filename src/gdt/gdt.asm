
[bits 64]

global GDTR

GDTR:
    resb 8
LoadGDT:   
    mov [GDTR], rdi
    lgdt [rdi]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    pop rdi
    mov rax, 0x08
    push rax
    push rdi
    retfq
GLOBAL LoadGDT