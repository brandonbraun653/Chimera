cmake_minimum_required(VERSION 3.12.2)
include("cmake/chimera.cmake")

# --------------------------------------
# Chimera Directories
# --------------------------------------
set(CHIMERA_ROOT "${CMAKE_CURRENT_LIST_DIR}")

# Set this property for whatever target library chimera is wrapping around.
# It expects the target library to use chimera includes like "#include <Chimera/xyz.hpp>"
set(CHIMERA_HEADERS_DIRS "${CHIMERA_ROOT}")
set_property(GLOBAL PROPERTY CHIMERA_HEADERS_DIR "${CHIMERA_HEADERS_DIRS}")

set(CHIMERA_SRC_FILES
    "${CHIMERA_ROOT}/Chimera/chimera.cpp"
    "${CHIMERA_ROOT}/Chimera/logging.cpp"
    "${CHIMERA_ROOT}/Chimera/threading.cpp"
)

# --------------------------------------
# Validate the specified backend
# --------------------------------------
if(NOT CHIMERA_BACKEND)
    message(FATAL_ERROR "Please set a desired backend using -DCHIMERA_BACKEND. Available options are [${CHIMERA_SUPPORTED_BACKENDS}]")
else()
    CHIMERA_GET_BACKEND(${CHIMERA_BACKEND} CHIMERA_BACKEND_TARGET)
    CHIMERA_IMPORT_RESOURCES(${CHIMERA_BACKEND_TARGET})
endif()

get_property(CHIMERA_BACKEND_INC_DIRS GLOBAL PROPERTY CHIMERA_BACKEND_INC_DIRS)
get_property(CHIMERA_BACKEND_COMPILER_OPTIONS GLOBAL PROPERTY CHIMERA_BACKEND_COMPILER_OPTIONS)
get_property(CHIMERA_BACKEND_COMPILER_DEFINITIONS GLOBAL PROPERTY CHIMERA_BACKEND_COMPILER_DEFINITIONS)
get_property(CHIMERA_BACKEND_LIB_DBG GLOBAL PROPERTY CHIMERA_BACKEND_LIB_DBG)
get_property(CHIMERA_BACKEND_LIB_REL GLOBAL PROPERTY CHIMERA_BACKEND_LIB_REL)

# message(STATUS "Chimera backend inc: ${CHIMERA_BACKEND_INC_DIRS}")
# message(STATUS "Chimera backend co: ${CHIMERA_BACKEND_COMPILER_OPTIONS}")
# message(STATUS "Chimera backend cd: ${CHIMERA_BACKEND_COMPILER_DEFINITIONS}")
# message(STATUS "Chimera backend lib dbg: ${CHIMERA_BACKEND_LIB_DBG}")
# message(STATUS "Chimera backend lib rel: ${CHIMERA_BACKEND_LIB_REL}")

# --------------------------------------
# Set and export compiler vars
# --------------------------------------
set(CHIMERA_INC_DIRS
    "${CHIMERA_HEADERS_DIRS}"
    "${CHIMERA_BACKEND_INC_DIRS}"
)
set_property(GLOBAL PROPERTY CHIMERA_INC_DIRS "${CHIMERA_INC_DIRS}")

set(CHIMERA_COMPILER_OPTIONS
    "${CHIMERA_BACKEND_COMPILER_OPTIONS}"
)
set_property(GLOBAL PROPERTY CHIMERA_COMPILER_OPTIONS "${CHIMERA_COMPILER_OPTIONS}")

set(CHIMERA_COMPILER_DEFINITIONS
    "${CHIMERA_BACKEND_COMPILER_DEFINITIONS}"
)
set_property(GLOBAL PROPERTY CHIMERA_COMPILER_DEFINITIONS "${CHIMERA_COMPILER_DEFINITIONS}")

# --------------------------------------
# Generate the project
# --------------------------------------
project(chimera)

add_library(${PROJECT_NAME} ${CHIMERA_SRC_FILES})

# Force the link language to C++
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# Include Directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CHIMERA_INC_DIRS})

# Compiler Definitions 
target_compile_definitions(${PROJECT_NAME} PUBLIC ${CHIMERA_COMPILER_DEFINITIONS})

# Compiler Options
target_compile_options(${PROJECT_NAME} PRIVATE ${CHIMERA_COMPILER_OPTIONS})
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:-ggdb -Og>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:RELEASE>:-O3>)

# Linked Libraries

set(CHIMERA_LINK_LIBS_DBG "${CHIMERA_BACKEND_LIB_DBG}")
set_property(GLOBAL PROPERTY CHIMERA_LINK_LIBS_DBG ${CHIMERA_LINK_LIBS_DBG})

set(CHIMERA_LINK_LIBS_REL "${CHIMERA_BACKEND_LIB_REL}")
set_property(GLOBAL PROPERTY CHIMERA_LINK_LIBS_REL ${CHIMERA_LINK_LIBS_REL})

target_link_libraries(${PROJECT_NAME} $<$<CONFIG:DEBUG>: ${CHIMERA_LINK_LIBS_DBG}>)
target_link_libraries(${PROJECT_NAME} $<$<CONFIG:RELEASE>: ${CHIMERA_LINK_LIBS_REL}>)