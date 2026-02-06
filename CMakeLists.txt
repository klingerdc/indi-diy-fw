# CMakeLists.txt for DIY Serial Firmware INDI Driver

cmake_minimum_required(VERSION 3.10)

project(diy_serial_fw LANGUAGES C CXX)

# Find dependencies
find_package(INDI REQUIRED)

# Add the executable for the INDI driver
add_executable(diy_serial_fw main.cpp)

# Link the INDI library
target_link_libraries(diy_serial_fw INDI::INDI)

# Set C++ standard
set_target_properties(diy_serial_fw PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)

# Install the driver
install(TARGETS diy_serial_fw DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)