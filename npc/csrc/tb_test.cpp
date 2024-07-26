#include "Vtest.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#define MAX_TIME 20

vluint64_t sim_time = 0;

int main(int argc, char** argv) {
	
        VerilatedContext* contextp = new VerilatedContext;
    	contextp->commandArgs(argc, argv);
   	contextp->traceEverOn(true);
   	
        Vtest* top = new Vtest{contextp};
        VerilatedVcdC* wave_trace = new VerilatedVcdC;
        
        top->trace(wave_trace,5);
        wave_trace->open("wave.vcd"); 
        
        while (sim_time < MAX_TIME) {
        	int a = rand() & 1;
        	int b = rand() & 1;
		top->a = a;
		top->b = b;
		top->eval();
		printf("a = %d, b = %d, c = %d\n", a, b, top->f);
		wave_trace->dump(sim_time);
		sim_time++;
		assert(top->f == (a ^ b));
        }
        wave_trace->close();
	delete top;
        delete contextp;
        delete wave_trace;
        return 0;
}
