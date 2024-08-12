module top(
    input clk,
    input rst,
    input btn,
    input [15:0] sw,
    input ps2_clk,
    input ps2_data,
    input uart_rx,
    output uart_tx,
    output [15:0] ledr,
    output VGA_CLK,
    output VGA_HSYNC,
    output VGA_VSYNC,
    output VGA_BLANK_N,
    output [7:0] VGA_R,
    output [7:0] VGA_G,
    output [7:0] VGA_B,
    output [7:0] seg0,
    output [7:0] seg1,
    output [7:0] seg2,
    output [7:0] seg3,
    output [7:0] seg4,
    output [7:0] seg5,
    output [7:0] seg6,
    output [7:0] seg7
);

assign VGA_CLK = clk;

wire [9:0] h_addr;
wire [9:0] v_addr;
wire [23:0] vga_data;

/* 8位线性反馈移位寄存器 
wire [3:0] lfsr_low;
wire [3:0] lfsr_high;

lfsr my_lfsr(
	.clk(btn),
	.lfsr({lfsr_high,lfsr_low})
);
	
bcd7hex my_bcd7high(
	.x(lfsr_high),
	.y(seg1)
);

bcd7hex my_bcd7low(
	.x(lfsr_low),
	.y(seg0)
);

assign ledr[7:0] = {lfsr_high, lfsr_low};
assign seg2 = 8'b11111111;
assign seg3 = 8'b11111111;
assign seg4 = 8'b11111111;
assign seg5 = 8'b11111111;
assign seg6 = 8'b11111111;
assign seg7 = 8'b11111111;
*/

/* 4位带符号加法器 
alu_4bit my_alu_4bit(
	.a(sw[7:4]),
	.b(sw[3:0]),
	.choose(sw[10:8]),
	.out(ledr[3:0]),
	.zero(ledr[4]),
	.overflow(ledr[5]),
	.carry(ledr[6])
);
*/ //带修改存在错误

/* 8-3优先编码器与七段数码管 
wire [2:0] decode_bcd;

endecode83  my_endecode83(
	.en(sw[8]),
	.x(sw[7:0]),
	.sign(ledr[0]),
	.y(decode_bcd)
);

bcd7seg  my_bcd7seg(
	.b(decode_bcd),
	.h(seg0)
);
assign ledr[6:4] = decode_bcd;
*/

/* 二位四选一选择器 
mux42 my_mux42(
	.x0(sw[15:14]),
	.x1(sw[13:12]),
	.x2(sw[11:10]),
	.x3(sw[9:8]),
	.y(sw[1:0]),
	.f(ledr[1:0])
);
*/

/* 双向开关
switch  my_switch(
    .a(sw[7]),
    .b(sw[6]),
    .f(ledr[15])
);
*/

/* 流水灯 */
light my_light(
	.clk(clk),
	.rst(sw[0]),
	.led(ledr)
);


/*  
led my_led(
    .clk(clk),
    .rst(rst),
    .btn(btn),
    .sw(sw),
    .ledr(ledr)
);
*/

/*
vga_ctrl my_vga_ctrl(
    .pclk(clk),
    .reset(rst),
    .vga_data(vga_data),
    .h_addr(h_addr),
    .v_addr(v_addr),
    .hsync(VGA_HSYNC),
    .vsync(VGA_VSYNC),
    .valid(VGA_BLANK_N),
    .vga_r(VGA_R),
    .vga_g(VGA_G),
    .vga_b(VGA_B)
);
*/

/* 状态机及键盘 
wire next;
wire over;
wire segen;
wire ready;
wire [7:0] out;
wire [7:0] ascii;
wire [23:0] segin;
wire [3:0] countlow; 
wire [3:0] counthigh;
  
fsm my_fsm(
    .clk(clk),
    .rst(sw[0]),
    .ready(ready),
    .out(out),
    .next(next),
    .segin(segin),
    .segen(segen),
    .countlow(countlow),
    .counthigh(counthigh)
);

ps2_keyboard my_ps2keyboard(
	.clk(clk),
	.clrn(~rst), 
	.ps2_clk(ps2_clk),
	.ps2_data(ps2_data),
	.data(out),
	.ready(ready),
	.nextdata_n(next),
	.overflow(over)
);

keycode_to_ascii my_keycode_to_ascii(
	.x(segin[7:0]),
	.y(ascii[7:0])
);

enbcd7hex my_enbcd7hex0_low(
	.en(segen),
	.x(segin[3:0]),
	.y(seg0)
);

enbcd7hex my_enbcd7hex1_high(
	.en(segen),
	.x(segin[7:4]),
	.y(seg1)
);

enbcd7hex my_enbcd7hex2_low(
	.en(segen),
	.x(ascii[3:0]),
	.y(seg3)
);

enbcd7hex my_enbcd7hex3_high(
	.en(segen),
	.x(ascii[7:4]),
	.y(seg4)
);

enbcd7 my_enbcd7_low(
	.en(1'b1),
	.x(countlow),
	.y(seg6)
);

enbcd7 my_enbcd7_high(
	.en(1'b1),
	.x(counthigh),
	.y(seg7)
);
assign seg2 = 8'b11111111;
assign seg5 = 8'b11111111;
*/

/* 
seg my_seg(
    .clk(clk),
    .rst(rst),
    .o_seg0(seg0),
    .o_seg1(seg1),
    .o_seg2(seg2),
    .o_seg3(seg3),
    .o_seg4(seg4),
    .o_seg5(seg5),
    .o_seg6(seg6),
    .o_seg7(seg7)
);

vmem my_vmem(
    .h_addr(h_addr),
    .v_addr(v_addr[8:0]),
    .vga_data(vga_data)
);

uart my_uart(
  .tx(uart_tx),
  .rx(uart_rx)
);
*/
endmodule


