#include <stdio.h>
#include "mtrace.h" 
#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <isa.h>

void read_mtrace(paddr_t addr, int len) {
    FILE *file = fopen("mtrace_output.log", "a");
    if (file) {
        fprintf(file, "Paddr_read  at  " FMT_PADDR "  len=%d\n", addr, len);
        fclose(file);
    }
    printf("Paddr_read  at  " FMT_PADDR "  len=%d\n", addr, len);
}

void write_mtrace(paddr_t addr, int len, word_t data) {
    FILE *file = fopen("mtrace_output.log", "a");
    if (file) {
        fprintf(file, "Paddr_write at  " FMT_PADDR "  len=%d  data=" FMT_WORD "\n", addr, len, data);
        fclose(file);
    }
    printf("\033[31mPaddr_write at  " FMT_PADDR "  len=%d  data=\033[31m" FMT_WORD "\033[0m\n", addr, len, data);
}
