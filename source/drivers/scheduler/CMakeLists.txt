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
set(CHIMERA chimera_scheduler_low_res)
add_library(${CHIMERA} STATIC
  scheduler_low_res.cpp
)
target_link_libraries(${CHIMERA} PRIVATE ${LINK_LIBS})
export(TARGETS ${CHIMERA} FILE "${PROJECT_BINARY_DIR}/Chimera/src/${CHIMERA}.cmake")
