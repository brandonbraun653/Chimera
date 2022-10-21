include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

gen_static_lib_variants(
  TARGET
    chimera_serial
  SOURCES
    chimera_serial.cpp
  PRV_LIBRARIES
    aurora_intf_inc
    chimera_intf_inc
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Chimera/serial"
)
