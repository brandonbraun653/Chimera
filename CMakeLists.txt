# =============================================================================
# Description:
#   Primary CMake entry point for building Chimera
#
# Exports:
#   chimera_inc: Interface target for getting public headers and macro defs
#
# 2020 | Brandon Braun | brandonbraun653@gmail.com
# =============================================================================
# ====================================================
# Import sub-projects
# ====================================================
add_subdirectory("Chimera")

# ====================================================
# Public Headers
# ====================================================
set(PUB_HDR chimera_inc)
add_library(${PUB_HDR} INTERFACE)
target_include_directories(${PUB_HDR} INTERFACE ".")

# Select the appropriate threading interface (if any)
if(Toolchain::REQUIRES_FREERTOS_THREADS)
  target_compile_definitions(${PUB_HDR} INTERFACE USING_FREERTOS_THREADS)
  target_link_libraries(${PUB_HDR} INTERFACE freertos_inc)
  target_link_libraries(${PUB_HDR} INTERFACE freertos_port)
  target_link_libraries(${PUB_HDR} INTERFACE freertos_cfg)
elseif(Toolchain::REQUIRES_NATIVE_THREADS)
  target_compile_definitions(${PUB_HDR} INTERFACE USING_NATIVE_THREADS)
endif()

export(TARGETS ${PUB_HDR} FILE "${PROJECT_BINARY_DIR}/Chimera/${PUB_HDR}.cmake")

# ====================================================
# Custom Compiler Options:
# By default always build release variant
# ====================================================
set(BUILD_OPTIONS chimera_build_options)
add_library(${BUILD_OPTIONS} INTERFACE)
target_link_libraries(${BUILD_OPTIONS} INTERFACE
  prj_build_target
  #prj_build_target_rel
  #prj_build_target_dbg
  prj_device_target
)
export(TARGETS ${BUILD_OPTIONS} FILE "${PROJECT_BINARY_DIR}/Chimera/${BUILD_OPTIONS}.cmake")