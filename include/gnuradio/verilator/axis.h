/* -*- c++ -*- */
/*
 * Copyright 2023 V.Korol.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_VERILATOR_AXIS_H
#define INCLUDED_VERILATOR_AXIS_H

#include <gnuradio/block.h>
#include <gnuradio/verilator/api.h>
#include <cstdint>

namespace gr {
namespace verilator {

/*!
 * \brief <+description of block+>
 * \ingroup verilator
 *
 */
template <class T>
class VERILATOR_API axis : virtual public gr::block
{
public:
    typedef std::shared_ptr<axis<T> > sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of verilator::axis.
     *
     * To avoid accidental use of raw pointers, verilator::axis's
     * constructor is in a private implementation
     * class. verilator::axis::make is the public interface for
     * creating new instances.
     */
    static sptr make(const char* libso_filepath, const char* options);
};

typedef axis<std::int32_t> axis_ii;

} // namespace verilator
} // namespace gr

#endif /* INCLUDED_VERILATOR_AXIS_H */
