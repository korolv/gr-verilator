/* -*- c++ -*- */
/*
 * Copyright 2023 V.Korol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_VERILATOR_AXIS_IMPL_H
#define INCLUDED_VERILATOR_AXIS_IMPL_H

#include "verilator_tb_axis.h"
#include <gnuradio/verilator/axis.h>

namespace gr {
namespace verilator {

template <class T>
class axis_impl : public axis<T>
{
private:
    const std::string d_libso_filepath;
    float d_io_ratio;
    std::string d_verilator_options;

    std::shared_ptr<::verilator::tb::Axis<std::int32_t> > d_dut;

public:
    axis_impl(const char* libso_filepath, const char* options);
    ~axis_impl();

    // Where all the action really happens
    void forecast(int noutput_items, gr_vector_int& ninput_items_required) override;

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items) override;
};

} // namespace verilator
} // namespace gr

#endif /* INCLUDED_VERILATOR_AXIS_IMPL_H */
