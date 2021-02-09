include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

gen_static_lib_variants(
  TARGET
    chimera_threading_freertos
  SOURCES
    freertos_hooks.cpp
    freertos_mutex.cpp
    freertos_queue.cpp
    freertos_semaphore.cpp
    freertos_thread.cpp
  PRV_LIBRARIES
    chimera_intf_inc
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Chimera"
)
