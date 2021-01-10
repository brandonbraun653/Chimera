# =============================================================================
# Description:
#   Primary CMake entry point for building Chimera
#
# Exports:
#   chimera_intf_inc: Interface target for getting public headers and macro defs
#
# 2020 | Brandon Braun | brandonbraun653@gmail.com
# =============================================================================
# ====================================================
# Import sub-projects
# ====================================================
add_subdirectory("source")

# ====================================================
# Public Headers
# ====================================================
set(PUB_HDR chimera_intf_inc)
add_library(${PUB_HDR} INTERFACE)
target_include_directories(${PUB_HDR} INTERFACE "./../" "./include")
target_link_libraries(${PUB_HDR} INTERFACE etl_inc  Boost::boost)

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
