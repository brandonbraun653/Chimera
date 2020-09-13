# ====================================================
# Various exports
# ====================================================

# Public include location
add_library(chimera_inc INTERFACE)
target_include_directories(chimera_inc INTERFACE ".")

if(FREERTOS)
  target_compile_definitions(chimera_inc INTERFACE USING_FREERTOS_THREADS)
endif()

export(TARGETS chimera_inc FILE "${PROJECT_BINARY_DIR}/Chimera/chimera-inc.cmake")