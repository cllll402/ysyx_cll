module lfsr (
    input wire clk,                  
    output reg [7:0] lfsr
);

    always @ (posedge clk) begin 
        if (lfsr == 8'b00000000) begin
            lfsr <= 8'b00000001;  
        end 
        else begin
            lfsr <= {lfsr[0] ^ lfsr[2] ^ lfsr[3] ^ lfsr[4], lfsr[7:1]}; 
        end 
    end
endmodule

