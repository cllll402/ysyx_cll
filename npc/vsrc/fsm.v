module fsm(
    input clk,
    input rst,
    input ready,
    input [7:0] out,
    output reg next,
    output reg segen,
    output reg [23:0] segin,
    output reg [3:0] countlow,
    output reg [3:0] counthigh
);
	parameter READ  = 4'b0001;
	parameter REACH = 4'b0010; 
	parameter DELAY = 4'b0100;
	parameter IDLE  = 4'b1000;
	reg [3:0] state_current;
	reg [3:0] state_next;

always @ (posedge clk or posedge rst) begin
	if (rst) begin
		state_current <= IDLE;
	end
	else begin
		state_current <= state_next;
	end
end

always @ (*) begin
	case (state_current)
		IDLE: begin
		state_next = (ready == 1'b1) ? READ : IDLE;
		end
		READ: begin
		state_next = REACH;
		end
		REACH: begin
		state_next = DELAY;
		end
		DELAY: begin
		state_next = IDLE;
		end
		default: begin
		state_next = IDLE;
		end
	endcase
end

always @ (posedge clk) begin
	case (state_current)
		IDLE: begin
		next <= 1;
		end
		REACH: begin
		next <= 0;
		end
		DELAY: begin
		next <= 0;
		end
		READ: begin
		segin <= {segin[15:0],out[7:0]};
		end
		default: begin
		next <= 1;
		end
	endcase
end

always @ (*) begin
	if ((segin[15:8] == 8'hf0) && (segin[7:0] == segin[23:16])) begin
	  segen = 0;
	end
	else begin
	  segen = 1;
	end
end

always @ (negedge segen or posedge rst) begin
	if (rst) begin
		countlow  <= 4'b0;
		counthigh <= 4'b0;
	end
	else begin
		if (countlow == 4'd9) begin
			countlow <= 4'd0;
			counthigh <= counthigh + 4'd1;
		end
		else begin
			countlow <= countlow + 4'd1;
		end
	end
end

endmodule



