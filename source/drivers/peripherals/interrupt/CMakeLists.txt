include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

gen_static_lib_variants(
  TARGET
    chimera_peripheral_interrupt
  SOURCES
    chimera_interrupt.cpp
  PRV_LIBRARIES
    chimera_intf_inc
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Chimera/src"
)

