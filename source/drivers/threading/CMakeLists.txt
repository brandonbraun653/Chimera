include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

# ====================================================
# Import sub-projects
# ====================================================
add_subdirectory("common")
add_subdirectory("freertos")
add_subdirectory("stl")


# ====================================================
# Exports
# ====================================================
function(build_library variant)
  add_library(chimera_threading${variant} INTERFACE)
  target_link_libraries(chimera_threading${variant} INTERFACE chimera_threading_common${variant})

  if(Toolchain::REQUIRES_FREERTOS_THREADS)
    target_link_libraries(chimera_threading${variant} INTERFACE chimera_threading_freertos${variant})
  else()
    target_link_libraries(chimera_threading${variant} INTERFACE chimera_threading_stl${variant})
  endif()

  export(TARGETS chimera_threading${variant} FILE "${PROJECT_BINARY_DIR}/Chimera/chimera_threading${variant}.cmake")
endfunction()
add_target_variants(build_library)

