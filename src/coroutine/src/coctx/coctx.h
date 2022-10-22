#pragma once

typedef void (*coctx_pfn_t)(void* s1, void* s2);

struct coctx_t
{
	void* rax;
	void* rbx;
	void* rcx;
	void* rdx;
	void* rsi;		//
	void* rdi;		//
	void* rsp;
	void* rbp;

	void* r[8];

	void* stackBase;			//支持try catch
	void* stackLimit;
	void* dealloctionStack;
};

/**
 * 线程本身可以认为是一个主协程，调用one_coctx_init就可以
 */
void one_coctx_init(coctx_t* ctx);

/**
 * 创建协程
 * stack必须是动态申请出来的内存，确保大于%gs:0x10，不然_setjmpex会崩溃
 */
void one_coctx_make(coctx_t* ctx, void* stack, unsigned int size, coctx_pfn_t pfn, const void *s1, const void *s2);

/**
 * 协程切换，当前寄存器内容会存入from，并切换至to的寄存器值
 */
extern "C" void one_coctx_swap(coctx_t* from, coctx_t* to);
