include(CTEmbedded)

gen_static_lib_variants(
  TARGET
    chimera_threading_stl
  SOURCES
    stl_mutex.cpp
    stl_semaphore.cpp
    stl_thread.cpp
  PUB_LIBRARIES
    pthread
  PRV_LIBRARIES
    aurora_intf_inc
    chimera_intf_inc
  EXPORT_DIR
    "${PROJECT_BINARY_DIR}/Chimera"
)
