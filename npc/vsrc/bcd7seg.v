module bcd7seg(b,h);
	input [2:0] b;
	output reg [7:0] h;

	always @ (*) begin
		case(b)
			3'b000 : h = 8'b00000010;
			3'b001 : h = 8'b10011110;
			3'b010 : h = 8'b00100100;
			3'b011 : h = 8'b00001100;
			3'b100 : h = 8'b10011000;
			3'b101 : h = 8'b01001000;
			3'b110 : h = 8'b01000000;
			3'b111 : h = 8'b00011110;
			default : h = 8'b11111111;
		endcase
	end
endmodule
