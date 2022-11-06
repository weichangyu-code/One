
#include "coctx.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _MSC_VER
extern "C" void one_coctx_swap(coctx_t* from, coctx_t* to);
#else
extern "C" void one_coctx_swap(coctx_t* from, coctx_t* to) asm("one_coctx_swap");
#endif

// 这个兼容性会好点，但是不能优化
// void one_coctx_swap(coctx_t* from, coctx_t* to)
// {
//     register void* src asm ("rsi");
//     register void* dst asm ("rdi");
//     src = to;
//     dst = from;
//     coctx_swap_register();
// }

void one_coctx_init(coctx_t* ctx)
{
    memset(ctx, 0, sizeof(*ctx));
}

void one_coctx_make(coctx_t* ctx, void* stack, unsigned int size, coctx_pfn_t pfn, const void* s1, const void* s2) 
{
    void* stack_top = (char*)stack + size;
    stack_top = (void*)((intptr_t)stack_top & -16LL);

    ctx->stackBase = stack_top;
    ctx->stackLimit = stack;
    ctx->dealloctionStack = stack;

    stack_top = (char*)stack_top - 64;          //确保16位对齐，因为媒体指令要求16位对齐。预留一部分的空间给pfn局部变量用
    *(void**)stack_top = (void*)pfn;            //存放返回地址

#ifdef _WIN32
    ctx->rcx = (void*)s1;
    ctx->rdx = (void*)s2;
#else
    ctx->rdi = (void*)s1;
    ctx->rsi = (void*)s2;
#endif
    ctx->rsp = stack_top;
}

