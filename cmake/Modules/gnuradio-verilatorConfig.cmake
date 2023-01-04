find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_VERILATOR gnuradio-verilator)

FIND_PATH(
    GR_VERILATOR_INCLUDE_DIRS
    NAMES gnuradio/verilator/api.h
    HINTS $ENV{VERILATOR_DIR}/include
        ${PC_VERILATOR_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_VERILATOR_LIBRARIES
    NAMES gnuradio-verilator
    HINTS $ENV{VERILATOR_DIR}/lib
        ${PC_VERILATOR_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-verilatorTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_VERILATOR DEFAULT_MSG GR_VERILATOR_LIBRARIES GR_VERILATOR_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_VERILATOR_LIBRARIES GR_VERILATOR_INCLUDE_DIRS)
