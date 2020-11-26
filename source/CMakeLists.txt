# ====================================================
# Import sub-projects
# ====================================================
add_subdirectory("drivers")
add_subdirectory("sim")


# ====================================================
# All Drivers
# ====================================================
set(CHIMERA_SRC chimera_src)
add_library(${CHIMERA_SRC} INTERFACE)
target_link_libraries(${CHIMERA_SRC} INTERFACE
  chimera_algorithm
  chimera_allocator
  chimera_buffer
  chimera_common
  chimera_event
  chimera_pwm
  chimera_scheduler_low_res
  chimera_serial
  chimera_system
  chimera_threading
  chimera_usb
  chimera_peripheral_can
  chimera_peripheral_clock
  chimera_peripheral_crc
  chimera_peripheral_dma
  chimera_peripheral_exti
  chimera_peripheral_gpio
  chimera_peripheral_power
  chimera_peripheral_spi
  chimera_peripheral_timer
  chimera_peripheral_uart
  chimera_peripheral_usart
  chimera_peripheral_usb
  chimera_peripheral_watchdog
)
export(TARGETS ${CHIMERA_SRC} FILE "${PROJECT_BINARY_DIR}/Chimera/${CHIMERA_SRC}.cmake")