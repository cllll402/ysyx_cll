TOP_NAME = ysyx_24090005_top

BUILD_DIR = /home/cll/ysyx/ysyx-workbench/npc/obj_dir
YOSYS_DIR = /home/cll/ysyx/ysyx-workbench/npc/yosys
INCLUDE_DIR = /home/cll/ysyx/ysyx-workbench/npc/csrc/include/

BIN = $(BUILD_DIR)/$(TOP_NAME)

VERILATOR = verilator
VERILATOR_CFLAGS = -MMD --build -cc -O3 --x-assign fast --x-initial fast --noassert --exe --trace --top-module $(TOP_NAME) 
VERILATOR_CFLAGS += -I$(INCLUDE_DIR)

YOSYS_SRC = /home/cll/ysyx/ysyx-workbench/npc/vsrc/top.v
VSRCS = $(shell find $(abspath /home/cll/ysyx/ysyx-workbench/npc/vsrc) -name "*.v")
CSRCS = $(shell find $(abspath /home/cll/ysyx/ysyx-workbench/npc/csrc) -name "*.c" -o -name "*.cc" -o -name "*.cpp")

yosys:
	@mkdir -p $(YOSYS_DIR)
	yosys -p "read_verilog $(YOSYS_SRC); show -format png -prefix $(YOSYS_DIR)/$(notdir $(basename $(YOSYS_SRC)))"

sim: $(VSRCS) $(CSRCS)
	@rm -rf $(BUILD_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) $(VSRCS) $(CSRCS) -o $(abspath $(BIN))

run: sim
	$(BIN) $(IMG)

clean:
	rm -rf $(BUILD_DIR) $(YOSYS_DIR)

.PHONY: sim run yosys clean

