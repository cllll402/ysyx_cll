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
#include <cpu/cpu.h>
#include <difftest-def.h>
#include <memory/paddr.h>

__EXPORT void difftest_memcpy(paddr_t addr, void *buf, size_t n, bool direction) {
    assert(buf != NULL);  
    void *nemu_buf = (void *)guest_to_host(addr);
    
    if (direction == DIFFTEST_TO_REF) {                        // DUT -> REF
        memcpy(nemu_buf, buf, n);
    } else {  
        memcpy(buf, nemu_buf, n);                              // REF -> DUT
    }
}

__EXPORT void difftest_regcpy(void *dut, bool direction) {
    assert(dut != NULL);  
    CPU_state* reg_p = (CPU_state*)dut;  

    if (direction == DIFFTEST_TO_REF) { 
        memcpy(cpu.gpr, reg_p->gpr, sizeof(cpu.gpr));          // DUT -> REF
        cpu.pc = reg_p->pc;
    } else {               
        memcpy(reg_p->gpr, cpu.gpr, sizeof(cpu.gpr));          // REF -> DUT
        reg_p->pc = cpu.pc;
    }
}


__EXPORT void difftest_exec(uint64_t n) {
  cpu_exec(n);
}

__EXPORT void difftest_raise_intr(word_t NO) {
  assert(0);
}

__EXPORT void difftest_init(int port) {
  void init_mem();
  init_mem();
  /* Perform ISA dependent initialization. */
  init_isa();
}
