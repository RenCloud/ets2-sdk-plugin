# CMakeList.txt : CMake project for scs-telemetry, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 17)

set (VCPKG_TARGET_TRIPLET x64-windows-static)
set (CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1")
set (CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /DNDEBUG")

project (scs-telemetry VERSION 1.11.0)

include_directories ("inc")
include_directories ("../scs_sdk_1_12/include")

# Add source to this project's library.
add_library (scs-telemetry SHARED "scs-telemetry.def" "src/log.cpp" "src/scs_config_handlers.cpp" "src/scs_gameplay_event_handlers.cpp" "src/scs_telemetry.cpp" "src/sharedmemory.cpp" "src/publisher.cpp" )

# TODO: Add tests and install targets if needed.
find_package (cppzmq CONFIG REQUIRED)
target_link_libraries (scs-telemetry PRIVATE cppzmq cppzmq-static)
