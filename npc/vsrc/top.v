module ysyx_24090005_top (
    input wire clk,                // 时钟信号
    input wire rst,                // 复位信号
    output wire [31:0] pc,         // 输出的PC值
    output wire [31:0] ins,        // 输出的指令
    output wire [31:0] reg_data    // 输出寄存器数据
);

    // IDU -> 输出控制信号和操作数
    wire [6:0] opcode;
    wire [4:0] rs1, rs2, rd;
    wire [2:0] funct3;
    wire [6:0] funct7;
    wire [31:0] imm;

    // EXU -> 输出执行结果和跳转控制
    wire [31:0] result;
    wire [31:0] reg_data1;
    wire [31:0] reg_data2;
    wire jump_en;
    wire [31:0] jump_addr;

    // 实例化取指模块
    ysyx_24090005_ifu ifu (
        .clk(clk),
        .rst(rst),
        .jump_en(jump_en),         
        .jump_addr(jump_addr),
        .pc(pc),
        .ins(ins)
    );

    // 实例化指令译码模块
    ysyx_24090005_idu idu (
        .ins(ins),
        .opcode(opcode),
        .rs1(rs1),
        .rs2(rs2),
        .rd(rd),
        .funct3(funct3),
        .funct7(funct7),
        .imm(imm)
    );

    // 实例化执行模块
    ysyx_24090005_exu exu (
        .clk(clk),
        .rd(rd),
        .rs1(rs1),
        .rs2(rs2),
        .imm(imm),
        .opcode(opcode),
        .funct3(funct3),
        .funct7(funct7),
        .pc(pc),
        .result(result),
        .reg_data1(reg_data1),
        .reg_data2(reg_data2),
        .rdata(reg_data),           
        .jump(jump_en),
        .jump_addr(jump_addr)
    );

endmodule

