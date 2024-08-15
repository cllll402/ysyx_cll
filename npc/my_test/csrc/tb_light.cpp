#include "Vlight.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#define MAX_TIME 500

vluint64_t sim_time = 0;
Vlight* top = nullptr; 
VerilatedVcdC* wave_trace = nullptr;

void single_cycle() {
    top->clk = 1; 
    top->eval();
    sim_time++;
    wave_trace->dump(sim_time); 

    top->clk = 0; 
    top->eval();
    sim_time++;
    wave_trace->dump(sim_time); 
}

void reset(int n) {
    top->rst = 1;
    while (n-- > 0) 
        single_cycle();
    top->rst = 0;
}

int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    contextp->traceEverOn(true);

    top = new Vlight{contextp};  
    wave_trace = new VerilatedVcdC;
    
    top->trace(wave_trace, 5);
    wave_trace->open("wave.vcd"); 

    reset(20);

    while (sim_time < MAX_TIME) {
        single_cycle();
    }

    wave_trace->close();
    delete wave_trace;
    delete top;
    delete contextp;
    return 0;
}

