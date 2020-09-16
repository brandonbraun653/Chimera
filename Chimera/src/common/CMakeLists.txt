# ====================================================
# Common
# ====================================================
set(LINK_LIBS
  aurora_inc        # Aurora public headers
  Boost::boost      # Boost public headers
  chimera_inc       # Chimera public headers
  prj_device_target # Compiler options for target device
)

# ====================================================
# Interface Library
# ====================================================
set(CHIMERA chimera_common)
add_library(${CHIMERA} STATIC
  boost_stubs.cpp
  chimera.cpp
  compiler_intf.cpp
)
target_link_libraries(${CHIMERA} PRIVATE ${LINK_LIBS})
export(TARGETS ${CHIMERA} FILE "${PROJECT_BINARY_DIR}/Chimera/src/${CHIMERA}.cmake")
