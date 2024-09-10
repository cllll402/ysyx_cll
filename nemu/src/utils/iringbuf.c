#include "iringbuf.h"

#ifdef CONFIG_IRINGBUF
// 声明外部函数
extern void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);

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
    printf("Most recently executed instructions:\n");

    do {
        // 清空 buf
        buf[0] = '\0';

        // 格式化输出指令和 PC
        sprintf(buf, "%s" FMT_WORD ": %08x ", 
            (show_start_pos == (current_pos - 1 + MAX_IRINGBUF) % MAX_IRINGBUF) ? " --> " : "     ", 
            iringbuf[show_start_pos].pc, 
            iringbuf[show_start_pos].inst);

        // 解码指令
        uint8_t code[4];
        memcpy(code, &iringbuf[show_start_pos].inst, sizeof(uint32_t));
        disassemble(buf + strlen(buf), sizeof(buf) - strlen(buf), iringbuf[show_start_pos].pc, code, sizeof(code));

        // 输出格式
        puts(buf);
        show_start_pos = (show_start_pos + 1) % MAX_IRINGBUF;
    } while (show_start_pos != current_pos);
}
#endif
