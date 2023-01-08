#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
from gnuradio.verilator import axis_ii
import os
import numpy as np


class qa_axis_xx(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_001_t (self):
        N = 10
        i32 = np.iinfo(np.int32)
        src_data = np.random.randint(low=i32.min, high=i32.max, size=N, dtype=np.int32)
        expected_result = src_data.copy()
        src = blocks.vector_source_i(src_data)
        dst = blocks.vector_sink_i()
        path = os.path.dirname(__file__)
        instance = axis_ii(f'{path}/testverilog/axis/gr_tb_axis.v', 1, '')

        self.tb.connect(src, instance)
        self.tb.connect(instance, dst)
        self.tb.run()
        test_result = dst.data()

        self.assertFloatTuplesAlmostEqual(expected_result, test_result, N)

    def test_002_t (self):
        N = 200
        i32 = np.iinfo(np.int32)
        src_data = np.random.randint(low=i32.min, high=i32.max, size=N, dtype=np.int32)
        expected_result = src_data.copy()
        src = blocks.vector_source_i(src_data)
        dst = blocks.vector_sink_i()
        path = os.path.dirname(__file__)
        instance = axis_ii(f'{path}/testverilog/axis/gr_tb_axis.v', 1, '')

        self.tb.connect(src, instance)
        self.tb.connect(instance, dst)
        self.tb.run()
        test_result = dst.data()

        self.assertFloatTuplesAlmostEqual(expected_result, test_result, N)

    def test_003_t (self):
        N = 3000
        i32 = np.iinfo(np.int32)
        src_data = np.random.randint(low=i32.min, high=i32.max, size=N, dtype=np.int32)
        expected_result = src_data.copy()
        src = blocks.vector_source_i(src_data)
        dst = blocks.vector_sink_i()
        path = os.path.dirname(__file__)
        instance = axis_ii(f'{path}/testverilog/axis/gr_tb_axis.v', 1, '')

        self.tb.connect(src, instance)
        self.tb.connect(instance, dst)
        self.tb.run()
        test_result = dst.data()

        self.assertFloatTuplesAlmostEqual(expected_result, test_result, N)

if __name__ == '__main__':
    gr_unittest.run(qa_axis_xx)
