include("${COMMON_TOOL_ROOT}/cmake/utility/embedded.cmake")

# ====================================================
# Common
# ====================================================
set(LINK_LIBS
  chimera_intf_inc       # Chimera public headers
  aurora_intf_inc
)

# ====================================================
# Interface Library
# ====================================================
function(build_library variant)
  set(CHIMERA chimera_peripheral_dma${variant})
  add_library(${CHIMERA} STATIC
    chimera_dma.cpp
  )
  target_link_libraries(${CHIMERA} PRIVATE ${LINK_LIBS} prj_build_target${variant} prj_device_target)
  export(TARGETS ${CHIMERA} FILE "${PROJECT_BINARY_DIR}/Chimera/src/${CHIMERA}.cmake")
endfunction()

add_target_variants(build_library)
