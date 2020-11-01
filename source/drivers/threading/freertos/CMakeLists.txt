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
set(CHIMERA chimera_threading_freertos)
add_library(${CHIMERA} STATIC
  freertos_hooks.cpp
  freertos_mutex.cpp
  freertos_queue.cpp
  freertos_semaphore.cpp
  freertos_thread.cpp
)
target_link_libraries(${CHIMERA} PRIVATE ${LINK_LIBS})
export(TARGETS ${CHIMERA} FILE "${PROJECT_BINARY_DIR}/Chimera/src/${CHIMERA}.cmake")
