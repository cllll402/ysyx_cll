/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

#define NUM_REGS 32

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
    // Check program counter
    if (ref_r->pc != cpu.pc) {
        printf("\033[31mPC has error\033[0m\n");
        printf("\033[31mExpected: %x, Actual: %x\033[0m\n", ref_r->pc, cpu.pc);
        return false;
    }

    // Check general purpose registers
    for (int i = 0; i < NUM_REGS; i++) {
        if (ref_r->gpr[i] != cpu.gpr[i]) {
            printf("\033[31mGPR has error at reg %d\033[0m\n", i);
            printf("\033[31mExpected: %x, Actual: %x\033[0m\n", ref_r->gpr[i], cpu.gpr[i]);
            return false;
        }
    }

    return true;
}

void isa_difftest_attach() {
    // Functionality to attach differential testing (if needed)
}
