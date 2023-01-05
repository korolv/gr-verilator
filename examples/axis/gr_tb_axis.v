

module gr_tb_axis #
(
    parameter DATA_WIDTH = 64
)
(
    input  wire                   clock,
    input  wire                   reset,

    /*
     * AXI input
     */
    input  wire [DATA_WIDTH-1:0]  s_axis_tdata,
    input  wire                   s_axis_tvalid,
    output wire                   s_axis_tready,
    input  wire                   s_axis_tlast,

    /*
     * AXI output
     */
    output wire [DATA_WIDTH-1:0]  m_axis_tdata,
    output wire                   m_axis_tvalid,
    input  wire                   m_axis_tready,
    output wire                   m_axis_tlast
);

delay1 #(
    .DATA_WIDTH(DATA_WIDTH)
)
DUT (
    .clk(clock),
    .rst(1'b0),

    /*
     * AXI input
     */
    .s_axis_tdata(s_axis_tdata),
    .s_axis_tkeep(),
    .s_axis_tvalid(s_axis_tvalid),
    .s_axis_tready(s_axis_tready),
    .s_axis_tlast(),

    /*
     * AXI output
     */
    .m_axis_tdata(m_axis_tdata),
    .m_axis_tkeep(),
    .m_axis_tvalid(m_axis_tvalid),
    .m_axis_tready(m_axis_tready),
    .m_axis_tlast()
);

endmodule
