cmake_minimum_required(VERSION 3.10)

# Project name
project(diy_serial_fw)

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source files
set(SOURCES
    src/main.cpp
    src/serial_driver.cpp
    src/indi_driver.cpp
)

# Include Header files
include_directories(include)

# Create the executable
add_executable(diy_serial_fw ${SOURCES})

# Link to the INDI library
find_package(INDI REQUIRED)
include_directories(${INDI_INCLUDE_DIRS})

target_link_libraries(diy_serial_fw ${INDI_LIBRARIES})
