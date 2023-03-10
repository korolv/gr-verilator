/* -*- c++ -*- */
/* 
 * Do not use special Characters in this code!
 */

#include <verilated.h>
#include "DesignUnderTest.h"
#include <memory>
#ifdef DUT_TRACE
#include <verilated_vcd_c.h>
#define stringer0(x) #x
#define stringer(x) stringer0(x)
#endif
#include "verilator_tb_axis.h"

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
d_time_per_clock(1),
d_iteration_limit(300)
{
    assert(d_top != nullptr);

#ifdef DUT_TRACE
    Verilated::traceEverOn(true);
    d_trace = new VerilatedVcdC();
    CAST_DUT(d_top)->trace(CAST_TRACE(d_trace), 5);
    CAST_TRACE(d_trace)->open(stringer(DUT_TRACE));
#endif

    CAST_DUT(d_top)->clock = 0;
    CAST_DUT(d_top)->eval();
}

template <class T>
Axis<T>::~Axis()
{
#ifdef DUT_TRACE
    CAST_TRACE(d_trace)->close();
    delete CAST_TRACE(d_trace);
#endif

    delete CAST_DUT(d_top);
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
    int ninput_items_required = std::round(d_io_ratio * noutput_items);
    unsigned niteration = 0;

    do {
        gr.m_axis_tvalid = (ninput_items_required > result.items.input);
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
        if (gr.m_axis_tvalid && gr.m_axis_tready) {
            result.items.input++;
        }

        if (gr.s_axis_tvalid && gr.s_axis_tready) {
            out[result.items.output] = gr.s_axis_tdata;
            result.items.output++;
        }

    } while (gr.s_axis_tready && (niteration++ < d_iteration_limit));

    return result;
}

template <class T>
void Axis<T>::set_iteration_limit(unsigned limit)
{
    d_iteration_limit = limit;
}

} // namespace tb
} // namespace verilator
