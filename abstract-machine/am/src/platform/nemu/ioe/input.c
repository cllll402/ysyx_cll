#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
	uint32_t key_addr = inl(KBD_ADDR);
	kbd->keydown = key_addr & KEYDOWN_MASK ? true : false;
	kbd->keycode = key_addr & ~KEYDOWN_MASK;
}

