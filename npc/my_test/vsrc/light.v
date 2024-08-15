module light(
	input clk,
	input rst,
	output reg [15:0] led
);
	
	always @ (posedge clk) begin
		if (rst) begin 
			led <= 1;
		end
		else begin
			led <= {led[14:0], led[15]};
		end
	end
endmodule
