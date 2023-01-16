#ifndef INCLUDED_VERILATOR_DUT_H
#define INCLUDED_VERILATOR_DUT_H

#include <vector>
#include <cstdint>

namespace verilator {
namespace tb {

union WorkResult {
    int noutput_items;
    struct {
        std::int16_t output;
        std::int16_t input;
    } items;
};

template <class T>
class Axis
{
private:
    void* d_top;
    void* d_trace;
    float d_io_ratio;
    struct {
        std::int8_t s_axis_tvalid;
        std::int8_t s_axis_tready;
        std::int8_t s_axis_tlast;
        std::int8_t m_axis_tvalid;
        std::int8_t m_axis_tready;
        std::int8_t m_axis_tlast;
        T s_axis_tdata;
        T m_axis_tdata;
    } gr;
    std::uint64_t d_time;
    unsigned d_time_per_clock;
    unsigned d_iteration_limit;

public:
    Axis(void* top, float io_ratio);
    virtual ~Axis();

    virtual void forecast(int noutput_items,
                          std::vector<int>& ninput_items_required);
    virtual WorkResult general_work(int noutput_items,
                                    std::vector<int>& ninput_items,
                                    std::vector<const void*>& input_items,
                                    std::vector<void*>& output_items);
    virtual void set_iteration_limit(unsigned limit);
};
};

}

#endif /* INCLUDED_VERILATOR_DUT_H */