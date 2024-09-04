#include "iringbuf.h"

ItraceNode iringbuf[MAX_IRINGBUF];  

int current_pos = 0;
bool buffer_full = false;

void save_inst(word_t pc, uint32_t inst) {
	iringbuf[current_pos].pc = pc;
	iringbuf[current_pos].inst = inst;
	current_pos = (current_pos + 1) % MAX_IRINGBUF;
	buffer_full = buffer_full || current_pos == 0;
}

void display_inst() {
	if (current_pos == 0 && !buffer_full) return;
	
	char buf[128];
	int show_start_pos = buffer_full ? current_pos : 0;
	extern void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
	printf("Most recently executed instructions\n");

	do {
		sprintf(buf, "%s" FMT_WORD ": %08x ", 
			(show_start_pos == (current_pos - 1 + MAX_IRINGBUF) % MAX_IRINGBUF) ? " --> " : "     ", 
			iringbuf[show_start_pos].pc, 
			iringbuf[show_start_pos].inst);

		disassemble(buf + strlen(buf), sizeof(buf) - strlen(buf), iringbuf[show_start_pos].pc, (uint8_t *)&iringbuf[show_start_pos].inst, 4);

		puts(buf);
		show_start_pos = (show_start_pos + 1) % MAX_IRINGBUF;
	} while (show_start_pos != current_pos);
}

