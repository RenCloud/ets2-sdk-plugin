cmake_minimum_required(VERSION 3.19)
project(scs-telemetry)

set(CMAKE_CXX_STANDARD 20)

if(UNIX)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

include_directories("../scs_sdk/include")
include_directories("inc")

file(GLOB scs-telemetry-HEADERS
        "inc/*.h"
        "inc/*.hpp")

file(GLOB scs-telemetry-SOURCE
        "src/*.c"
        "src/*.cpp")

add_library(scs-telemetry SHARED ${scs-telemetry-HEADERS} ${scs-telemetry-SOURCE})