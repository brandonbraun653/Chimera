include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

gen_static_lib_variants(
  TARGET
    chimera_peripheral_dma
  SOURCES
    chimera_dma.cpp
    chimera_dma_util.cpp
  PRV_LIBRARIES
    chimera_intf_inc
    aurora_intf_inc
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Chimera/src/dma"
)
