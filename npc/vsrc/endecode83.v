module endecode83(en,x,sign,y);
	input  en;
	input  [7:0] x;
	output reg sign;
	output reg [2:0]y;
	
	always @ (*) begin
		if (en) begin sign = 1'b1;
			casez (x)
				8'b1??????? : y = 3'b111; 
				8'b01?????? : y = 3'b110;
				8'b001????? : y = 3'b101;
				8'b0001???? : y = 3'b100;
				8'b00001??? : y = 3'b011;
				8'b000001?? : y = 3'b010;
				8'b0000001? : y = 3'b001;
				8'b00000001 : y = 3'b000;
				default : y = 3'b111;
			endcase
		end
		else y = 3'b111; 
	end
endmodule



