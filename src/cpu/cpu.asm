global store_result
global detect_cpuid
global cpuid_command

store_result: resq 4

output_cpuid:
    resb 8

detect_cpuid:
    pushf
    pushf
    xor dword [esp],0x00200000
    popf
    pushf
    pop rax
    xor rax,[esp]
    popf
    and rax,0x00200000
    mov [store_result], rax
    ret

cpuid_command:
    mov rax, rdi
    cpuid
    mov [output_cpuid], rbx
    mov [output_cpuid + 4], rdx
    mov [output_cpuid + 8], rcx
    mov rax, output_cpuid
    ret