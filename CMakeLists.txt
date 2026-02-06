cmake_minimum_required(VERSION 3.10)
project(indi_diy_serial_fw CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(INDI REQUIRED libindi)

add_library(indi_diy_serial_fw MODULE diy_serial_fw.cpp)

target_include_directories(indi_diy_serial_fw PRIVATE ${INDI_INCLUDE_DIRS})
target_compile_options(indi_diy_serial_fw PRIVATE ${INDI_CFLAGS_OTHER})
target_link_libraries(indi_diy_serial_fw PRIVATE ${INDI_LIBRARIES} indidriver)

# Avoid "lib" prefix so the module name matches the driver name
set_target_properties(indi_diy_serial_fw PROPERTIES PREFIX "")

