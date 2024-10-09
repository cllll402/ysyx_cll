#include <stdio.h>
#include "dtrace.h" 
#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <isa.h>
#include <device/map.h>

void read_dtrace(paddr_t addr, int len, IOMap *map) {
    FILE *file = fopen("dtrace_output.log", "a");
    if (file) {
        fprintf(file, "Device: %s  Daddr_read  at  " FMT_PADDR "  len=%d\n", map->name, addr, len);
        fclose(file);
    }
    printf("Device: %s  Daddr_read  at  " FMT_PADDR "  len=%d\n", map->name, addr, len);
}

void write_dtrace(paddr_t addr, int len, word_t data, IOMap *map) {
    FILE *file = fopen("dtrace_output.log", "a");
    if (file) {
        fprintf(file, "Device: %s  Daddr_write at  " FMT_PADDR "  len=%d  data=" FMT_WORD "\n", map->name, addr, len, data);
        fclose(file);
    }
    printf("\033[31mDevice: %s  Daddr_write at  " FMT_PADDR "  len=%d  data=\033[31m" FMT_WORD "\033[0m\n", map->name, addr, len, data);
}
