# ====================================================
# Import sub-projects
# ====================================================
add_subdirectory("freertos")
add_subdirectory("stl")


# ====================================================
# Exports
# ====================================================
add_library(chimera_threading INTERFACE)

if(FREERTOS)
  target_link_libraries(chimera_threading INTERFACE chimera_threading_freertos)
else()
  target_link_libraries(chimera_threading INTERFACE chimera_threading_stl)
endif()

export(TARGETS chimera_threading FILE "${PROJECT_BINARY_DIR}/Chimera/chimera_threading.cmake")