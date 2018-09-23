set(CHIMERA_SUPPORTED_BACKENDS "thor" "esp32" "esp8266" )

## Check if the backend passed in matches anything we support
macro(CHIMERA_GET_BACKEND USER_BACKEND FOUND_BACKEND)
    if(${USER_BACKEND} IN_LIST CHIMERA_SUPPORTED_BACKENDS)
        set(${FOUND_BACKEND} ${USER_BACKEND})
    else()
        message(FATAL_ERROR "Invalid/unsupported backend!")
    endif()
endmacro()

## Get Thor Resources
function(GET_THOR_RESOURCES)
    # This is expecting to be called from the root Chimera CMakeLists.txt file
    include("${CMAKE_CURRENT_LIST_DIR}/cmake/thor.cmake")
endfunction()

## Gather the necessary resources used to compile Chimera features
function(CHIMERA_IMPORT_RESOURCES BACKEND)
    message(STATUS "Gathering resources for backend: ${BACKEND}")
    if(${BACKEND} STREQUAL "thor")
        GET_THOR_RESOURCES()
    elseif(${BACKEND} STREQUAL "esp32")
        message(STATUS "Backend esp32 selected, but it isn't fully supported yet.")
    elseif(${BACKEND} STREQUAL "esp8266")
        message(STATUS "Backend esp8266 selected, but it isn't fully supported yet.")
    else()
        message(FATAL_ERROR "Unknown backend [${BACKEND}]")
    endif()
endfunction()