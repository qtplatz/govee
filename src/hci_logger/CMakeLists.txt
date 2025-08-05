cmake_minimum_required(VERSION 3.30)
project(hci_logger LANGUAGES CXX)

find_library(BLUETOOTH_LIB bluetooth REQUIRED)
include_directories( "${QTPLATZ_INCLUDE_DIRS}" "${Boost_INCLUDE_DIRS}")

add_executable(${PROJECT_NAME}
  main.cpp
  parse_govee_payload.cpp parse_govee_payload.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Required libs
target_link_libraries(${PROJECT_NAME} PRIVATE
  QtPlatz::adportable
  ${BLUETOOTH_LIB}
)

# Install executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
