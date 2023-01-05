/* -*- c++ -*- */
/*
 * Copyright 2023 V.Korol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "axis_impl.h"
#include <gnuradio/io_signature.h>
#include <dlfcn.h>

namespace gr {
namespace verilator {

template <class T>
typename axis<T>::sptr axis<T>::make(const char* libso_filepath, const char* options)
{
    return gnuradio::make_block_sptr<axis_impl<T>>(libso_filepath, options);
}


/*
 * The private constructor
 */
template <class T>
axis_impl<T>::axis_impl(const char* libso_filepath, const char* options)
    :gr::block("verilator_axis",
     gr::io_signature::make(1, 1, sizeof(T)),
     gr::io_signature::make(1, 1, sizeof(T))),
     d_io_ratio(1)
{
    typedef std::shared_ptr<::verilator::tb::Axis<T> > (*Create) ();

    void *handle = dlopen(libso_filepath, RTLD_LAZY);
    assert(handle != nullptr);

    Create create = reinterpret_cast<Create>(dlsym(handle, "create_int32"));
    assert(create != nullptr);

    dut = create();
}

/*
 * Our virtual destructor.
 */
template <class T>
axis_impl<T>::~axis_impl() {}

template <class T>
void axis_impl<T>::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    ninput_items_required[0] = std::round(d_io_ratio * noutput_items);
}

template <class T>
int axis_impl<T>::general_work(int noutput_items,
                            gr_vector_int& ninput_items,
                            gr_vector_const_void_star& input_items,
                            gr_vector_void_star& output_items)
{
    // Do <+signal processing+>
    ::verilator::tb::WorkResult result;
    result = dut->general_work(noutput_items, ninput_items, input_items, output_items);

    int ratio = result.items.output*100;
    ratio /= result.items.input;
    ratio -= d_io_ratio*100;
    if (ratio != 0)
        gr::block::d_logger->warn("Too big ratio difference input/output: {}/{}", result.items.input, result.items.output);

    gr::block::consume_each(result.items.input);

    // Tell runtime system how many output items we produced.
    return result.items.output;
}

template class axis<std::int32_t>;

} /* namespace verilator */
} /* namespace gr */
