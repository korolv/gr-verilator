// `timescale 1 ns / 1 ns

module delay1 #
(
    parameter DATA_WIDTH = 8,
    parameter KEEP_WIDTH = ((DATA_WIDTH+7)/8)
)
(
    input  wire                   clk,
    input  wire                   rst,

    /*
     * AXI input
     */
    input  wire [DATA_WIDTH-1:0]  s_axis_tdata,
    input  wire [KEEP_WIDTH-1:0]  s_axis_tkeep,
    input  wire                   s_axis_tvalid,
    output wire                   s_axis_tready,
    input  wire                   s_axis_tlast,

    /*
     * AXI output
     */
    output wire [DATA_WIDTH-1:0]  m_axis_tdata,
    output wire [KEEP_WIDTH-1:0]  m_axis_tkeep,
    output wire                   m_axis_tvalid,
    input  wire                   m_axis_tready,
    output wire                   m_axis_tlast
);

reg [DATA_WIDTH-1:0] delay1_out1 = {DATA_WIDTH{1'b0}};
reg delay1_tvalid = 1'b0;
reg delay1_tlast = 1'b0;

always @(posedge clk or posedge rst) begin
    if (rst == 1'b1) begin
        delay1_out1 <= {DATA_WIDTH{1'b0}};
        delay1_tvalid <= s_axis_tvalid;
        delay1_tlast <= s_axis_tlast;
    end
    else begin
        if (s_axis_tvalid) begin
            delay1_out1 <= s_axis_tdata;
        end
        delay1_tvalid <= s_axis_tvalid & m_axis_tready;
        delay1_tlast <= s_axis_tlast;
    end
end

assign m_axis_tdata = delay1_out1;
assign m_axis_tvalid = delay1_tvalid;
assign m_axis_tlast = delay1_tlast;

assign m_axis_tkeep = {KEEP_WIDTH{1'b1}};
assign s_axis_tready = m_axis_tready;


endmodule
