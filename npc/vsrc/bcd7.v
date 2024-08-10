module bcd7(x,y);
	input [3:0] x;
	output reg [7:0] y;

	always @ (*) begin
		case(x)
			4'b0001 : y = 8'b10011110;
			4'b0010 : y = 8'b00100100;
			4'b0011 : y = 8'b00001100;
			4'b0100 : y = 8'b10011000;
			4'b0101 : y = 8'b01001000;
			4'b0110 : y = 8'b01000000;
			4'b0111 : y = 8'b00011110;
			4'b1000 : y = 8'b00000000;   //  8
			default : y = 8'b00000010;
		endcase
	end
endmodule
