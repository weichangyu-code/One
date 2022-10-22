
.globl one_coctx_swap
one_coctx_swap:
    movq %rax, (%rcx)
    movq %rbx, 8(%rcx)
    movq %rsi, 32(%rcx)
    movq %rdi, 40(%rcx)
    movq %rsp, 48(%rcx)
    movq %rbp, 56(%rcx)
    movq %r8,  64(%rcx)
    movq %r9,  72(%rcx)
    movq %r10, 80(%rcx)
    movq %r11, 88(%rcx)
    movq %r12, 96(%rcx)
    movq %r13, 104(%rcx)
    movq %r14, 112(%rcx)
    //movq %r15, 120(%rcx)

    movq %gs:(0x30), %rax
    movq 0x08(%rax), %rbx
    movq %rbx, 128(%rcx)
    movq 0x10(%rax), %rbx
    movq %rbx, 136(%rcx)
    movq 0x1478(%rax), %rbx
    movq %rbx, 144(%rcx)

    movq 128(%rdx), %rbx
    movq %rbx, 0x08(%rax)
    movq 136(%rdx), %rbx
    movq %rbx, 0x10(%rax)
    movq 144(%rdx), %rbx
    movq %rbx, 0x1478(%rax)

    movq (%rdx), %rax
    movq 8(%rdx), %rbx
    movq 32(%rdx), %rsi
    movq 40(%rdx), %rdi
    movq 48(%rdx), %rsp
    movq 56(%rdx), %rbp
    movq 64(%rdx), %r8
    movq 72(%rdx), %r9
    movq 80(%rdx), %r10
    movq 88(%rdx), %r11
    movq 96(%rdx), %r12
    movq 104(%rdx), %r13
    movq 112(%rdx), %r14
    //movq 120(%rdx), %r15
    
    movq 16(%rdx), %rcx
    movq 24(%rdx), %rdx

	ret
