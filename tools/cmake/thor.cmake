# First, validate that we can find Thor
if(NOT THOR_ROOT)
    message(STATUS "THOR_ROOT not specified. Trying to find it from the environment.")
    if(NOT $ENV{THOR_ROOT} STREQUAL "")
        set(THOR_ROOT "$ENV{THOR_ROOT}")
        message(STATUS "Found THOR_ROOT: ${THOR_ROOT}")
    else()
        message(FATAL_ERROR "Please specify THOR_ROOT. I can't find it from the environment!")
    endif()
endif()

# Second, validate that we have SOMETHING for the chip we want to use. Let the included
# Thor build script determine if it actually is a supported target.
if(NOT STM32_CHIP)
    message(FATAL_ERROR "Please specify STM32_CHIP to compile Thor against!")
endif()

set(USING_CHIMERA "TRUE")
include("${THOR_ROOT}/CMakeLists.txt")

# Update the expected Chimera variables so the backend can be correctly built
set_property(GLOBAL PROPERTY CHIMERA_BACKEND_INC_DIRS "${THOR_INC_DIRS}")
set_property(GLOBAL PROPERTY CHIMERA_BACKEND_COMPILER_OPTIONS "${THOR_COMPILER_OPTIONS}")
set_property(GLOBAL PROPERTY CHIMERA_BACKEND_COMPILER_DEFINITIONS "${THOR_COMPILER_DEFINITIONS}")
set_property(GLOBAL PROPERTY CHIMERA_BACKEND_LIB_DBG "thor_dbg.a")
set_property(GLOBAL PROPERTY CHIMERA_BACKEND_LIB_REL "thor_rel.a")