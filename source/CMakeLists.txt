# ====================================================
# Import sub-projects
# ====================================================
add_subdirectory("drivers")


# ====================================================
# Collections of all Chimera drivers
# ====================================================
set(all_libraries
  chimera_algorithm
  chimera_allocator
  chimera_assert
  chimera_buffer
  chimera_common
  chimera_event
  chimera_pwm
  chimera_scheduler_low_res
  chimera_serial
  chimera_system
  chimera_threading
  chimera_usb
  chimera_utilities
  chimera_peripheral_adc
  chimera_peripheral_can
  chimera_peripheral_clock
  chimera_peripheral_crc
  chimera_peripheral_dma
  chimera_peripheral_exti
  chimera_peripheral_gpio
  chimera_peripheral_i2c
  chimera_peripheral_interrupt
  chimera_peripheral_power
  chimera_peripheral_spi
  chimera_peripheral_timer
  chimera_peripheral_uart
  chimera_peripheral_usart
  chimera_peripheral_usb
  chimera_peripheral_watchdog
)


# ====================================================
# This variant follows the CMAKE_BUILD_TYPE specs
# ====================================================
set(CHIMERA_SRC chimera_core)
add_library(${CHIMERA_SRC} INTERFACE)
target_link_libraries(${CHIMERA_SRC} INTERFACE ${all_libraries})
export(TARGETS ${CHIMERA_SRC} FILE "${PROJECT_BINARY_DIR}/Chimera/${CHIMERA_SRC}.cmake")


# ====================================================
# Builds all targets as release
# ====================================================
set(all_libraries_rel "")
foreach(lib ${all_libraries})
  list(APPEND all_libraries_rel ${lib}_rel)
endforeach()

set(CHIMERA_SRC_REL chimera_core_rel)
add_library(${CHIMERA_SRC_REL} INTERFACE)
target_link_libraries(${CHIMERA_SRC_REL} INTERFACE ${all_libraries_rel})
export(TARGETS ${CHIMERA_SRC_REL} FILE "${PROJECT_BINARY_DIR}/Chimera/${CHIMERA_SRC_REL}.cmake")


# ====================================================
# Builds all targets as release with debug symbols
# ====================================================
set(all_libraries_rel_dbg "")
foreach(lib ${all_libraries})
  list(APPEND all_libraries_rel_dbg ${lib}_rel_dbg)
endforeach()

set(CHIMERA_SRC_REL_DBG chimera_core_rel_dbg)
add_library(${CHIMERA_SRC_REL_DBG} INTERFACE)
target_link_libraries(${CHIMERA_SRC_REL_DBG} INTERFACE ${all_libraries_rel_dbg})
export(TARGETS ${CHIMERA_SRC_REL_DBG} FILE "${PROJECT_BINARY_DIR}/Chimera/${CHIMERA_SRC_REL_DBG}.cmake")


# ====================================================
# Builds all targets as debug only
# ====================================================
set(all_libraries_dbg "")
foreach(lib ${all_libraries})
  list(APPEND all_libraries_dbg ${lib}_dbg)
endforeach()

set(CHIMERA_SRC_DBG chimera_core_dbg)
add_library(${CHIMERA_SRC_DBG} INTERFACE)
target_link_libraries(${CHIMERA_SRC_DBG} INTERFACE ${all_libraries_dbg})
export(TARGETS ${CHIMERA_SRC_DBG} FILE "${PROJECT_BINARY_DIR}/Chimera/${CHIMERA_SRC_DBG}.cmake")
