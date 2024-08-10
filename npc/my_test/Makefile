TOPNAME = switch

VERILATOR = verilator
VERILATOR_CFLAGS += --build --cc --exe -Wall --trace

BUILD_DIR= /home/cll/ysyx/ysyx-workbench/npc/my_test/obj_dir
VSRCS = /home/cll/ysyx/ysyx-workbench/npc/my_test/vsrc/$(TOPNAME).v
CSRCS = /home/cll/ysyx/ysyx-workbench/npc/my_test/csrc/tb_$(TOPNAME).cpp

default: $(VSRCS) $(CSRCS) 
	rm -rf $(BUILD_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) -j 8 $(CSRCS) $(VSRCS)
	$(call git_commit, "sim RTL")

clean:
	rm -rf $(BUILD_DIR)

.PHONY: default clean

