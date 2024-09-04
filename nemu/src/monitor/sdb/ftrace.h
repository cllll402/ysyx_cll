#ifndef FTRACE_H
#define FTRACE_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>
#include <elf.h>
#include <memory/paddr.h>

// 定义函数信息结构体
typedef struct {
    char func_name[64];  // 函数名称
    uint64_t start;      // 函数起始地址
    size_t size;        // 函数大小
} func_info;

// 声明全局变量
extern func_info* symbol_tbl;

// 函数声明
void init_elf(const char* elf_file, paddr_t global_offset);
func_info* check_func(paddr_t addr);
void trace_func_call(paddr_t pc, paddr_t target);
void trace_func_ret(paddr_t pc);
void read_from_file(FILE *elf, size_t offset, size_t size, void* dest);
void get_str_from_file(FILE *elf, size_t offset, size_t n, void* dest);

#endif // FTRACE_H
