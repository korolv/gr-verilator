#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2023 V.Korol.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#



from . import verilator_python as pybind
from gnuradio import gr
import os
import tempfile
import subprocess
import shutil
from . import template


WRAPPER_H = 'verilator_tb_axis.h'
WRAPPER_CPP = 'verilator_tb_axis.cpp'
OBJECT_DIRECTORY = 'obj_dir'
CPP_CLASS_PREFIX = 'DesignUnderTest'
OBJECT_SO = 'dut.so'


class WorkingDirectory(object):
     def __init__(self, path):
          self._path = path
          self._origin = os.getcwd()

     def __enter__(self):
            os.chdir(self._path)
     def __exit__(self, exc_type, exc_val, exc_tb):
            os.chdir(self._origin)


class axis_xx(object):
    def __init__(self, verilog_file_path, io_ratio, verilator_options):
        self.logger = gr.logger(self.alias())
        self.data_width = 64
        self.heart = None
        self.trace_file = None

        ##################################################
        # Parameters
        ##################################################
        self.verilog = verilog_file_path
        self.io_ratio = io_ratio
        self.verilator_options = verilator_options

    def build(self):
        verilog_dirpath = os.path.dirname(self.verilog)

        with tempfile.TemporaryDirectory() as build_dirpath:
            with open(f'{build_dirpath}/{WRAPPER_H}','w') as f:
                f.write(template.h.content)

            with open(f'{build_dirpath}/{WRAPPER_CPP}','w') as f:
                f.write(template.cpp.content)

            # Build shared library
            with WorkingDirectory(build_dirpath):
                verilator_args = [
                    'verilator',
                    '-Mdir', OBJECT_DIRECTORY,
                    '--prefix', CPP_CLASS_PREFIX,
                    '--cc',
                    '--exe',
                    f'-GDATA_WIDTH={self.data_width}',
                    '-o', OBJECT_SO,
                    '-y', verilog_dirpath,
                    self.verilog,
                    WRAPPER_CPP
                ]
                if (self.trace_file):
                    verilator_args.extend(['-CFLAGS', '-fPIC --std=c++11 -Wall -DDUT_TRACE', '--trace'])
                else:
                    verilator_args.extend(['-CFLAGS', '-fPIC --std=c++11 -Wall'])
                subprocess.run(verilator_args, stdout=subprocess.PIPE, check=True)

                make_args = [
                    'make',
                    '-j',
                    '-C', OBJECT_DIRECTORY,
                    '-f', f'{CPP_CLASS_PREFIX}.mk'
                ]
                make_env = {**os.environ, 'USER_LDFLAGS': '-shared'}
                self.logger.info(f'Building {OBJECT_SO}. It takes a while...')
                subprocess.run(make_args, env=make_env, stdout=subprocess.PIPE, check=True)

                libso_filepath = os.path.join(build_dirpath, OBJECT_DIRECTORY, OBJECT_SO)
                if not os.path.exists(libso_filepath):
                    raise Exception(f'Expects "{libso_filepath}"')

                self.heart = self.new_axis(libso_filepath)

    # Must be overrided
    def new_axis(self, libso_filepath):
        raise Exception(f'Undefined type')


class axis_ii(gr.hier_block2, axis_xx):
    """
    Axi-stream instance for int32 data type
    """
    def __init__(self, verilog_file_path, io_ratio, verilator_options):
        gr.hier_block2.__init__(
            self,
            "Verilator AXI-Stream",
            gr.io_signature(1, 1, gr.sizeof_int),
            gr.io_signature(1, 1, gr.sizeof_int)
        )
        axis_xx.__init__(
            self,
            verilog_file_path,
            io_ratio,
            verilator_options,
        )
        self.data_width = 32
        self.build()

        ##################################################
        # Conections
        ##################################################
        self.connect((self, 0), (self.heart, 0))
        self.connect((self.heart, 0), (self, 0))

    def new_axis(self, libso_filepath):
        return pybind.axis_ii(
            libso_filepath,
            self.verilator_options
        )
