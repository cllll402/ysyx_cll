module alu_4bit (
    input [3:0] a,       
    input [3:0] b,       
    input [2:0] choose,     
    output reg [3:0] out,
    output reg zero,     
    output reg overflow, 
    output reg carry     
);

reg signed [4:0] temp; 

always @(*) begin
    zero = 0;
    overflow = 0;
    carry = 0;
    temp = 5'b00000; 

    case(choose)
        3'b000: begin
            temp = $signed(a) + $signed(b);
            out = temp[3:0];  
            carry = temp[4];  
            overflow = (a[3] == b[3]) && (out[3] != a[3]); 
        end
        
        3'b001: begin
            temp = $signed(a) - $signed(b);
            out = temp[3:0];  
            carry = temp[4];  
            overflow = (a[3] != b[3]) && (out[3] != a[3]); 
        end

        3'b010: begin
            out = ~a;  
            carry = 0;
            overflow = 0;
        end

        3'b011: begin
            out = a & b;  
            carry = 0;
            overflow = 0;
        end

        3'b100: begin
            out = a | b;  
            carry = 0;
            overflow = 0;
        end

        3'b101: begin
            out = a ^ b;  
            carry = 0;
            overflow = 0;
        end

        3'b110: begin
            if ($signed(a) < $signed(b)) begin
                out = 4'b0001; 
            end else begin
                out = 4'b0000; 
            end
            carry = 0;
            overflow = 0;
        end

        3'b111: begin
            if (a == b) begin
                out = 4'b0001; 
            end else begin
                out = 4'b0000; 
            end
            carry = 0;
            overflow = 0;
        end

        default: begin
            out = 4'b0000; 
            carry = 0;
            overflow = 0;
        end
    endcase

    if (out == 4'b0000)
        zero = 1;
end

endmodule

