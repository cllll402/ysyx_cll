module enbcd7(en,x,y);
	input en;	
	input [3:0] x;
	output reg [7:0] y;

	always @ (*) begin
		if (en) begin 
			case(x)
				4'b0000 : y = 8'b00000010;	 //  0
				4'b0001 : y = 8'b10011110;	 //  1
				4'b0010 : y = 8'b00100100;	 //  2
				4'b0011 : y = 8'b00001100;	 //  3
				4'b0100 : y = 8'b10011000;	 //  4
				4'b0101 : y = 8'b01001000;	 //  5
				4'b0110 : y = 8'b01000000;	 //  6
				4'b0111 : y = 8'b00011110;	 //  7
				4'b1000 : y = 8'b00000000;   //  8
				4'b1001 : y = 8'b00001000;   //  9
				default : y = 8'b11111111;
			endcase
		end 
		else begin 
			y = 8'b11111111;
		end
	end
endmodule
