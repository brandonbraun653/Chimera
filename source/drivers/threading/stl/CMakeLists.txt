include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

gen_static_lib_variants(
  TARGET
    chimera_threading_stl
  SOURCES
    stl_mutex.cpp
    stl_semaphore.cpp
    stl_thread.cpp
  PRV_LIBRARIES
    chimera_intf_inc
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Chimera"
)
