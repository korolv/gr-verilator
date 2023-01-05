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
    void* top;
    void* trace;
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
    std::uint64_t sim_time;
    unsigned clock_time;

public:
    Axis(void* top);
    virtual ~Axis();

    virtual WorkResult general_work(int noutput_items,
                                    std::vector<int>& ninput_items,
                                    std::vector<const void*>& input_items,
                                    std::vector<void*>& output_items);
};
};

}

#endif /* INCLUDED_VERILATOR_DUT_H */