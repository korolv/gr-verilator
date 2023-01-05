/* -*- c++ -*- */
/* 
 * Do not use special Characters in this code!
 */
#include "verilator_tb_axis.h"

#include <verilated.h>
#include <verilated_vcd_c.h>
#include "DesignUnderTest.h"
#include <memory>

// #include <iostream>

#define CAST_DUT(x)   reinterpret_cast<DesignUnderTest*>(x)
#define CAST_TRACE(x) reinterpret_cast<VerilatedVcdC*>(x)


class Init
{
public:
    Init() {
        int argc = 0;
        char** argv = NULL;
        Verilated::commandArgs(argc, argv);
    };
    virtual ~Init() {};
};
static Init init;

extern "C" std::shared_ptr<::verilator::tb::Axis<std::int32_t> > create_int32(float io_ratio)
{
    DesignUnderTest* d_top = new DesignUnderTest();
    return std::make_shared<::verilator::tb::Axis<std::int32_t> >(d_top, io_ratio);
}

namespace verilator {
namespace tb {

template <class T>
Axis<T>::Axis(void* top, float io_ratio):
d_top(top),
d_io_ratio(io_ratio),
gr({0}),
d_time(0),
d_time_per_clock(1)
{
    assert(d_top != nullptr);

#ifdef DUT_TRACE
    Verilated::traceEverOn(true);
    d_trace = new VerilatedVcdC();
    CAST_DUT(d_top)->trace(CAST_TRACE(d_trace), 5);
    CAST_TRACE(d_trace)->open("/home/devel/Developments/gr-iverilog/build/waveform.vcd");
#endif

    CAST_DUT(d_top)->clock = 0;
    CAST_DUT(d_top)->eval();
}

template <class T>
Axis<T>::~Axis()
{
    CAST_TRACE(d_trace)->close();

    delete CAST_DUT(d_top);
    delete CAST_TRACE(d_trace);
}

template <class T>
void Axis<T>::forecast(int noutput_items,
                        std::vector<int>& ninput_items_required)
{
    ninput_items_required[0] = std::round(d_io_ratio * noutput_items);
}

template <class T>
WorkResult Axis<T>::general_work(int noutput_items,
                                std::vector<int>& ninput_items,
                                std::vector<const void*>& input_items,
                                std::vector<void*>& output_items)
{
    DesignUnderTest* dut = CAST_DUT(d_top);
    const T* in = static_cast<const T*>(input_items[0]);
    T* out = static_cast<T*>(output_items[0]);

    WorkResult result;
    result.noutput_items = 0;
    bool one_more_input;

    do {
        gr.m_axis_tvalid = (ninput_items[0] > result.items.input);
        if (gr.m_axis_tvalid)
            gr.m_axis_tdata = in[result.items.input];
        gr.s_axis_tready = (noutput_items > result.items.output);

        /* Assignment inputs */
        dut->m_axis_tready = gr.s_axis_tready;
        dut->s_axis_tvalid = gr.m_axis_tvalid;
        dut->s_axis_tdata = gr.m_axis_tdata;

        /* A cycle */
        dut->clock = 1;
        dut->eval();
#ifdef DUT_TRACE
        CAST_TRACE(d_trace)->dump(d_time);
        d_time += d_time_per_clock;
#endif
        dut->clock = 0;
        dut->eval();
#ifdef DUT_TRACE
        CAST_TRACE(d_trace)->dump(d_time);
        d_time += d_time_per_clock;
#endif

        /* Assignment outputs */
        gr.m_axis_tready = dut->s_axis_tready;
        gr.s_axis_tvalid = dut->m_axis_tvalid;
        gr.s_axis_tdata = dut->m_axis_tdata;

        /* Resulting */
        one_more_input = gr.m_axis_tvalid && gr.m_axis_tready;
        if (one_more_input) {
            result.items.input++;
        }

        if (gr.s_axis_tvalid && gr.s_axis_tready) {
            out[result.items.output] = gr.s_axis_tdata;
            result.items.output++;
        }

    } while (one_more_input || gr.s_axis_tvalid);

    return result;
}

}
}
