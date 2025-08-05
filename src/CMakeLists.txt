cmake_minimum_required(VERSION 3.30)
project(switchbot-monitor LANGUAGES CXX)
set ( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Version from Git (fallback to 0.1.0)
execute_process(
  COMMAND git describe --tags --always --dirty
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
if (NOT GIT_VERSION)
  set(GIT_VERSION "0.1.0")
endif()

# Output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package( QtPlatz REQUIRED )
include_directories( "${QTPLATZ_INCLUDE_DIRS}" "${Boost_INCLUDE_DIRS}")

find_library(BLUETOOTH_LIB bluetooth REQUIRED)

add_subdirectory( switchbot_monitor )
add_subdirectory( simple )
add_subdirectory( hci_logger )
add_subdirectory( switchbot_ble_parser )
add_subdirectory( ble_scanner )
