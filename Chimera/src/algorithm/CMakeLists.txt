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
set(CHIMERA chimera_algorithm)
add_library(${CHIMERA} STATIC
  register_optimizer.cpp
)
target_link_libraries(${CHIMERA} PRIVATE ${LINK_LIBS})
export(TARGETS ${CHIMERA} FILE "${PROJECT_BINARY_DIR}/Chimera/src/${CHIMERA}.cmake")
