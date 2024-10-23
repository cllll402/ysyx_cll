TOP_NAME = ysyx_24090005_top

BUILD_DIR = /home/cll/ysyx/ysyx-workbench/npc/obj_dir
YOSYS_DIR = /home/cll/ysyx/ysyx-workbench/npc/yosys
INCLUDE_DIR = /home/cll/ysyx/ysyx-workbench/npc/csrc/include/

VERILATOR = verilator
VERILATOR_CFLAGS = -MMD --build -cc -O3 --x-assign fast --x-initial fast --noassert --exe --trace --top-module $(TOP_NAME) 
VERILATOR_CFLAGS += -I$(INCLUDE_DIR) 

YOSRC = /home/cll/ysyx/ysyx-workbench/npc/vsrc/top.v
VSRCS = $(shell find /home/cll/ysyx/ysyx-workbench/npc/vsrc -name "*.v")
CSRCS = $(shell find /home/cll/ysyx/ysyx-workbench/npc/csrc -name "*.c" -o -name "*.cc" -o -name "*.cpp")

#NPCFLAGS += -d $(NPC_HOME)/riscv32-nemu-interpreter-so
#NPCFLAGS += -l ./npc_log.txt
#NPCFLAGS += -e $(ELF)
#NPCFLAGS += -b

BIN = $(BUILD_DIR)/$(TOP_NAME)
ELF = $(basename $(IMG)).elf

yosys:
	mkdir -p $(YOSYS_DIR)
	yosys -p "read_verilog $(YOSRC); show -format png -prefix $(YOSYS_DIR)/$(notdir $(basename $(YOSRC)))"

sim: $(VSRCS) $(CSRCS)
	rm -rf $(BUILD_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) $(VSRCS) $(CSRCS) -o $(abspath $(BIN))

run: sim 
	$(BIN) $(NPCFLAGS) $(IMG) 

clean:
	rm -rf $(BUILD_DIR) $(YOSYS_DIR) npc_log.txt mtrace_output.log

.PHONY: sim run yosys clean

