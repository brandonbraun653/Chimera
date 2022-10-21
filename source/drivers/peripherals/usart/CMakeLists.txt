include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

gen_static_lib_variants(
  TARGET
    chimera_peripheral_usart
  SOURCES
    chimera_usart.cpp
  PRV_LIBRARIES
    aurora_intf_inc
    chimera_intf_inc
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Chimera/usart"
)
