# ====================================================
# Import sub-projects
# ====================================================
add_subdirectory("Chimera")

# ====================================================
# Various exports
# ====================================================

# Public include location
add_library(chimera_inc INTERFACE)
target_include_directories(chimera_inc INTERFACE ".")

if(FREERTOS)
  target_compile_definitions(chimera_inc INTERFACE USING_FREERTOS_THREADS)
  target_link_libraries(chimera_inc INTERFACE freertos_inc)
  target_link_libraries(chimera_inc INTERFACE freertos_port)
  target_link_libraries(chimera_inc INTERFACE freertos_cfg)
endif()

export(TARGETS chimera_inc FILE "${PROJECT_BINARY_DIR}/Chimera/chimera-inc.cmake")