#ifndef MTRACE_H
#define MTRACE_H

#include <stdint.h>
#include <stdio.h>
#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <isa.h>

void read_mtrace(paddr_t addr, int len);
void write_mtrace(paddr_t addr, int len, word_t data);

#endif // MTRACE_H
