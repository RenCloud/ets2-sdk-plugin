cmake_minimum_required(VERSION 3.19)
project(scs-telemetry)

set(CMAKE_CXX_STANDARD 20)

if(UNIX)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

include_directories("../scs_sdk/include")
include_directories("inc")

add_library(scs-telemetry SHARED
        inc/log.hpp
        inc/scs-telemetry-common.hpp
        inc/scs_config_handlers.hpp
        inc/scs_gameplay_event_handlers.hpp
        src/log.cpp
        src/scs_config_handlers.cpp
        src/scs_gameplay_event_handlers.cpp
        src/scs_telemetry.cpp)