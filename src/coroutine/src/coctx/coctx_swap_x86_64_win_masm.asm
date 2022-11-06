
; standard C library function
.code
one_coctx_swap PROC

    mov [rcx], rax
    mov [rcx+8], rbx
    mov [rcx+16], rcx
    mov [rcx+24], rdx
    mov [rcx+32], rsi
    mov [rcx+40], rdi
    mov [rcx+48], rsp
    mov [rcx+56], rbp
    mov [rcx+64], r8
    mov [rcx+72], r9
    mov [rcx+80], r10
    mov [rcx+88], r11
    mov [rcx+96], r12
    mov [rcx+104], r13
    mov [rcx+112], r14
    mov [rcx+120], r15
    
    mov rax, gs:[030h]
    mov rbx, [rax+08h]
    mov [rcx+128], rbx
    mov rbx, [rax+010h]
    mov [rcx+136], rbx
    mov rbx, [rax+01478h]
    mov [rcx+144], rbx

    mov rbx, [rdx+128]
    mov [rax+08h], rbx
    mov rbx, [rdx+136]
    mov [rax+010h], rbx
    mov rbx, [rdx+144]
    mov [rax+01478h], rbx

    mov rax, [rdx]
    mov rbx, [rdx+8]
    mov rsi, [rdx+32]
    mov rdi, [rdx+40]
    mov rsp, [rdx+48]
    mov rbp, [rdx+56]
    mov r8, [rdx+64]
    mov r9, [rdx+72]
    mov r10, [rdx+80]
    mov r11, [rdx+88]
    mov r12, [rdx+96]
    mov r13, [rdx+104]
    mov r14, [rdx+112]
    mov r15, [rdx+120]
    
    mov rcx, [rdx+16]
    mov rdx, [rdx+24]

	ret

one_coctx_swap ENDP
END
