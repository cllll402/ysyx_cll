# Makefile for simulation

CSRC_DIR=/home/cll/ysyx-workbench/npc/csrc
VSRC_DIR=/home/cll/ysyx-workbench/npc/vsrc

CPP_FILE=sim_test.cpp
VERILOG_FILE=test.v

VERILATOR=verilator

TARGET=sim_test

sim: 
	$(VERILATOR) -Wall --cc --exe --build $(CSRC_DIR)/$(CPP_FILE) $(VSRC_DIR)/$(VERILOG_FILE) -o $(TARGET)

clean:	
	rm -rf obj_dir $(TARGET) $(TARGET).d $(TARGET).mk $(TARGET).cpp

all: sim
.PHONY: all sim clean

#$(call git_commit, "sim RTL")
