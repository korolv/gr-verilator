file(READ ${TEMPLATE_DUT_SRC} dut_src_content)
file(READ ${TEMPLATE_DUT_HDR} dut_hdr_content)

configure_file(template.py.in ${CMAKE_CURRENT_BINARY_DIR}/template.py @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/template.py DESTINATION ${GR_PYTHON_DIR}/gnuradio/verilator)

add_custom_target(
  copy_templates_for_tests ALL
  COMMAND
    ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/*.py"
    ${CMAKE_BINARY_DIR}/test_modules/gnuradio/verilator/
)
