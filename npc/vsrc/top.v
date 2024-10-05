module ysyx_24090005_top(
    input clk,
    input rst,
    output reg [31:0] outdata
);
    wire wen;
    wire [4:0] rd;
    wire [31:0] pc;
    wire [31:0] ins;
    wire [31:0] rs1;
    wire [31:0] rs2;
    wire [31:0] imm;

    ysyx_24090005_ifu ifu(
        .clk(clk),
        .rst(rst),
        .ins(ins),
        .pc(pc)   
    );

    ysyx_24090005_idu idu(
        .clk(clk), 
        .ins(ins), 
        .wen(wen),  
        .rd(rd),
        .rs1(rs1),
        .rs2(rs2),
        .imm(imm)
    );

    ysyx_24090005_alu alu(
    	.rd(rd),
        .clk(clk),
        .imm(imm),
        .rs1(rs1),
        .rs2(rs2),
		.ins(ins),
        .outdata(outdata)
    );

endmodule
