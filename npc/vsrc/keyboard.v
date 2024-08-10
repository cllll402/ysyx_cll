module keyboard (
    input clk,
    input resetn,
    input ps2_clk,
    input ps2_data,
    output reg [7:0] seg0,
    output reg [7:0] seg1,
    output reg [7:0] seg2
);

    reg [9:0] buffer; 
    reg [3:0] count;  
    reg [2:0] ps2_clk_sync;
    reg [1:0] state_current;
    reg [1:0] state_next;
    reg [7:0] key_count; 
    reg key_down;
    reg key_up;
    reg key_recorded;
    
    typedef enum reg [1:0] {
        IDLE  = 2'b00,
        READ  = 2'b01,
        SHOW  = 2'b10
    } state_t;

    always @(posedge clk) begin
        ps2_clk_sync <= {ps2_clk_sync[1:0], ps2_clk};
    end

    wire sampling = ps2_clk_sync[2] & ~ps2_clk_sync[1];

    // 状态机的下一个状态逻辑
    always @(*) begin
        case (state_current)
            IDLE : state_next = (key_down && !key_up && !key_recorded) ? READ : IDLE;
            READ : state_next = (count == 4'd10) ? SHOW : READ;
            SHOW : state_next = (key_up) ? IDLE : SHOW;
            default: state_next = IDLE;
        endcase
    end

    always @(posedge clk or posedge resetn) begin
        if (resetn) begin
            seg0 <= 8'b00000000;
            seg1 <= 8'b00000000;
            seg2 <= 8'b00000000;
            state_current <= IDLE;
            key_down <= 0;
            key_up <= 0;
            count <= 0;
            key_count <= 0;
            key_recorded <= 0;
        end 
        
        else begin
            state_current <= state_next;

            case (state_current)
                IDLE: begin 
                    if (ps2_data == 0) begin
                        key_down <= 1; 
                        key_up <= 0;
                        key_recorded <= 0;
                    end
                    count <= 0;
                end
                
                READ: begin
                    if (sampling) begin
                        if (count == 4'd10) begin
                            state_current <= state_next;
                        end 
                        else begin
                            buffer[count] <= ps2_data;
                            count <= count + 1;
                        end
                    end
                end
                
                SHOW: begin
                    if ((buffer[0] == 0) && (ps2_data == 1) && (^buffer[9:1]) && (key_down)) begin  
                        if (!key_recorded) begin
                            key_count <= key_count + 8'b00000001;
                            seg0 <= buffer[8:1];  // 键码
                            seg1 <= buffer[8:1];  // ASCII
                            seg2 <= key_count[7:0];  // 计数
                            key_recorded <= 1;
                        end
                        key_up <= 1;  
                        count <= 0;
                        key_down <= 0;
                    end
                    else begin
                        key_up <= 0;
                        key_recorded <= 0;
                    end
                end
                
                default: begin
                    state_current <= IDLE;        
                end
            endcase
        end
    end
endmodule
