# ====================================================
# Common
# ====================================================
set(LINK_LIBS
  chimera_inc       # Chimera public headers
  prj_device_target # Compiler options for target device
)

# ====================================================
# Interface Library
# ====================================================
set(CHIMERA chimera_peripheral_crc)
add_library(${CHIMERA} STATIC
  chimera_crc.cpp
)
target_link_libraries(${CHIMERA} PRIVATE ${LINK_LIBS})
export(TARGETS ${CHIMERA} FILE "${PROJECT_BINARY_DIR}/Chimera/src/${CHIMERA}.cmake")
