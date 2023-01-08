

module gr_tb_axis_sc16 #
(
    parameter DATA_WIDTH = 32
)
(
    input  wire                   clock,
    input  wire                   reset,

    /*
     * AXI input
     */
    input  wire [31:0]            s_axis_tdata,
    input  wire                   s_axis_tvalid,
    output wire                   s_axis_tready,
    input  wire                   s_axis_tlast,

    /*
     * AXI output
     */
    output wire [31:0]            m_axis_tdata,
    output wire                   m_axis_tvalid,
    input  wire                   m_axis_tready,
    output wire                   m_axis_tlast
);

wire signed [15:0] s_axis_tdata_re, s_axis_tdata_im;
wire signed [15:0] m_axis_tdata_re, m_axis_tdata_im;

assign s_axis_tdata_re = s_axis_tdata[15:0];
assign s_axis_tdata_im = s_axis_tdata[31:16];
assign m_axis_tdata = {m_axis_tdata_im, m_axis_tdata_re};

delay1 #(
    .DATA_WIDTH(16)
)
DUT_re (
    .clk(clock),
    .rst(1'b0),

    /*
     * AXI input
     */
    .s_axis_tdata(s_axis_tdata_re),
    .s_axis_tkeep(),
    .s_axis_tvalid(s_axis_tvalid),
    .s_axis_tready(s_axis_tready),
    .s_axis_tlast(),

    /*
     * AXI output
     */
    .m_axis_tdata(m_axis_tdata_re),
    .m_axis_tkeep(),
    .m_axis_tvalid(m_axis_tvalid),
    .m_axis_tready(m_axis_tready),
    .m_axis_tlast()
),
DUT_im (
    .clk(clock),
    .rst(1'b0),

    /*
     * AXI input
     */
    .s_axis_tdata(s_axis_tdata_im),
    .s_axis_tkeep(),
    .s_axis_tvalid(s_axis_tvalid),
    .s_axis_tready(s_axis_tready),
    .s_axis_tlast(),

    /*
     * AXI output
     */
    .m_axis_tdata(m_axis_tdata_im),
    .m_axis_tkeep(),
    .m_axis_tvalid(m_axis_tvalid),
    .m_axis_tready(m_axis_tready),
    .m_axis_tlast()
);

endmodule
