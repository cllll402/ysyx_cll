module top(
    input clk,
    input rst,
    input btn,
    input [10:0] sw,
    input ps2_clk,
    input ps2_data,
    input uart_rx,
    output uart_tx,
    output [7:0] ledr,
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
*/

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
*/

/* 二位四选一选择器
mux42 my_mux42(
	.x0(sw[15:14]),
	.x1(sw[13:12]),
	.x2(sw[11:10]),
	.x3(sw[9:8]),
	.y(sw[7:6]),
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

/*  
led my_led(
    .clk(clk),
    .rst(rst),
    .btn(btn),
    .sw(sw),
    .ledr(ledr)
);
*/

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

/* 状态机及键盘 */
wire [3:0] seg0_low;
wire [3:0] seg0_high;
wire [3:0] seg1_low;
wire [3:0] seg1_high;
wire [3:0] seg1_low_ascii;
wire [3:0] seg1_high_ascii;
wire [3:0] seg2_low;
wire [3:0] seg2_high;

keyboard my_keyboard(
    .clk(clk),
    .resetn(sw[0]),
    .ps2_clk(ps2_clk),
    .ps2_data(ps2_data),
    .seg0({seg0_low,seg0_high}),
    .seg1({seg1_low,seg1_high}),
    .seg2({seg2_low,seg2_high})
);

keycode_to_ascii my_keycode_to_ascii(
	.x({seg1_low,seg1_high}),
	.y({seg1_low_ascii,seg1_high_ascii})
);

bcd7hex my_bcd7hex_seg0_low(
	.x(seg0_low),
	.y(seg1)
);

bcd7hex my_bcd7hex_seg0_high(
	.x(seg0_high),
	.y(seg0)
);

bcd7hex my_bcd7hex_seg1_low(
	.x(seg1_low_ascii),
	.y(seg4)
);

bcd7hex my_bcd7hex_seg1_high(
	.x(seg1_high_ascii),
	.y(seg3)
);

bcd7 my_bcd7_seg2_low(
	.x(seg2_low),
	.y(seg7)
);

bcd7 my_bcd7_seg2_high(
	.x(seg2_high),
	.y(seg6)
);
assign seg2 = 8'b11111111;
assign seg5 = 8'b11111111;

/* seg my_seg(
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
*/

vmem my_vmem(
    .h_addr(h_addr),
    .v_addr(v_addr[8:0]),
    .vga_data(vga_data)
);

uart my_uart(
  .tx(uart_tx),
  .rx(uart_rx)
);

endmodule


