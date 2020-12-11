# ====================================================
# Common
# ====================================================
set(LINK_LIBS
  chimera_inc       # Chimera public headers
  chimera_build_options
)

# ====================================================
# Interface Library
# ====================================================
set(CHIMERA_DRIVER chimera_peripheral_adc)
add_library(${CHIMERA_DRIVER} STATIC
  chimera_adc.cpp
)
target_link_libraries(${CHIMERA_DRIVER} PRIVATE ${LINK_LIBS})
export(TARGETS ${CHIMERA_DRIVER} FILE "${PROJECT_BINARY_DIR}/Chimera/src/${CHIMERA_DRIVER}.cmake")
