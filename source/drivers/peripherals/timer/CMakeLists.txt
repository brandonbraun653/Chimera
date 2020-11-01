# ====================================================
# Common
# ====================================================
set(LINK_LIBS
  aurora_inc
  chimera_inc       # Chimera public headers
  chimera_build_options
)

# ====================================================
# Interface Library
# ====================================================
set(CHIMERA chimera_peripheral_timer)
add_library(${CHIMERA} STATIC
  chimera_timer.cpp
)
target_link_libraries(${CHIMERA} PRIVATE ${LINK_LIBS})
export(TARGETS ${CHIMERA} FILE "${PROJECT_BINARY_DIR}/Chimera/src/${CHIMERA}.cmake")
