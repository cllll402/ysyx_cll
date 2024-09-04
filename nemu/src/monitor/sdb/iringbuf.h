#ifndef __IRINGBUF_H__
#define __IRINGBUF_H__

#include <common.h>

#define MAX_IRINGBUF 21

typedef struct {
	word_t pc;
	uint32_t inst;
} ItraceNode;

extern ItraceNode iringbuf[MAX_IRINGBUF];  

void save_inst(word_t pc, uint32_t inst);
void display_inst();

#endif

