#ifndef DTRACE_H
#define DTRACE_H

#include <stdint.h>
#include <stdio.h>
#include <memory/paddr.h>
#include <device/map.h>

void read_dtrace(paddr_t addr, int len, IOMap *map);
void write_dtrace(paddr_t addr, int len, word_t data, IOMap *map);

#endif
